// Copyright 2020 Apulis Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package main

import (
	"flag"
	"path"
	"regexp"
	//"strings"
	"fmt"
	"github.com/apulis/k8s-device-plugin/cmd/debug"
	dpapi "github.com/apulis/k8s-device-plugin/cmd/deviceplugin"
	"github.com/pkg/errors"
	"io/ioutil"
	pluginapi "k8s.io/kubelet/pkg/apis/deviceplugin/v1beta1"
	"os"
	"time"
)

const (
	sysfsDirectory  = "/sys/class/devdrv-class"
	devfsDirectory  = "/dev"
	npuDeviceRE     = `^davinci[0-9]+$`
	controlDeviceRE = `^controlD[0-9]+$` // looks like we won't use this variable for Huawei device.
	vendorString    = "????"             // Not sure the vendor string is important. Maybe not at this point.

	// Device plugin settings.
	namespace  = "npu.huawei.com"
	deviceType = "NPU"
)

type devicePlugin struct {
	sysfsDir string
	devfsDir string

	sharedDevNum int

	npuDeviceReg     *regexp.Regexp
	controlDeviceReg *regexp.Regexp
}

func newDevicePlugin(sysfsDir, devfsDir string, sharedDevNum int) *devicePlugin {
	return &devicePlugin{
		sysfsDir:         sysfsDir,
		devfsDir:         devfsDir,
		sharedDevNum:     sharedDevNum,
		npuDeviceReg:     regexp.MustCompile(npuDeviceRE),
		controlDeviceReg: regexp.MustCompile(controlDeviceRE),
	}
}

func (dp *devicePlugin) Scan(notifier dpapi.Notifier) error {
	for {
		debug.Printf("...Scan devices ...")
		devTree, err := dp.scan()
		if err != nil {
			fmt.Println("WARNING: Failed to scan: ", err)
		}

		notifier.Notify(devTree)

		time.Sleep(5 * time.Second)
	}
}

func (dp *devicePlugin) scan() (dpapi.DeviceTree, error) {
	files, err := ioutil.ReadDir(dp.sysfsDir)
	if err != nil {
		return nil, errors.Wrap(err, "Can't read sysfs folder")
	}

	devTree := dpapi.NewDeviceTree()
	for _, f := range files {
		var nodes []pluginapi.DeviceSpec

		debug.Print("......Find device:", f.Name())
		if !dp.npuDeviceReg.MatchString(f.Name()) {
			debug.Print("Not compatible device", f.Name())
			continue
		}

		// Based on the directory structure, /sys/class/devdrv-class/davinci0 contains:
		//     dev power* subsystem* uevent    (* denotes directory)
		//     dev -->   240:0
		//     uevent:
		//        MAJOR=214
		//        MINOR=0
		//        DEVNAME=davinci0
		//        DEVMODE=0666
		//

		/***   comment out this for now for future use
		dat, err := ioutil.ReadFile(path.Join(dp.sysfsDir, f.Name(), "uevent"))
		if err != nil {
			fmt.Println("WARNING: Skipping. Can't read vendor file: ", err)
			continue
		}

		if strings.TrimSpace(string(dat)) != vendorString {
			debug.Print("Non-Huawei NPU", f.Name())
			continue
		}*/

		// At this point, no further checking, directly treat this device is A910 device (Maybe not ideal).
		// We can change the logic if we get more information from HUAWEI
		devPath := path.Join(dp.devfsDir, f.Name())
		if _, err := os.Stat(devPath); err != nil {
			continue
		}

		debug.Printf("......Adding %s to NPU %s", devPath, f.Name())
		nodes = append(nodes, pluginapi.DeviceSpec{
			HostPath:      devPath,
			ContainerPath: devPath,
			Permissions:   "rwm",
		})

		if len(nodes) > 0 {
			for i := 0; i < dp.sharedDevNum; i++ {
				// Later, need to find the device number from this string
				devID := fmt.Sprintf("%s-%d", f.Name(), i)
				// TODO: check model ID to differentiate device models.
				devTree.AddDevice(deviceType, devID, dpapi.DeviceInfo{
					State: pluginapi.Healthy,
					Nodes: nodes,
				})
				debug.Printf("......device %s added to the tree", devID)
			}
		}
	}

	return devTree, nil
}

func main() {
	var sharedDevNum int
	var debugEnabled bool

	flag.IntVar(&sharedDevNum, "shared-dev-num", 1, "number of containers sharing the same NPU device")
	flag.BoolVar(&debugEnabled, "debug", false, "enable debug output")
	flag.Parse()

	if debugEnabled {
		debug.Activate()
	}

	if sharedDevNum < 1 {
		fmt.Println("The number of containers sharing the same NPU must greater than zero")
		os.Exit(1)
	}

	fmt.Println("NPU device plugin started")

	plugin := newDevicePlugin(sysfsDirectory, devfsDirectory, sharedDevNum)
	manager := dpapi.NewManager(namespace, plugin)
	manager.Run()
}

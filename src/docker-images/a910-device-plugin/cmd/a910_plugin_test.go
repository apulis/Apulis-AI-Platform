package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"path"
	"testing"
	"time"

	"github.com/apulis/k8s-device-plugin/cmd/debug"
)

func init() {
	debug.Activate()
}

func TestScan(t *testing.T) {
	tmpdir := fmt.Sprintf("/tmp/npuplugin-test-%d", time.Now().Unix())
	sysfs := path.Join(tmpdir, "sysfs")
	devfs := path.Join(tmpdir, "devfs")
	testcases := []struct {
		devfsdirs    []string
		sysfsdirs    []string
		sysfsfiles   map[string][]byte
		expectedDevs int
		expectedErr  bool
	}{
		{
			expectedErr:  true,
			expectedDevs: 0,
		},
		{
			sysfsdirs:    []string{"davinci0"},
			expectedDevs: 0,
			expectedErr:  false,
		},
		{
			sysfsdirs: []string{"davinci0/uevent"},
			sysfsfiles: map[string][]byte{
				"card0/device/vendor": []byte("0x8086"),
			},
			expectedDevs: 0,
			expectedErr:  true,
		},
		{
			sysfsdirs:    []string{"non_gpu_card"},
			expectedDevs: 0,
			expectedErr:  false,
		},
	}

	testPlugin := newDevicePlugin(sysfs, devfs, 1)

	if testPlugin == nil {
		t.Fatal("Failed to create a deviceManager")
	}

	for _, testcase := range testcases {
		for _, devfsdir := range testcase.devfsdirs {
			err := os.MkdirAll(path.Join(devfs, devfsdir), 0755)
			if err != nil {
				t.Fatalf("Failed to create fake device directory: %+v", err)
			}
		}
		for _, sysfsdir := range testcase.sysfsdirs {
			err := os.MkdirAll(path.Join(sysfs, sysfsdir), 0755)
			if err != nil {
				t.Fatalf("Failed to create fake device directory: %+v", err)
			}
		}
		for filename, body := range testcase.sysfsfiles {
			err := ioutil.WriteFile(path.Join(sysfs, filename), body, 0644)
			if err != nil {
				t.Fatalf("Failed to create fake vendor file: %+v", err)
			}
		}

		tree, err := testPlugin.scan()
		if testcase.expectedErr && err == nil {
			t.Error("Expected error hasn't been triggered")
		}
		if !testcase.expectedErr && err != nil {
			t.Errorf("Unexpcted error: %+v", err)
		}
		if testcase.expectedDevs != len(tree[deviceType]) {
			t.Errorf("Wrong number of discovered devices")
		}

		err = os.RemoveAll(tmpdir)
		if err != nil {
			t.Fatalf("Failed to remove fake device directory: %+v", err)
		}
	}
}

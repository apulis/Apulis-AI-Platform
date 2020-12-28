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

package deviceplugin

import (
	"fmt"
	"errors"
	"strconv"
)

var (
	InvalidId = errors.New("Invalid NPU number")
	WrongGrouping = errors.New("wrong grouping!")
)

// This internal unexposed interface simplifies unit testing.
type grouping interface {

	// check if target devices fits the allocation policy
	validate(devices []string) (bool, error)
}

// atlasNPUGrouping implements grouping interface
type atlasNPUGrouping  struct {
}

func (a *atlasNPUGrouping) validate(devices []string) (bool, error)  {

	var allocated []uint
	var needs int = len(devices)

	for _, val := range(devices) {
		i, err := strconv.ParseUint(val, 10, 32)
		if err == nil {
			allocated = append(allocated, uint(i))
		} else {
			return false, InvalidId
		}
	}

	// request all devices, return true immediately
	if needs >= 8 {
		return true, nil
	}

	maxId := MaxUIntSlice(allocated)
	minId := MinUIntSlice(allocated)
	fmt.Printf("maxId: %+v, minId: %+v\n", maxId, minId)

	// first group: [0, 3]
	// second group: [4, 7]
	// As memos from huawei: devices can't span across groups
	if minId <= 3 && maxId >=4 {
		return false, WrongGrouping
	}

	return true, nil
}

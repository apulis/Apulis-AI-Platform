import _ from "lodash";
import React from "react";
const { DateTime } = require('luxon');
export const checkObjIsEmpty = (obj: object) => {
  return _.keys(obj).length === 0;
}

export const mergeTwoObjsByKey = (objs: any[], otherObjs: any[] ,key: string) => {
  const merged = _.merge(_.keyBy(objs, key), _.keyBy(otherObjs, key));
  return merged;
}

export const convertToArrayByKey = (objs: any, key: string) => {
  return _.map(objs,key);
}
export const filterByEventValue = (objs: any, key: string ,event: React.ChangeEvent<HTMLInputElement>) => {
  const  filtered= objs.filter((item: any)=>item[key] === event.target.value);
  return filtered
}

export const sumValues= (obj: any) => {
  if (!obj) return 0;
  if (typeof obj === 'number') return obj < 0 ? 0 : obj;
  let total = 0;
  total = _.sum(Object.values(obj))
  return total < 0 ? 0 : total;
}

export const toLocalTime = (data: any) => {
  if (!data) {
    return ""
  }
  if (new Date(data).toString().indexOf('Invalid') !== -1) {
    data = data.concat("0");
  }
  return DateTime.fromJSDate(new Date(Date.parse(data))).toFormat("yyyy/LL/dd HH:mm:ss")
}

export const mergePropertyByKey = (arr1: any[], arr2: any[], key: string) => {
  const result = arr1.map(val1 => {
    const property1 = val1[key];
    arr2.forEach(val2 => {
      const property2 = val2[key];
      if (property1 === property2) {
        val1 = Object.assign({}, val1, val2)
      }
    });
    return val1;
  })
  return result;
}

export const judgeContainsSameKey = (arr1: any[], arr2: any[], keys: string[]) => {

}
export const mergePropertyByUserNameAndGPUType = (arr1: any[], arr2: any[]) => {
  console.log('arr', arr1, arr2)
  const result: any = [];
  // userName 和 gpu 相同的部分：合并
  arr1.forEach(val1 => {
    arr2.forEach((val2) => {
      if (val1.userName === val2.userName && val1.gpuType === val2.gpuType) {
        val1.usedGPU = val2.usedGPU;
        const newObj = Object.assign({}, val1, val2);
        result.push(newObj);
      }
    })
  })
  const mergedArrUserNameAndGpu = result.map((val: any) => val.userName + '-----' + val.gpuType).join('');
  console.log('mergedArrUserNameAndGpu', mergedArrUserNameAndGpu)
  // 不存在相同的部分，则直接push进去
  arr1.forEach(val => {
    const { userName, gpuType } = val;
    const key = userName + '-----' + gpuType;
    if (!mergedArrUserNameAndGpu.includes(key)) {
      result.push(val);
    }
  });
  arr2.forEach(val => {
    const { userName, gpuType } = val;
    const key = userName + '-----' + gpuType;
    if (!mergedArrUserNameAndGpu.includes(key)) {
      result.push(val);
    }
  })
  console.log('result', result)
  return result;
}

export const sumObjectValues = (obj: any) => {
  let count = 0;
  for (const key in obj) {
    count += obj[key];
  }
  return count;
}
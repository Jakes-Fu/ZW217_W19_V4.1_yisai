
/*
  * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
  * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
  * Copyright 2021-present Unisoc (Shanghai) Technologies Co., Ltd. 
  * Licensed under the Unisoc General Software License V1; 
  * you may not use this file except in compliance with the License. 
  * You may obtain a copy of the License at 
  * Software distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OF ANY KIND, either express or implied. 
  * See the Unisoc General Software License v1 for more details.
  * 
  * Created on: 2022/03/01
  * Author: jie.yang@unisoc.com
  */

function any2number(newValue) {
    if ('string' == typeof newValue) {
        if (newValue.indexOf('0x') != -1) {
            return parseInt(newValue, 16);
        } else {
            return parseInt(newValue, 10);
        }

    } else {
        return newValue;
    }

}

function deepCopy(data, hash = new WeakMap()) {
    if (typeof data !== 'object' || data === null) {
        throw new TypeError('data is not object');
    }

    if (hash.has(data)) {
        return hash.get(data);
    }

    let newData = {};
    const dataKeys = Object.keys(data);
    dataKeys.forEach(value => {
        const currentDataValue = data[value];
        if (typeof currentDataValue !== "object" || currentDataValue === null) {
            newData[value] = currentDataValue;
        } else if (Array.isArray(currentDataValue)) {
            newData[value] = [...currentDataValue];
        } else if (currentDataValue instanceof Set) {
            newData[value] = new Set([...currentDataValue]);
        } else if (currentDataValue instanceof Map) {
            newData[value] = new Map([...currentDataValue]);
        } else {
            hash.set(data, data)
            newData[value] = deepCopy(currentDataValue, hash);
        }
    });

    return newData;
}



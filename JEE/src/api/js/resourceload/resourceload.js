/**
  * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
  * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
  *
  * Copyright 2022-present Unisoc (Shanghai) Technologies Co., Ltd.
  * Licensed under the Unisoc General Software License V1;
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  * Software distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
  * See the Unisoc General Software License v1 for more details.
  *
**/

/**
 *****************************************************************************
 * @file     resourceload.js
 * @brief    -
 * @author  zhigao.sun@unisoc.com
 * @version  V1.1.0
 * @date
 * @history
 * @note
 * detailed description:
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 unisoc </center></h2>
 **/
device = require("device");

function getSystemMcc() {
    return null;
}

function getSystemMnc() {
    return null;
}

function getResourceResolution() {
    return device.screenWidth + 'x' + device.screenHeight;
}

function getMatchedDir(dirs, patterns) {
    console.log("getMatchedDir() loading");
    var idx = dirs.indexOf(patterns.join('-'));
    if (idx != -1) {
        return dirs[idx];
    }

    const patternsReversed = [...patterns].reverse();
    var max;
    dirs = dirs.map(dir => {
        //caculate each dir's weight and match count
        var dirParts = dir.split('-');
        var weightValue = 0;
        var matchValue = 0;
        for (const d of dirParts) {
            var idx = patternsReversed.indexOf(d);
            if (idx == -1) {
                weightValue = 0;
                break;
            }
            matchValue++;
            weightValue += Math.pow(2, idx);
        }
        return { dirName: dir, weight: weightValue, matchCount: matchValue };
    })
        .filter(dir => (dir.weight != 0) || (dir.dirName === 'common'))
        .sort((a, b) => b.matchCount - a.matchCount)
        .filter(dir => {
            if (!max) {
                max = dir.matchCount;
                return true;
            }
            return dir.matchCount == max;
        }
        )
        .sort((a, b) => b.weight - a.weight);

    console.log(dirs);
    if (dirs.length != 0) {
        return dirs[0].dirName;
    } else {
        return null;
    }
}

export function $r(filename) {
    console.log("resourceload.js loading");
    var appPathName = js_art_getAppRoot();
    if (null == appPathName) {
        return null;
    }
    var dirNames = js_get_all_resource_dir_info();
    if (null == dirNames) {
        return null;
    }
    var dirArray = [];
    for (var i = 0; i < dirNames.length; i++) {
        dirArray[i] = dirNames[i].dirName;
    }

    var fileNames = filename.split(':');
    var fileNameType = fileNames[0];

    var patterns = [];
    patterns.push(getResourceResolution());

    var filePath = "localfile://res/" + fileNameType + "/" + getMatchedDir(dirArray, patterns) + "/" + fileNames[1];
    return filePath;

}

export function $r_img(filename) {
    return $r("img:" + filename);
}

export function $r_str(filename) {
    return $r("str:" + filename);
}

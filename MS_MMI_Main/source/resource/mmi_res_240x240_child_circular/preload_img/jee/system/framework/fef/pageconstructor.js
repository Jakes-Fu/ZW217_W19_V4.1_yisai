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
 * @file     pageconstructor.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/


import { PageObj } from "pageobj.js";
import { PageStatusMachine } from "pagestatusmachine.js";
import { LogicObject } from "logicobj.js";
import { ViewObject } from "viewobj.js";

function closureConstructor(uri) {
    return closure_export_object(uri);
}

function constructorLogicObject(obj) {
    if (obj == null) {
        return new LogicObject();
    } else {
        return deepCopy(obj);
    }
}

function constructorViewObject(obj) {
    if (obj == null) {
        return new ViewObject();
    } else {
        return deepCopy(obj);
    }
}

export function PageConstructor(pm, uri, init_data) {
    let po = new PageObj(uri, init_data);

    //pageObj.logic_obj = closure_constructor(uri);
    //pageObj.view_obj = closure_constructor(uri);
    let obj = pm.page_pool[uri];
    po.logic_obj = constructorLogicObject(obj["logic"]);
    po.view_obj = constructorViewObject(obj["view"]);
    po.status_machine = new PageStatusMachine(po);

    return po;
}

console.log("pageconstructor.js ok");
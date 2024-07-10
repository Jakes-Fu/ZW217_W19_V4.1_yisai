/*
*SPDX-FileCopyrightText: 2021-2022 Unisoc (Shanghai) Technologies Co., Ltd
*SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
*Copyright 2021-2022 Unisoc (Shanghai) Technologies Co., Ltd.
*Licensed under the Unisoc General Software License V1;
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*Software distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OF ANY KIND, either express or implied.
*See the Unisoc General Software License v1 for more details.

*NOTE: this is a machine generated file(uxml-compiler 1.0.11) - editing not recommended
*2022/5/13 下午5:34:54
*/
import { Page } from 'page';
import { Button } from 'button';
import { Label } from 'label';
import { Editbox } from 'editbox';
import { List } from 'list';
import { Image } from 'image';
import { $r } from 'resourceload';
import { Progressbar } from 'progressbar';
import { Div } from 'div';
export default {
  _ui_start: function (logic) {
    let e0, e1, e2, e3;

    e0 = new Page();
    e0.create(null, 'undefined');

    e1 = new Div();
    e1.create(e0, 'undefined');

    e2 = new Label();
    e2.create(e1, 'undefined');


    e0.setAttribute("background-color", "0x0");

    e1.setAttribute("background-color", "0x0000");
    e1.setAttribute("padding", "20");


    e2.setAttribute("value", getApp().msgString);
    e2.setAttribute("fontsize", "24");
    e2.setAttribute("color", "0x8000");
    e2.setAttribute("align", "center");


    e1.addChild(e2);

    return e0;
  }
}
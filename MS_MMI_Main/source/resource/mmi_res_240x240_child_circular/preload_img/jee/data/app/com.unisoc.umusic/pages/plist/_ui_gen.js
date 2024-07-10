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

*NOTE: this is a machine generated file(uxml-compiler 1.0.12) - editing not recommended
*2022/7/7 下午2:42:52
*/
import { Page } from 'page';
import { Button } from 'button';
import { Label } from 'label';
import { Editbox } from 'editbox';
import { List } from 'list';
import { Image } from 'image';
import { Audio } from 'audio';
import { $r } from 'resourceload';
import { Progressbar } from 'progressbar';
import { Div } from 'div';
export default {
  _ui_start: function(logic) {
    let e0, e1, e2, e3;

    e0 = new Page();
    e0.create(null, 'undefined');

    //e1 = new Div();
    //e1.create(e0, 'undefined');

    e2 = new Label();
    e2.create(e0, 'undefined');

    e3 = new List();
    e3.create(e0, 'undefined');

    e0.setAttribute("background-color", "0x0");

    //e1.setAttribute("background-color", "0x0000");
    //e1.setAttribute("padding", "6");

    e2.setAttribute("value", $t('strings.title'));
    e2.setAttribute("font-size", "24");
    e2.setAttribute("color", "0x8000");
    e2.setAttribute("text-align", "center");
    e2.setAttribute("width", "240");
    e2.setAttribute("height", "30");
    e2.setAttribute("x", "0");
    e2.setAttribute("y", "5");

    e3.setAttribute("item-max", "15");
    e3.setAttribute("background-color", "0x0");
    e3.setAttribute("fontColor", "0xffff");
    e3.setAttribute("isscroll", "1");
    e3.setAttribute("value", "{{songs}}");
    e3.setAttribute("fontSize", "20");
    e3.setAttribute("width", "168");
    e3.setAttribute("height", "180");
    e3.setAttribute("x", "36");
    e3.setAttribute("y", "30");

    //e1.addChild(e2);
    //e1.addChild(e3);

    e3.addEventListener("click", function(event) {
      logic.listClicked(event);
    }.bind(logic));

    _mvvm_bind(logic.data, e0);
    //_mvvm_bind(logic.data, e1);
    _mvvm_bind(logic.data, e2);
    _mvvm_bind(logic.data, e3);

    return e0;
  }
}
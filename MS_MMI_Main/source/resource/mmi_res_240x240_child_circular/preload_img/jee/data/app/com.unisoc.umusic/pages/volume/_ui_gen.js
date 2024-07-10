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
    let e0, e1, e2, e3, e4;

    e0 = new Page();
    e0.create(null, 'undefined');

    e1 = new Div();
    e1.create(e0, 'undefined');

    e2 = new Image();
    e2.create(e1, 'max');

    e3 = new Progressbar();
    e3.create(e1, 'prgbar');

    e4 = new Image();
    e4.create(e1, 'min');

    e0.setAttribute("background-color", "0x0");

    e1.setAttribute("background-color", "0x0000");
    e1.setAttribute("align-items", "center");
    e1.setAttribute("padding", "20");

    e2.setAttribute("id", "max");
    e2.setAttribute("width", "36");
    e2.setAttribute("height", "36");
    e2.setAttribute("src", $r('img:res_aw_music_vol_3.png'));

    e3.setAttribute("id", "prgbar");
    e3.setAttribute("width", "60");
    e3.setAttribute("height", "113");
    e3.setAttribute("step", "11");
    e3.setAttribute("is_slide", "true");
    e3.setAttribute("type", "VERTICAL");
    e3.setAttribute("prg_bg_color", "21130");
    e3.setAttribute("prg_fg_color", "65535");

    e4.setAttribute("id", "min");
    e4.setAttribute("width", "36");
    e4.setAttribute("height", "36");
    e4.setAttribute("src", $r('img:res_aw_music_vol_1.png'));

    e1.addChild(e2);
    e1.addChild(e3);
    e1.addChild(e4);

    e3.addEventListener("change", function(event) {
      logic.prgChange(event);
    }.bind(logic));

    _mvvm_bind(logic.data, e0);
    _mvvm_bind(logic.data, e1);
    _mvvm_bind(logic.data, e2);
    _mvvm_bind(logic.data, e3);
    _mvvm_bind(logic.data, e4);

    return e0;
  }
}
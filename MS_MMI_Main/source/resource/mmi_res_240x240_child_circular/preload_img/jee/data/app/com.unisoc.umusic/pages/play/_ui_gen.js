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
    let e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16;

    e0 = new Page();
    e0.create(null, 'undefined');

    e1 = new Div();
    e1.create(e0, 'undefined');

    e2 = new Label();
    e2.create(e1, 'undefined');

    e3 = new Label();
    e3.create(e1, 'undefined');

    e4 = new Progressbar();
    e4.create(e1, 'pgbar');

    e5 = new Div();
    e5.create(e1, 'undefined');

    e6 = new Image();
    e6.create(e5, 'undefined');

    e7 = new Image();
    e7.create(e5, 'play');

    e8 = new Image();
    e8.create(e5, 'undefined');

    e9 = new Div();
    e9.create(e1, 'undefined');

    e10 = new Image();
    e10.create(e9, 'undefined');

    e11 = new Image();
    e11.create(e9, 'undefined');

    e12 = new Image();
    e12.create(e9, 'undefined');

    e13 = new Div();
    e13.create(e1, 'undefined');

    e14 = new Image();
    e14.create(e13, 'undefined');

    e15 = new Image();
    e15.create(e13, 'undefined');

    e16 = new Audio();
    e16.create(e0, 'audio');

    e0.setAttribute("background-color", "0x0");

    e1.setAttribute("background-color", "0x0000");
    e1.setAttribute("align-items", "center");
    e1.setAttribute("padding", "5");

    e2.setAttribute("value", "{{singer}}");
    e2.setAttribute("class", "text");
    e2.setAttribute("font-size", "16");
    e2.setAttribute("color", "0xffff");
    e2.setAttribute("text-align", "center");

    e3.setAttribute("value", "{{song}}");
    e3.setAttribute("class", "text");
    e3.setAttribute("font-size", "16");
    e3.setAttribute("color", "0xffff");
    e3.setAttribute("text-align", "center");

    e4.setAttribute("id", "pgbar");
    e4.setAttribute("width", "99");
    e4.setAttribute("height", "6");
    e4.setAttribute("step", "0");
    e4.setAttribute("type", "HORIZONTAL");
    e4.setAttribute("prg_bg_color", "0x8410");
    e4.setAttribute("prg_fg_color", "0x8000");
    e4.setAttribute("is_slide", "false");

    e5.setAttribute("flex-direction", "row");
    e5.setAttribute("justify-content", "center");
    e5.setAttribute("padding", "6");

    e6.setAttribute("width", "40");
    e6.setAttribute("height", "40");
    e6.setAttribute("src", $r('img:res_aw_music_prev.png'));

    e7.setAttribute("id", "play");
    e7.setAttribute("width", "70");
    e7.setAttribute("height", "70");
    e7.setAttribute("src", $r('img:res_aw_music_play.png'));

    e8.setAttribute("width", "40");
    e8.setAttribute("height", "40");
    e8.setAttribute("src", $r('img:res_aw_music_next.png'));

    e9.setAttribute("flex-direction", "row");
    e9.setAttribute("justify-content", "center");
    e9.setAttribute("padding", "6");

    e10.setAttribute("height", "36");
    e10.setAttribute("src", $r('img:res_aw_music_fav.png'));

    e11.setAttribute("is_tp_enable", "true");
    e11.setAttribute("height", "36");
    e11.setAttribute("src", $r('img:res_aw_music_speaker.png'));

    e12.setAttribute("height", "36");
    e12.setAttribute("src", $r('img:res_aw_music_download.png'));

    e13.setAttribute("flex-direction", "row");
    e13.setAttribute("justify-content", "center");
    e13.setAttribute("padding", "6");

    e14.setAttribute("width", "9");
    e14.setAttribute("height", "9");
    e14.setAttribute("src", $r('img:res_aw_music_indicator.png'));

    e15.setAttribute("width", "9");
    e15.setAttribute("height", "9");
    e15.setAttribute("src", $r('img:res_aw_music_list_indicator.png'));

    e16.setAttribute("id", "audio");
    e16.setAttribute("volume", "50");
    e16.setAttribute("autoplay", "false");

    e1.addChild(e2);
    e1.addChild(e3);
    e1.addChild(e4);
    e5.addChild(e6);
    e5.addChild(e7);
    e5.addChild(e8);
    e9.addChild(e10);
    e9.addChild(e11);
    e9.addChild(e12);
    e13.addChild(e14);
    e13.addChild(e15);

    e1.addChild(e5);
    e1.addChild(e9);
    e1.addChild(e13);

    e6.addEventListener("click", function(event) {
      logic.preClicked(event);
    }.bind(logic));
    e7.addEventListener("click", function(event) {
      logic.playClicked(event);
    }.bind(logic));
    e8.addEventListener("click", function(event) {
      logic.nextClicked(event);
    }.bind(logic));
    e11.addEventListener("click", function(event) {
      logic.volumeClicked(event);
    }.bind(logic));
    e16.addEventListener("play", function(event) {
      logic.onPlay(event);
    }.bind(logic));
    e16.addEventListener("pause", function(event) {
      logic.onPause(event);
    }.bind(logic));
    e16.addEventListener("ended", function(event) {
      logic.onEnded(event);
    }.bind(logic));
    e16.addEventListener("error", function(event) {
      logic.onError(event);
    }.bind(logic));
    e16.addEventListener("timeupdate", function(event) {
      logic.onTimeUpdate(event);
    }.bind(logic));
    e16.addEventListener("buffer", function(event) {
      logic.onBuffer(event);
    }.bind(logic));
    e16.addEventListener("endbuffer", function(event) {
      logic.onEndBuffer(event);
    }.bind(logic));

    _mvvm_bind(logic.data, e0);
    _mvvm_bind(logic.data, e1);
    _mvvm_bind(logic.data, e2);
    _mvvm_bind(logic.data, e3);
    _mvvm_bind(logic.data, e4);
    _mvvm_bind(logic.data, e5);
    _mvvm_bind(logic.data, e6);
    _mvvm_bind(logic.data, e7);
    _mvvm_bind(logic.data, e8);
    _mvvm_bind(logic.data, e9);
    _mvvm_bind(logic.data, e10);
    _mvvm_bind(logic.data, e11);
    _mvvm_bind(logic.data, e12);
    _mvvm_bind(logic.data, e13);
    _mvvm_bind(logic.data, e14);
    _mvvm_bind(logic.data, e15);
    _mvvm_bind(logic.data, e16);

    return e0;
  }
}
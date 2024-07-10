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
 * @file     component.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/


import { view } from 'view';
import { BaseEvent as IBaseEvent, TouchEvent as ITouchEvent, KeyEvent as IKeyEvent, SwipeEvent as ISwipeEvent, ChangeEvent as IChangeEvent } from "event.js";
import { KeyCode } from 'keycode';
export class Component extends view {
    constructor(x, y, width, height) {
        super();

        if (arguments.length == 4) {
            this._x = x;
            this._y = y;
            this._width = width;
            this._height = height;
        } else {
            this._x = 0;
            this._y = 0;
            this._width = 0;
            this._height = 0;
        }

        this.$class_name = "Component";
        this.dataset = null;
        this._id = "";
        this._style = "";
        this._class = "";
        this._ref = "";
        this._disabled = false;
        this._focused = false;
        this._data = "";

        this._parentHandle = null;
        this._eventHandle = null;
        this._nativeHandle = null;
        this._readonly = true;
        this._rawtext = "undefined";
        Object.defineProperty(this, "databind", {
            enumerable: true,
            get: function () {
                return (this._rawtext.indexOf('{{') != -1);
            }
        })
        Object.defineProperty(this, "readOnly", {
            enumerable: true,
            get: function () {
                return this._readonly;
            },
            set: function (newValue) {
                this._readonly = newValue;
            }
        })
        Object.defineProperty(this, "x", {
            enumerable: true,
            get: function () {
                return this._x;
            },
            set: function (newValue) {
                this._x = any2number(newValue);

                if (this._nativeHandle !== null) {
                    js_component_set_rect(this._nativeHandle, this._x, this._y, this._width, this._height);
                }
            }
        })

        Object.defineProperty(this, "y", {
            enumerable: true,
            get: function () {
                return this._y;
            },
            set: function (newValue) {
                this._y = any2number(newValue);
                if (this._nativeHandle !== null) {
                    js_component_set_rect(this._nativeHandle, this._x, this._y, this._width, this._height);
                }
            }
        })

        Object.defineProperty(this, "width", {
            enumerable: true,
            get: function () {
                return this._width;
            },
            set: function (newValue) {
                this._width = any2number(newValue);
                if (this._nativeHandle !== null) {
                    js_component_set_rect(this._nativeHandle, this._x, this._y, this._width, this._height);
                }
            }
        })

        Object.defineProperty(this, "height", {
            enumerable: true,
            get: function () {
                return this._height;
            },
            set: function (newValue) {

                this._height = any2number(newValue);
                if (this._nativeHandle !== null) {
                    js_component_set_rect(this._nativeHandle, this._x, this._y, this._width, this._height);
                }
            }
        })

        Object.defineProperty(this, "id", {
            enumerable: true,
            get: function () {
                return this._id;
            },
            set: function (newValue) {
                this._id = newValue;
            }
        })

        Object.defineProperty(this, "style", {
            enumerable: true,
            get: function () {
                return this._style;
            },
            set: function (newValue) {
                this._style = newValue;
            }
        })

        Object.defineProperty(this, "class", {
            enumerable: true,
            get: function () {
                return this._class;
            },
            set: function (newValue) {
                this._class = newValue;
            }
        })

        Object.defineProperty(this, "ref", {
            enumerable: true,
            get: function () {
                return this._ref;
            },
            set: function (newValue) {
                this._ref = newValue;
            }
        })

        Object.defineProperty(this, "disabled", {
            enumerable: true,
            get: function () {
                return this._disabled;
            },
            set: function (newValue) {
                if (this._nativeHandle === null) {
                    return;
                }

                var val = this.getBoolean(newValue);
                if (val != null) {
                    this._disabled = val;
                    js_component_set_disable(this._nativeHandle, this._disabled);
                } else {
                    console.log("error disable value: " + newValue);
                }
            }
        })

        Object.defineProperty(this, "focusable", {
            enumerable: true,
            get: function () {
                return this._focused;
            },
            set: function (newValue) {
                if (this._nativeHandle === null) {
                    return;
                }
                var val = this.getBoolean(newValue);
                if (val != null) {
                    this._focused = val;
                    js_component_set_focusable(this._nativeHandle, this._focused);
                } else {
                    console.log("error focusable value: " + newValue);
                }
            }
        })

        Object.defineProperty(this, "data", {
            enumerable: true,
            get: function () {
                return this._data;
            },
            set: function (newValue) {
                this._data = newValue;
            }
        })

    }

    draw() {
        //console.log("component: draw");
    }

    create(handle, id) {
        //console.log("component: create");
        this._id = id;
        if (handle !== null) {
            this._parentHandle = handle._nativeHandle;
            handle.add(this);
        }
    }

    delete() {
        //console.log("component: delete");
    }

    setEventHandle(eventHandle) {
        //console.log("component: setEventHandler");
        this._eventHandle = eventHandle;
    }

    focus() {
        //console.log("component: focus");
        this._focused = true;
        onFocus();
    }

    onTouchStart(TouchEvent) {
        console.log("component: onTouchStart");
        return false;
    }

    onTouchMove(TouchEvent) {
        console.log("component: onTouchMove");
        return false;
    }

    onTouchCancel(TouchEvent) {
        console.log("component: onTouchCancel");
        return false;
    }

    onTouchEnd(TouchEvent) {
        console.log("component: onTouchEnd");
        return false;
    }

    onClick() {
        console.log("component: onClick");
        return false;
    }

    onLongPress() {
        console.log("component: onLongPress");
        return false;
    }

    onFocus() {
        console.log("component: onFocus");
        return false;
    }

    onBlur() {
        console.log("component: onBlur");
        return false;
    }

    onChange() {
        console.log("component: onChange");
        return false;
    }

    onSwipe(event) {
        console.log("component: onSwipe", event);
        if (this.$class_name == "Page" && event.direction == "right") {
            if ($global.router != null) {
                $global.router.navigateBack();
            }

            return true;
        }

        return false;
    }

    onKey(event) {
        console.log("component: onKey", event);
        if (this.$class_name == "Page") {
            if (event.code == KeyCode["KEY_CANCEL"] && event.action == 0) {
                if ($global.router != null) {
                    $global.router.navigateBack();
                }

                return true;
            } else if (event.code == KeyCode["KEY_RED"] && event.action == 0) {
                if ($global.router != null) {
                    $global.router.exit();
                }
                return true;
            }
        }

        return false;
    }

    addEventTarget(event) {
        console.log("component: addEventTarget");

        if (event != null) {
            event.target = new Object();
            event.target.id = this._id;
            event.target.datatset = this.dataset;
        }
    }

    addEventCustom(event) {
        console.log("component: addEventCutom");

        event.detail = new Object();
    }

    addExternalInfo(event) {
        console.log("component: addExternalInfo");

        this.addEventTarget(event);
        this.addEventCustom(event);
    }

    eventLoop(event) {
        var result = false;

        this.addEventTarget(event);

        if (this._eventHandle !== null)
            result = this._eventHandle(event);
        if (result) return true;

        //console.log("Component: eventLoop");

        if (event.type === "touchstart") {
            this.onTouchStart(event);
        } else if (event.type === "touchcancel") {
            this.onTouchCancel(event);
        } else if (event.type === "touchmove") {
            this.onTouchMove(event);
        } else if (event.type === "touchend") {
            this.onTouchEnd(event);
        } else if (event.type === "click") {
            this.addEventCustom(event);
            this.onClick(event);
            //this.onclick(event);
        } else if (event.type === "longpress") {
            this.addEventCustom(event);
            this.onLongPress(event);
        } else if (event.type === "focus") {
            this.onFocus(event);
        } else if (event.type === "blur") {
            this.onBlur(event);
        } else if (event.type === "change") {
            this.addEventCustom(event);
            this.onChange(event);
        } else if (event.type === "swipe") {
            this.addEventCustom(event);
            if (this.onSwipe(event) == false) {
                Component.prototype.onSwipe.apply(this, arguments);
            }
        } else if (event.type === "key") {
            if (this.onKey(event) == false) {
                Component.prototype.onKey.apply(this, arguments);
            }
        } else if (event.type === "show") {
            if (this.onPageShow != null) {
                this.onPageShow();
            }
        } else if (event.type === "hide") {
            if (this.onPageHide != null) {
                this.onPageHide();
            }
        } else {
        }
    }


    update2mode(obj) {
        _mmvm_view2mode(this, obj, this._mmvmPropDefine[this._view2ModeKey]['tag'], this._view2ModeKey);
    }

    update2view(key1, obj) {
        var key = key1;
        if (this._mmvmPropDefine[key] == null) {
            for (const x in this._mmvmPropDefine) {
                if ((this._mmvmPropDefine[x]['tag'] != null) && (this._mmvmPropDefine[x]['tag'].indexOf(key) != -1)) {
                    key = x;
                    break;
                }
            }
            if (key == key1) {
                return;
            }
        }

        if (this._mmvmPropDefine[key]['tag'] != null) {
            this[key] = _mmvm_mode2view(this, obj, this._mmvmPropDefine[key]['tag']);
        }
    }

    setAttribute(attribute, value) {
        if (value.indexOf('{{') == -1) {
            this[attribute] = value;
        } else {
            if (this._mmvmPropDefine[attribute]['tag'] == null) {
                this._mmvmPropDefine[attribute]['tag'] = value;
            }
        }
    }

    addEventListener(eventname, listener) {
        //this['on'+eventname] = listener;

        var e = eventname[0].toUpperCase() + eventname.substring(1);
        this['on' + e] = listener;
    }

    binddatahost(data) {
        this["datahost"] = data;
    }

    BaseEvent() {
        return new IBaseEvent();
    }

    TouchEvent() {
        return new ITouchEvent();
    }

    KeyEvent() {
        return new IKeyEvent();
    }

    SwipeEvent() {
        return new ISwipeEvent();
    }

    ChangeEvent() {
        return new IChangeEvent();
    }
}
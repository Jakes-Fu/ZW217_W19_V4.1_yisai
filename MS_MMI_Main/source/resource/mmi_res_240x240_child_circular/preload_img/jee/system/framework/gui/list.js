
console.log("list.js loading");

import { Component } from 'component';
class IList extends Component {
    constructor(handle, id) {
        if (arguments.length == 2) {
            super.create(handle, id);
            this._nativeHandle = js_gui_list_create(this, this._parentHandle);
        } else {
            super();
        }
        console.log("List: constructor");
        this.TAG = "List:";
        this._bgColor = "";
        this._itemMax = "";
        this._fontSize = "";
        this._fontColor = "";
        this._itemTextScroll = "";
        this._isScroll = "";
        this._mmvmPropDefine = {
            'value': { 'type': [[1, 2]] }
        };
        Object.defineProperty(this, "item-max", {
            enumerable: true,
            get: function () {
                return this._itemMax;
            },
            set: function (newValue) {
                {
                    this._itemMax = any2number(newValue);
                    if (this._nativeHandle !== null) {
                        js_gui_list_setMaxItem(this._nativeHandle, this._itemMax);
                    }
                    else {
                        console.log(this.TAG + "this._nativeHandle === null");
                    }
                }
            }
        })
        Object.defineProperty(this, "bgColor", {
            enumerable: true,
            get: function () {
                return this._bgColor;
            },
            set: function (newValue) {
                if (typeof newValue !== "number") {
                    console.log(this.TAG + "bgColor type error");
                }
                else {
                    this._bgColor = newValue;
                    if (this._nativeHandle !== null) {
                        js_gui_list_set_bg_color(this._nativeHandle, newValue);
                    }
                    else {
                        console.log(this.TAG + "this._nativeHandle === null");
                    }
                }
            }
        })
        Object.defineProperty(this, "fontSize", {
            enumerable: true,
            get: function () {
                return this._fontSize;
            },
            set: function (newValue) {
                this._fontSize = any2number(newValue);
                if ((this._nativeHandle !== null) && (this._fontColor !== "")) {
                    js_gui_list_set_font(this._nativeHandle, this._fontSize, this._fontColor);
                }
                else {
                    console.log(this.TAG + "this._nativeHandle === null");
                }
            }
        })
        Object.defineProperty(this, "fontColor", {
            enumerable: true,
            get: function () {
                return this._fontColor;
            },
            set: function (newValue) {
                this._fontColor = any2number(newValue);
                if ((this._nativeHandle !== null) && (this._fontSize !== "")) {
                    js_gui_list_set_font(this._nativeHandle, this._fontSize, this._fontColor);
                }
                else {
                    console.log(this.TAG + "this._nativeHandle === null");
                }
            }
        })
        Object.defineProperty(this, "itemTextScroll", {
            enumerable: true,
            get: function () {
                return this._itemTextScroll;
            },
            set: function (newValue) {
                if (typeof newValue !== "boolean") {
                    console.log(this.TAG + "itemtextScroll type error");
                    //throw "type error";
                }
                else {
                    this._itemTextScroll = newValue;
                    if (this._nativeHandle !== null) {
                        js_gui_list_set_text_scroll(this._nativeHandle, newValue);
                    }
                    else {
                        console.log(this.TAG + "this._nativeHandle === null");
                    }
                }
            }
        })
        Object.defineProperty(this, "isScroll", {
            enumerable: true,
            get: function () {
                return this._isScroll;
            },
            set: function (newValue) {
                if (typeof newValue !== "boolean") {
                    console.log(this.TAG + "listScroll type error");
                    //throw "type error";
                }
                else {
                    this._isScroll = newValue;
                    if (this._nativeHandle !== null) {
                        js_gui_list_set_scrollbar(this._nativeHandle, newValue);
                    }
                    else {
                        console.log(this.TAG + "this._nativeHandle === null");
                    }
                }
            }
        })

        Object.defineProperty(this, "value", {
            enumerable: true,
            get: function () {
                return this._string;
            },
            set: function (newValue) {
                if (typeof newValue !== "string") {
                    if (Array.isArray(newValue)) {
                        for (var i = 0; i < newValue.length; i++) {
                            this.appendData(newValue[i][0], newValue[i][1], 90, 30, 24);
                        }
                    }
                }
            }
        })
    }

    appendData(text1, text2, item_height, text1_size, text2_size) {
        console.log("List:appendData");
        js_gui_list_append_data(this._nativeHandle, text1, text2);
        console.log("List:js_gui_list_append_data");
        js_gui_list_set_item_height(this._nativeHandle, item_height);
        console.log("List:js_gui_list_set_item_height");
        js_gui_list_set_font_size(this._nativeHandle, text1_size, text2_size);
        console.log("List:js_gui_list_set_font_size");
    }

    getCurIndex() {
        console.log("List:getCurIndex");
        return js_gui_list_get_cur_index(this._nativeHandle);
        console.log("List:js_gui_list_get_cur_index");
    }

    addEventCustom(event) {
        console.log("List: addEventCutom");

        super.addEventCustom(event);
        event.detail.value = this.getCurIndex();
    }

    delete() {
        super.delete();
        js_gui_list_destroy(this._nativeHandle);
    }
    create(handle, id) {
        super.create(handle, id);
        console.log("List: create");
        this._nativeHandle = js_gui_list_create(this, this._parentHandle);
        return this._nativeHandle;
    }
    setItemFontColor(item_index, item_font_color, is_font_color_modify) {
        return js_gui_list_set_item_font_color(this._nativeHandle, item_index, item_font_color, is_font_color_modify);
    }

    onClick() {
        console.log("list: onClick" + getCurIndex());
    }

    delete() {
        super.delete();
        js_gui_text_destroy(this._nativeHandle);
    }

}

export function List(x, y, width, height) {
    if (arguments.length == 4) {
        return new IList(x, y, width, height);
    } else {
        return new IList();
    }
}
console.log("list.js ok");




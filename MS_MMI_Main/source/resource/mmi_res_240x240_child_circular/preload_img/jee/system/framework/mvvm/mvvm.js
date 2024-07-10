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

__mvvm_console = require('console');

function _mvvm_observe(data) {
    if (Object.prototype.toString.call(data) === '[object Object]') {
        for (let prop in data) {
            defineReactive(data, prop, data[prop]);
        }
    } else {
    }
}

class Dep {
    constructor() {
        this.subs = new Set();
    }
    addSub(sub) {
        this.subs.add(sub);
    }
    removeSub(sub) {
        this.subs.delete(sub);
    }
    notify(key, obj) {
        for (let sub of this.subs) {
            if (obj == sub.data) {
                sub.view.update2view(key, sub.data);
            }
        }

    }
}
var _mvvm_dep = new Dep();
function defineReactive(obj, key, val) {
    var ret = Object.defineProperty(obj, key, {
        enumerable: true,
        configurable: true,
        get: function () {
            return val;
        },
        set: function (newVal) {
            if (newVal === val) return;
            val = newVal;
            _mvvm_observe(newVal);
            _mvvm_dep.notify(key, obj);
        }
    });

    _mvvm_observe(val);
}

function textToExp(text) {
    let pieces = text.split(/({{.+?}})/g);

    pieces = pieces.map(piece => {
        if (piece.match(/{{.+?}}/g)) {
            var pv = '(' + piece.replace(/^{{|}}$/g, '') + ')';
            piece = 'Array.isArray(' + pv + ')?' + pv + ':' + '`' + pv + '`';
        } else {
            piece = '`' + piece.replace(/`/g, '\\`') + '`';
        }
        return piece;
    });
    return pieces.join('+');
}


function expToFunc(exp, scope) {
    return new Function('with(this){return ' + exp + '}').bind(scope);
}

function textToExp2(text) {
    let pieces = text.split(/({{.+?}})/g);
    pieces = pieces.map(piece => {
        if (piece.match(/{{.+?}}/g)) {
            piece = '' + piece.replace(/^{{|}}$/g, '') + '=value;';
        } else {
            piece = ''
        }
        return piece;
    });

    return pieces.join('');
}

function _mmvm_mode2view(node, scope, mustache) {
    let exp = textToExp(mustache);
    return expToFunc(exp, scope)();
}

function expToFunc2(exp, scope) {
    return new Function('value', 'with(this){ ' + exp + '}').bind(scope);
}

function _mmvm_view2mode(node, scope, mustache, key) {
    let exp = textToExp2(mustache);
    var value;// = node.value;
    if ('string' == typeof node[key]) {
        value = any2number(node[key]);
    }
    expToFunc2(exp, scope)(value);

}

function _mvvm_datachanged(event) {
    this.update2mode(this["datahost"]);
}


function _mvvm_bind2(data, view, key) {
    //if (view.databind) {
    var sub = new Object();
    sub.data = data;
    sub.view = view;
    _mvvm_dep.addSub(sub);
    view.binddatahost(data);
    _mvvm_dep.notify(key, data);
    if (!view.readOnly) {
        view.onChange = _mvvm_datachanged;
    }

    //    }
}

function _mvvm_bind(data, view) {
    for (const x in view._mmvmPropDefine) {
        _mvvm_bind2(data, view, x);
    }
}
__mvvm_console.log("mvvm.js ok");

/*************************************************************************
SPDX-FileCopyrightText: 2022-2022 Unisoc (Shanghai) Technologies Co., Ltd
SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0

Copyright 2022-2022 Unisoc (Shanghai) Technologies Co., Ltd.
Licensed under the Unisoc General Software License V1;
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
Software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OF ANY KIND, either express or implied.
See the Unisoc General Software License v1 for more details.
**************************************************************************/

const console = require('console');

const __version = '0.1';
const __author = 'xuanchen.jiang@unisoc.com';
console.log('JS TEST FRAMEWORK, version: ' + __version);

const TEST_STATE = Object.freeze({
    PASS: 'PASS',
    FAIL: 'FAIL',
    IGNORE: 'IGNORE',
});


/**
 * run test cases
 * @param  {Object} cases - the case set that will be executed
 * @param  {(string|string[])} [caseName]  --  the case name or array of case name
 *                                                  belong to the case set, only these
 *                                                  cases will be executed
 * @returns {number}  0 means success, otherwise error happens
 * @example
    var cases = {
        setName: 'calc',

        testWillPass: function () {
            assert(true);
        },

        testWillFail: function () {
            assert(false);
        }
    }
    runTest(cases);
    runTest(cases, 'testWillPass');
    runTest(cases, ['testWillPass', 'testWillFail']);
 */
export function runTest(cases, caseName) {
    notifyStart();
    if (!validateCaseSet(cases)) {
        log.error('invalid case set');
        notifyFinish();
        return -1;
    }

    var ret = 0;
    if (caseName) {
        var caseArray;
        if (Array.isArray(caseName)) {
            caseArray = caseName;
        } else if (typeof caseName === 'string') {
            caseArray = [caseName];
        } else {
            log.error('invalid case name');
            notifyFinish();
            return -1;
        }
        ret =  runCaseSet(cases, caseArray);
    } else {
        ret =  runCaseSet(cases);
    }

    log.debug('test finish, ret=' + ret);
    notifyFinish();
    return ret;
}

function notifyStart() {
    console.log('JS Test Start');
}

function notifyFinish() {
    console.log('JS Test finish');
}

function runCaseSet(caseSet, caseArr) {
    if (!validateCaseSet(caseSet)) {
        log.error('caseSet is invalid');
        return -1;
    }
    log.debug('runCaseSet: ' + caseSet.setName);

    //get all methods from case set
    var methods = getMethods(caseSet, caseArr);
    log.debug('methods: ' + JSON.stringify(methods));

    if (caseArr) {
        if (methods.length != caseArr.length) {
            log.debug('need case: ' + caseArr);
            var mArr = methods.map(m => m.name);
            log.debug('found methods: ' + mArr);
            var casesNotFound = caseArr.filter(c => mArr.indexOf(c) == -1);
            log.error('case not found: ' + casesNotFound);
            return -1;
        }
    }

    var total = methods.length;
    var passed = 0;
    var failed = 0;
    var ignored = 0;

    //loop all methods in caseSet and exec
    for (let m of methods) {
        log.debug('run case');
        var result = execCaseFunc(getCaseName(caseSet.setName, m.name), m.func, caseSet);
        printResult(result);
        log.debug('run case end');

        if (config.resultCallback) {
            log.debug('call back');
            config.resultCallback(result);
        }

        if (result.state == TEST_STATE.PASS) {
            passed++;
        } else if (result.state == TEST_STATE.FAIL) {
            failed++;
            if (config.stopWhenFail) {
                log.info('test stop when case failed');
                ignored = total - passed - failed;
                break;
            }
        } else {
            ignored++;
        }
    };

    printStat(total, passed, failed, ignored);
    return 0;
}

//ignore case which name start with '_'
function isIgnore(caseName) {
    var name = caseName;
    if (caseName.includes('.')) {
        name = caseName.split('.')[1];
    }
    return name.startsWith('_');
}

//exec single case, return result object
function execCaseFunc(caseName, caseFunc, that) {
    if (isIgnore(caseName)) {
        log.info('case is ignored: ' + caseName);
        return generateResult(caseName, TEST_STATE.IGNORE, 0);
    }
    log.debug('execCaseFunc, case name: ' + caseName + ', caseFunc: ' + caseFunc);
    var startTime = Date.now();
    try {
        caseFunc.apply(that);
    } catch (e) {
        log.error(e);
        return generateResult(caseName, TEST_STATE.FAIL, Date.now() - startTime, '' + e);
    }
    return generateResult(caseName, TEST_STATE.PASS, Date.now() - startTime);
}

function isFunction(value) {
    return value instanceof Function
}

//get all methods from object. if methodArr is not undefined, only returns the specifed methods
function getMethods(obj, methodArr) {
    log.debug('getMethods: ' + methodArr);
    return Object.keys(obj)
        .filter(key => typeof obj[key] === 'function')
        .filter(key => (methodArr === undefined) || (methodArr.indexOf(key) != -1))
        .map((key) => ({
            name: key,
            func: obj[key]
        }));
}

function getCaseName(moduleName, caseName) {
    if (!moduleName) {
        return caseName;
    }
    return moduleName + '.' + caseName;
}

function printResult(result) {
    if (result.state == TEST_STATE.PASS) {
        console.log(`${result.caseName}: ${TEST_STATE.PASS} (${result.costTime}ms)`);
    } else if (result.state == TEST_STATE.FAIL) {
        console.log(`${result.caseName}: ${TEST_STATE.FAIL} (${result.costTime}ms) [${result.details}]`);
    } else {
        console.log(`${result.caseName}: ${TEST_STATE.IGNORE}`);
    }
}

function generateResult(caseName, state, costTime, details) {
    return {
        caseName: caseName,
        state: state,
        costTime: costTime,
        details: details
    };
}

function printStat(total, passed, failed, ignored) {
    log.debug('test finished');
    console.log(`Total: ${total}, Passed: ${passed}, Failed: ${failed}, Ignored: ${ignored}`);
}

function validateCaseSet(caseSet) {
    if (!isObject(caseSet)) {
        log.error('case set is not a object');
        return false;
    }
    if ((caseSet.setName === undefined) || (typeof caseSet.setName !== 'string')) {
        log.error("case set's name not found");
        return false;
    }

    return true;
}

function isObject(o) {
    //this is a incomplete check but is enough for now
    return (typeof o === 'object') && !Array.isArray(o) && (o != null);
}

const LOG_LEVEL = Object.freeze({
    NONE: 0,
    ERROR: 5,
    WARN: 10,
    INFO: 15,
    DEBUG: 20,
});

const log = {
    debug: function (msg) {
        if (config.logLevel < LOG_LEVEL.DEBUG) return;
        console.log('DEBUG: ' + msg);
    },

    info: function (msg) {
        if (config.logLevel < LOG_LEVEL.INFO) return;
        console.log('INFO: ' + msg);
    },

    warn: function (msg) {
        if (config.logLevel < LOG_LEVEL.WARN) return;
        console.log('WARN: ' + msg);
    },

    error: function (msg) {
        if (config.logLevel < LOG_LEVEL.ERROR) return;
        console.log('ERROR: ' + msg);
    }
}

//Outer can use this config to customize.
export var config = {
    logLevel: LOG_LEVEL.DEBUG, // log level
    stopWhenFail: false,  //stop exec cases when any cases failed
    resultCallback: null   //callback function, invoked to report result
}


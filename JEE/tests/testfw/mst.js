import {runTest, config} from 'test-runner.js'
import mstBasic from './mst-cases-basic.js'
import mstPressure from './mst-cases-pressure.js'
import mstException from './mst-cases-exception.js'
import assert from 'assert.js'

const console = require('console');

//Total: 0, Passed: 0, Failed: 0, Ignored: 0
export function testBasic() {
    runTest(mstBasic);
}

export function testSingle() {
    runTest(mstBasic, 'willSuccess');
    runTest(mstBasic, 'willFail');
    runTest(mstBasic, ['willSuccess','willFail']);
}

export function testPressure() {
    runTest(mstPressure);
}

export function testPressure2() {
    function pass() {
        console.log(this.setName);
        assert(true);
    }
    function fail() {
        assert(false);
    }
    function ignore() {
    }
    var cases = {
        setName: "MST-Pressure"
    };
    var baseName = "test";
    var caseName = "";

    for(let i=0; i<100; i++) {
        caseName = baseName + "pass" + i;
        cases[caseName] = pass;
    }

    for(let i=0; i<100; i++) {
        caseName = baseName + "fail" + i;
        cases[caseName] = fail;
    }

    for(let i=0; i<100; i++) {
        caseName = '_' + baseName + "ignore" + i;
        cases[caseName] = ignore;
    }

    runTest(cases);
}

export function testPressure3() {
    for(var i=0; i<1000; i++) {
        runTest(mstBasic);
    }
}

export function testConfigWithStopWhenFail() {
    config.stopWhenFail = true;
    runTest(mstPressure);
    config.stopWhenFail = false;
}

export function testConfigWithCallback() {
    function handleResult(result) {
        console.log("callback: " + result.state);
    }
    config.resultCallback = handleResult;
    runTest(mstBasic);
    config.resultCallback = null;
}

export function testWithEmptySet() {
    runTest();
    runTest(null);
    runTest({});
    var empty = {
        setName: 'empty',
    }
    runTest(empty);
}

export function testWithInvalidSetType() {
    runTest('str');
    runTest([1,2]);
}

export function testWithEmptyCase() {
    runTest(caseSet, null);
    runTest(caseSet, []);
    runTest(caseSet, '');
}

export function testWithInvalidCaseType() {
    var caseSet = {
        setName: 'testSet',
        testA: function () {}
    }
    runTest(caseSet, 123);
    runTest(caseSet, [1,2]);
    runTest(caseSet, {});
}

export function testInvalideCaseSetName() {
    var oldName = mstBasisc.setName;
    mstBasic.setName = undefined;
    runTest(mstBasic);
    mstBasic.setName = 123;
    runTest(mstBasic);
    mstBasic.setName = '';
    runTest(mstBasic);
    mstBasic.setName = oldName;
}

export function testNonExistCase() {
    runTest(mstBasic, 'nonexistcase');
    runTest(mstBasic, ['willSuccess','nonexistcase']);
    runTest(mstBasic, ['willSuccess','nonexistcase', "non", "willFail"]);
}

export function testAllException() {
    runTest(mstException);
}

export function testLongCaseName() {
    runTest(mstException, 'longfunctionname_abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234');
}

export function testCaseWithUnderscore() {
    runTest(mstException, '__aaaaa');
}

export function testCaseWithProp() {
    runTest(mstException, 'testSetProp');
}

export function testCaseWithAnotherDef() {
    runTest(mstException, 'testOther');
}

export function testCaseWithParam() {
    runTest(mstException, 'testWithParam');
}

export function testThrowError() {
    runTest(mstException, 'testThrowError');
}

export function testCallOuterFunc() {
    runTest(mstException, 'testCallOutFunc');
}

export function testLongTimeExec() {
    runTest(mstException, 'testLongTime');
}


export function test_main() {
    console.log('test_main');
    // config.logLevel = 15;
    testBasic();
    // testSingle();
    // testConfig();
    // testAllException();
    // testPressure();
    // testPressure2();
}


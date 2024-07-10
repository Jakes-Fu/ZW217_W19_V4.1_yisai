import assert from 'assert.js'


export default {
    setName: "MST-Exception",
    prop: "hello",
    useless: "useless",

    longfunctionname_abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234abcdefghijklmnopqrstuvwxyz1234: function () {
        assert(true);
        console.log("long case name");
    },

    s: function () {
        console.log("short case name");
    },

    __aaaaa: function () {
        console.log("double ignore");
    },

    testSetProp: function() {
        this.prop = "world";
        console.log("prop: " + this.prop);
    },

    testOther() {
        console.log("other function define");
    },

    testWithParam(str) {
        console.log("case with param " + str);
    },

    testDivideZero() {
        console.log("divide zero");
        var a = 1/0;
        console.log(a);
    },

    testThrowError() {
        console.log("throw error");
        throw 'test throw error';
    },

    testCallOutFunc() {
        console.log("call pub");
        pubFunc();
    },

    testLongTime() {
        for(let i=0;i<1000000; i++) {
            //donothing
        }
    }
}

function pubFunc() {
    console.log("pubfunc");
}
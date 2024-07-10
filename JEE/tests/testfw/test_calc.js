import assert from 'assert.js'
const calc = require('./calc.js')


export default {
    setName: "calc",

    testAdd: function () {
        var result = calc.add(1, 2);
        assert.equal(result, 3);
    },

    _testSub: function () {
        var result = calc.sub(2, 1);
        assert.equal(result, 1);
    },

    testMul: function () {
        var result = calc.mul(2, 3);
        assert.equal(result, 6);
    },

    testDiv: function() {
        var result = calc.div(5, 2);
        assert.equal(result, 2.5);
    }
}

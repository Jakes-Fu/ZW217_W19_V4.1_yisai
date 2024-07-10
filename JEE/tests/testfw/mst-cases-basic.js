import assert from 'assert.js'

export default {
    setName: "MST-Basic",

    willSuccess: function () {
        assert(true);
    },

    willFail: function () {
        assert(false);
    },

    _willIgnore: function () {
    }
}


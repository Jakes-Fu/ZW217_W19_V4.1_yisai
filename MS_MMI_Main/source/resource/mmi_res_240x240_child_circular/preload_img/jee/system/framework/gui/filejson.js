
fs = require("fs");

function FileJSON() { };

FileJSON.prototype.load = function (path) {
    var ret = fs.statSync(path);
    if (null == ret || 0 == ret.size) {
        console.log("fs statSync " + path + " fail!!!!");
        return '';
    }

    var fd = fs.openSync(path, "r", 0);
    if (fd == null) {
        console.log("open " + path + " fail!!!!!");
        return '';
    }

    var buf = new Buffer(" ".repeat(ret.size));
    fs.readSync(fd, buf, 0, ret.size, null);
    var content = buf.toString();
    fs.closeSync(fd);

    return JSON.parse(content.replace(/[\r\n\s+]/g, ''));
}

FileJSON.prototype.toString = function (obj) {
    return JSON.stringify(obj);
}

var FileJson = new FileJSON();
module.exports = FileJson;
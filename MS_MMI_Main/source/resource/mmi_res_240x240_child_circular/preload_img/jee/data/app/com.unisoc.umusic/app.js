import { runtest } from "./smoketest.js";


App({
    onLaunch: function (options) {
        console.log("umusic: onLaunch");
        console.log('app parameter:' + options.parameter);
        if (options.parameter === '1') {
            runtest();
        }
    },
    onExit: function (options) {
        console.log("umusic: onExit");
    },
    onError: function (options) {
        console.log('umusic: onError');
    },


    songs: [
        { singer: "无名a", name: "短歌1", url: "http://music.163.com/song/media/outer/url?id=4878977.mp3" },
        { singer: "无名b", name: "短歌2", url: "http://music.163.com/song/media/outer/url?id=4975732.mp3" },
        { singer: "无名c", name: "短歌3", url: "http://music.163.com/song/media/outer/url?id=449555.mp3" },
        { singer: "无名d", name: "短歌4", url: "http://music.163.com/song/media/outer/url?id=544543.mp3" },
        { singer: "李健", name: "贝加尔湖畔", url: "http://music.163.com/song/media/outer/url?id=31877245.mp3" },
        { singer: "刘铮", name: "没有人知道我看见彩虹", url: "http://music.163.com/song/media/outer/url?id=117579.mp3" },
        { singer: "周深", name: "亲爱的旅人啊", url: "http://music.163.com/song/media/outer/url?id=1371939273.mp3" },
        { singer: "毛不易", name: "像我这样的人", url: "http://music.163.com/song/media/outer/url?id=569213220.mp3" },
        { singer: "Two Steps From Hell", name: "Victory", url: "http://music.163.com/song/media/outer/url?id=31654455.mp3" },
        { singer: "文文", name: "山歌好比春江水", url: "http://music.163.com/song/media/outer/url?id=1494192743.mp3" },
        { singer: "张学友", name: "旧情绵绵", url: "http://music.163.com/song/media/outer/url?id=189714.mp3" },
        { singer: "毛不易", name: "消愁", url: "http://music.163.com/song/media/outer/url?id=569200213.mp3" },
        { singer: "张学友", name: "遥远的她", url: "http://music.163.com/song/media/outer/url?id=401585374.mp3" },
        { singer: "小娟", name: "红雪莲", url: "http://music.163.com/song/media/outer/url?id=5243605.mp3" },
        { singer: "Keiichi Suzuki", name: "Constructor", url: "http://music.163.com/song/media/outer/url?id=1857433937.mp3" }
    ]


});

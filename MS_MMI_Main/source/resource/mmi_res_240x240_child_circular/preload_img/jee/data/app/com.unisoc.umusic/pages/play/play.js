import { KV } from 'kv';
import { $r } from 'resourceload';
import { Toast } from 'toast';

app = require("@app");

export default {
    data: {
        index: 0,
        song: 'song',
        singer: 'singer'

    },

    onReady: function () {
        this.TAG = "PlayPage : ";
        console.log(this.TAG + "onReady " + this.data.song + this.data.singer + this.data.index);
        this.initPlayer(this.data.index);
    },

    onPlay: function () {
        console.log(this.TAG + " onPlay");
    },
    onPause: function () {
        console.log(this.TAG + " onPause");
    },
    onEnded: function () {
        console.log(this.TAG + " onPlayEnd");

        this.nextClicked();

    },

    onTimeUpdate: function (event) {
        console.log(this.TAG + " onTimeUpdate");
        var progressbar = this.$element("pgbar");
        var player = this.$element("audio");
        var curplaytime = event.cur_time;
        var totaltime = event.total_time;

        if (totaltime != 0) {

            if (progressbar != null) {
                console.log(this.TAG + " set progressbar:" + curplaytime + '/' + totaltime);
                progressbar.percent = (curplaytime) * 100 / (totaltime);

            }
        }
        else {
            console.log(this.TAG + " totalTime is 0");
        }
    },

    onBuffer: function (event) {
        console.log(this.TAG + "begin buffer");
        var playMusicBtn = this.$element("play");
        playMusicBtn.src = $r('img:res_aw_music_play.png');
    },

    onEndBuffer: function (event) {
        console.log(this.TAG + "end buffer");
        var playMusicBtn = this.$element("play");
        playMusicBtn.src = $r('img:res_aw_music_pause.png');
    },

    playClicked: function () {
        var player = this.$element("audio");
        console.log(this.TAG + " play onclick pause status:" + player.paused);
        console.log(this.TAG + " play buffer status:" + player.state);
        var playMusicBtn = this.$element("play");
        if ("buffering" === player.state) {
            playMusicBtn.src = $r('img:res_aw_music_play.png');
        }
        else {
            if (player.paused) {
                console.log(this.TAG + " paused is true!");
                player.resume();
                playMusicBtn.src = $r('img:res_aw_music_pause.png');
            }
            else {
                console.log(this.TAG + "paused is false!");
                player.pause();
                playMusicBtn.src = $r('img:res_aw_music_play.png');
            }
        }
        console.log(this.TAG + " playClicked end!");
    },


    preClicked: function () {
        console.log(this.TAG + " preMusic");
        var playMusicBtn = this.$element("play");
        var progressbar = this.$element("pgbar");

        progressbar.percent = 0;

        playMusicBtn.src = $r('img:res_aw_music_pause.png');
        this.preMusic();
    },


    nextClicked: function () {
        console.log(this.TAG + " nextMusic");

        var playMusicBtn = this.$element("play");
        var progressbar = this.$element("pgbar");
        progressbar.percent = 0;
        playMusicBtn.src = $r('img:res_aw_music_pause.png');
        this.nextMusic();
        console.log(this.TAG + " nextMusic done");
    },


    volumeClicked: function () {
        console.log(this.TAG + " clickVolume");

        router.navigateTo("/pages/volume/volume", null);


    },

    loopMusic: function (direction) {
        console.log(this.TAG + " loopMusic(" + direction + ")");
        this.data.index += direction;
        var songs = getApp().songs;
        var totalLen = songs.length;
        if (this.data.index == -1) {
            this.data.index = totalLen - 1;
        } else if (this.data.index == totalLen) {
            this.data.index = 0;
        }
        var index = this.data.index;


        this.data.song = songs[index].name;
        this.data.singer = songs[index].singer;
        var player = this.$element("audio");
        player.play(songs[index].url);

        console.log(this.TAG + " loopMusic done");
    },

    preMusic: function () {
        this.loopMusic(-1);
    },


    nextMusic: function () {
        this.loopMusic(1);
    },


    initPlayer: function (curIndex) {
        console.log(this.TAG + " initPlayer ");

        var player = this.$element("audio");
        getApp().player = player;

        this.data.singer = getApp().songs[curIndex].singer;
        this.data.song = getApp().songs[curIndex].name;

        this.k = new KV();
        console.log(this.TAG + "new KV");
        var nmpVolume = this.k.getKeyValue("nmpVolume");

        this.data.singer = getApp().songs[curIndex].singer;
        this.data.song = getApp().songs[curIndex].name;

        if (nmpVolume == "") {
            this.k.setKeyValue("nmpVolume", "7");
            nmpVolume = "7";
        }
        console.log(this.TAG + "initPlayer nmpVolume = " + nmpVolume);
        player.volume = nmpVolume * 1;

        if (curIndex >= 0) {
            player.play(getApp().songs[curIndex].url);
        }



        console.log(this.TAG + " updateUI");
        var playMusicBtn = this.$element("play");
        var state = player.state;
        if (state == "pause") {
            playMusicBtn.src = $r('img:res_aw_music_play.png');
        } else if (state == "buffering") {
            playMusicBtn.src = $r('img:res_aw_music_buffer.png');
        } else {
            playMusicBtn.src = $r('img:res_aw_music_pause.png');
        }

        console.log(this.TAG + " updateUI done");
    },


    onError(event) {
        console.log(this.TAG + " audioError errtype:" + event.errtype);

        if (event.errtype == "netError") {
            console.log(this.TAG + " netError state");
            Toast.show(1500, "请检查网络连接");
        }
        else if (event.errtype == "paramError") {
            console.log(this.TAG + " paramError state");
            Toast.show(1500, "参数错误");
        }
        else if (event.errtype == "noMemory") {
            console.log(this.TAG + " noMemory state");
            Toast.show(1500, "内存不足");
            var player = this.$element("audio");
            player.pause();
            console.log(this.TAG + "player paused");
            var playMusicBtn = this.$element("play");
            playMusicBtn.src = $r('img:res_aw_music_play.png');
        }
        else if (event.errtype == "setVolumeFail") {
            console.log(this.TAG + " setVolumeFail state");
            Toast.show(1500, "设置音量失败");
        }
        else if (event.errtype == "setRouteFail") {
            console.log(this.TAG + " setRouteFail state");
            Toast.show(1500, "设置音频通路失败");
        }
        else if (event.errtype == "getProgressFail") {
            console.log(this.TAG + " getProgressFail state");
            Toast.show(1500, "获取进度失败");
        }
        else if (event.errtype == "playFail") {
            console.log(this.TAG + " playFail state");
            Toast.show(1500, "播放失败");
        }
        else if (event.errtype == "createFail") {
            console.log(this.TAG + " createFail state");
            Toast.show(1500, "创建失败");
        }
        else if (event.errtype == "unknownError") {
            console.log(this.TAG + " unknownError state");
            Toast.show(1500, "未知的错误");
        }
        else {
            console.log(this.TAG + " unknownError all");
        }
    }


}


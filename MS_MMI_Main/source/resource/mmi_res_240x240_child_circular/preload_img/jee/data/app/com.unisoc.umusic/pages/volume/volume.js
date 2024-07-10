import { KV } from 'kv';
import { $r } from 'resourceload';

export default {
    data: {

    },

    onInit: function () {
        console.log("volume: onInit");
        this.InitVol();
    },

    onReady: function () {
        console.log("volume: onReady");
        var progbar = this.$element("prgbar");

        console.log('onReady: progressbar.step = %s,  this.nmpVolume = %s', progbar.step, this.nmpVolume);

        getApp().player.volume = parseInt(this.nmpVolume);
        if (null != progbar) {
            console.log('progbar!=null');
            progbar.percent = parseInt(this.nmpVolume);
            console.log('progbar end');
        }

        console.log('onReady: function () end');
    },

    prgChange: function (event) {
        console.log('nmp: prgChange v:' + event.value);

        if (getApp().player != null) {
            getApp().player.volume = event.value;
        }

        console.log('prgChange: this.k.setKeyValue =' + event.value);
        this.k.setKeyValue("nmpVolume", event.value.toString());
        this.nmpVolume = event.value.toString();

        var minImage = this.$element("min");
        if (minImage != null) {
            if (event.value == 0) {
                this.mute_falg = true;
                minImage.src = $r('img:res_aw_music_mute.png');
            }
            if (event.value > 0) {
                this.mute_falg = false;
                minImage.src = $r('img:res_aw_music_vol_1.png');
            }
        }


    },



    InitVol: function () {
        console.log('nmp: InitVol');

        this.k = new KV();
        this.nmpVolume = this.k.getKeyValue("nmpVolume");
        console.log('InitVol this.nmpVolume = ' + this.nmpVolume);

        if (this.nmpVolume == "") {
            this.k.setKeyValue("nmpVolume", "7");
            this.nmpVolume = "7";
        }

        if (this.nmpVolume == 0) {
            this.mute_falg = true;
        }
        if (this.nmpVolume > 0) {
            this.mute_falg = false;
        }

    }
}


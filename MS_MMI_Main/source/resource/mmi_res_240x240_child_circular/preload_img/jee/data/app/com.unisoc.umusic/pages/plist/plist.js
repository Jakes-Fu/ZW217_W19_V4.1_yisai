export default {
    data: {
        songs: []
    },


    onInit: function () {
        console.log("plist: onInit1");
        var songs = getApp().songs;
        for (var i = 0; i < songs.length; i++) {
            var obj = [];
            obj.push(songs[i].singer, songs[i].name);
            this.data.songs.push(obj);
        }
        console.log("plist: onInit2");
    },

    listClicked: function (event) {
        console.log("plist: clicklist called ");

        //push with list index
        var param = new Object();
        param.index = event.detail.value;
        router.navigateTo("/pages/play/play", param);

        return true;
    }
}



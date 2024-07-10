import { add } from "./common/CommonUtil.js";
jee = require("@jee");
export function runtest(arg) {
    //test meminfo api
    console.log('smoke test begin');


    //test console.log for chinese string
    var songs = [{ singer: "胡夏", name: "那些年", url: "http://music.163.com/song/media/outer/url?id=25638810.mp3" },
    { singer: "林俊杰/蔡卓妍", name: "小酒窝", url: "http://music.163.com/song/media/outer/url?id=26305541.mp3" },
    { singer: "苏琛", name: "拾忆", url: "http://music.163.com/song/media/outer/url?id=4872533.mp3" },
    { singer: "华晨宇", name: "国王与乞丐", url: "http://music.163.com/song/media/outer/url?id=32835565.mp3" },
    { singer: "温岚/周杰伦", name: "屋顶", url: "http://music.163.com/song/media/outer/url?id=298317.mp3" }];
    console.log(songs);

    //test import local .js
    add();



    //test console
    console.log("log");
    console.info("info");
    console.warn("warn");
    console.error("error1");

    //test jee
    console.log('dataurl2native:' + jee.getNativePath('datafile://1.txt'));
    console.log('cacheurl2native:' + jee.getNativePath('cachefile://b/2.txt'));
    console.log('localurl2native:' + jee.getNativePath('localfile://3.txt'));
    console.log('getAppInstallDir:' + jee.getAppInstallDir());
    console.log('getSystemDataDir:' + jee.getSystemDataDir());
    console.log('getAppDataDir:' + jee.getAppDataDir());
    console.log('getFWKDir:' + jee.getFwkDir());
    console.log('getPreloadAppDir:' + jee.getPreloadAppDir());

    console.log('getAppLocalCacheDir:' + jee.getAppLocalCacheDir());
    console.log('getAppLocalDataDir:' + jee.getAppLocalDataDir());


    /*$r('');
    console.log($r(''));*/

    //test cyclical struct
    const obj1 = {
        a: 1,
    }
    console.log(obj1);
    obj1.b = obj1;
    //console.log(obj1);




    console.log('smoke test end');

}
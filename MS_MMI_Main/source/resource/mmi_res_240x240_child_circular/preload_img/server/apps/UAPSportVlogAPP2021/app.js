
import { Page } from 'page';
import { Button } from 'button';

app = require("@app");
var pageHome;
var btnHome;

function gotoHomePage() {

    pageHome = new Page();
    pageHome.create(null, "pageHome");
    pageHome.onSwipe = pageOnSwipe;

    btnHome = new Button(0, 80, 240, 105);
    btnHome.create(pageHome, "btnHome");
    btnHome.value = "Enter Sport Vlog APP success";
    btnHome['background-color'] = 0x0000;
    btnHome.color = 0xFFFF;

}

function pageOnSwipe(event) {
    if (event.type == "swipe") {
        console.log("pageEventLoop swipe");
        if (event.direction == "right") {
            pageHome.delete();
            app.exitSync();
        }
    }
}

App
    (
        {
            onLaunch: function onLaunch(options) {
                gotoHomePage();
            },
            onExit: function onExit(options) {
            },
            onError: function onError(msg) {
                console.log(msg);
            }
        }
    );

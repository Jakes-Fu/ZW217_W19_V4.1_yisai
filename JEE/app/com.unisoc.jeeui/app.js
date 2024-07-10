App({
    onLaunch: function (options) {
        console.log(" onLaunch");
        console.log('app parameter:' + options.parameter);
        this.msgString = options.parameter;

    },
    onExit: function (options) {
        console.log(" onExit");
    },
    onError: function (options) {
        console.log(' onError');
    },
    msgString: "Error...."
});
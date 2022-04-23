const express = require('express');
const app = express();
const https = require("https");

const port = 8080;
let connectionpairs = [
    // {
    //     roboip,
    //     uiip,
    //     socketrobo,
    //     socketui,
    // }
];

app.get('/roboregister', function (req, res) {
    // update servers[commandstream,videostream]
    // with robots ip
    registerRobotToVideoStreamServer(req);
    registerRobotToCommandStreamServer(req);
});

app.get('/uiregister', function (req, res) {
    // gets robot ip, updates the streamers
    // with ip,socket pairs between robot and web ui.
    registerUItoVideoStreamServer(req);
    registerUItoCommandStreamServer(req);
});

app.get('/unregister',function(req,res){
    unregisterUItoVideoStreamServer(req);
    unregisterCommandStreamServer(req);
});



app.listen(port);
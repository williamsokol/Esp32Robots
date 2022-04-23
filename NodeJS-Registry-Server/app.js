const express = require('express');
const app = express();
// const http = require('http');
const axios = require('axios');

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




function registerRobotToVideoStreamServer(req) {
    // http.request(`${videoRegistryUrl}/${videoRegisterPath}`)
    axios
        .get(roboVideoRegistryUri)
        .then(res => {
            console.log(`statusCode: ${res.status}`)
            console.log(res)
        })
        .catch(error => {
            // console.error(error)
            unregisterEverything(req);
        })
}
function registerRobotToCommandStreamServer(req){
    axios
        .get(roboCommandRegistryUri)
        .then(res => {
            console.log(`statusCode: ${res.status}`)
            console.log(res)
        })
        .catch(error => {
            // console.error(error)
            unregisterEverything(req);
        })
}
function registerUItoVideoStreamServer(req){
    axios
        .get(uiVideoRegistryUri)
        .then(res => {
            console.log(`statusCode: ${res.status}`)
            console.log(res)
        })
        .catch(error => {
            // console.error(error)
            unregisterEverything(req);
        })
}
function registerUItoCommandStreamServer(req){
    axios
        .get(uiCommandRegistryUri)
        .then(res => {
            console.log(`statusCode: ${res.status}`)
            console.log(res)
        })
        .catch(error => {
            // console.error(error)
            unregisterEverything(req);
        })
}
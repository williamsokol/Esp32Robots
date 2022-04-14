/**
 * 
 * This video server has to clients connected to it
 * one is the robot, and the other is the UI.
 * Robot to Server is inboundStream
 * and
 * Server to Client is outboundStream.
 * All parties must be connected before any video streaming
 * is transferred.
 */

const wsServer = require("ws").WebSocketServer({port: 8080});

let sockets = [];

// let socketObjInterface = {
//     robotIP: '',
//     uiIP,
//     dataflow: ["inbound","outbound","both"],
//     ws,
// };
    
const ip = req.socket.remoteAddress;


wsServerRobot.on('connection', function connection(ws) {
    // const ip = req.socket.remoteAddress;
    ws.on('message', function message(data, isBinary) {

        if (isRobotIP) sendDataToUIIP(ws, data, isBinary);
        else if (isClientIP) registerRobotAndSocketIP(ws, data, isBinary)
        else unhandledRouteError(ws, data, isBinary);

        // client.send(data, { binary: isBinary });

    });

    ws.on('error', function(){
        console.log("error");
    })
});




function registerRobotAndSocketIP(){

}
function sendDataToUIIP(ws, data){
    let socketObj = sockets.find((socketObj)=>{
        return socketObj.robotIP === robotIP;
    });
    let WSclientUI = socketObj.ws; 
    WSclientUI.send(data);
}

function unhandledRouteError(ws, data){
    console.log("Unhandled route");
}
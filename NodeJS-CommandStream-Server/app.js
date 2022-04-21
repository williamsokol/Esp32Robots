/**
 * This is basically the reverse datadlow direction of the VideoStream Server
 */

const wsServer = require("ws").WebSocketServer
const wsServerInstance0 = new wsServer({port: 8080});

let sockets = [];
 
// let socketObjInterface = {
//     robotIP: '',
//     uiIP,
//     dataflow: ["inbound","outbound","both"],
//     wsRobot,
//     wsUI,
// };
     
wsServerInstance0.on('connection', function connection(ws) {
    // const ip = req.socket.remoteAddress;
    ws.on('message', function message(data, isBinary) {

        // let isUIIP = ws._socket.remoteAddress in clientIPs;



        // if (isUIIP) sendDataToRobotIP(ws, data, isBinary);
        // else unhandledRouteError(ws, data, isBinary);

        // // client.send(data, { binary: isBinary });


        sendDataToRobotIP(ws, data, isBinary);

    });

    ws.on('error', function(){
        console.log("error");
    })
});

 


function registerRobotAndSocketIP(){

}
function sendDataToRobotIP(ws, data){
    let socketObj = sockets.find((socketObj)=>{
        return socketObj.UIIP === UIIP;
    });
    let WSrobot = socketObj.wsRobot; 
    WSrobot.send(data);
}

function unhandledRouteError(ws, data){
    console.log("Unhandled route");
}
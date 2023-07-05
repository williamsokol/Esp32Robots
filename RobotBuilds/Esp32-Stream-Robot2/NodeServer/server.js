const crud = require('./CRUD.js');
const path = require('path');
const express = require('express');
const WebSocket = require('ws');
const url = require('url');
const app = express();

app.use(express.static('Pages'));

const WS_PORT  = 8888;
const HTTP_PORT = 8000;

const wsServer = new WebSocket.Server({port: WS_PORT}, ()=> console.log(`WS Server is listening at ${WS_PORT}`));

let connectedClients = [];
const robots = []
async function start() {
    let test = [1,2,3,4,5,6]
    test.splice(2,1)
    // console.log(await crud.readlist());
}start()

wsServer.on('connection', async (ws, req)=>{
    
    console.log('Connected to: ' + req.headers['type']);
    //setUpConnection(ws,req);
    //handleRobotConnection(ws)
    // Object.keys(req.headers).forEach((key) => {
    //     const value = req.headers[key];
    //     console.log(`${key}: ${value}`);
    // }); 
   
        
    if(req.headers['type'] == "robot")
    {
        //handle robot dataIn
        handleRobotConnection(ws,req);
    }else{
        // handle client dataIn
        handleWebConnection(ws,req);
    }
  

   
});

app.get('/client',(req,res)=>res.sendFile(path.resolve(__dirname, './client.html')));
app.get('/LobbyList',(req,res)=>res.sendFile(path.resolve(__dirname, './LobbyList.html')));
app.listen(HTTP_PORT, ()=> console.log(`HTTP server listening at ${HTTP_PORT}`));



async function handleRobotConnection(ws,req)
{
    let lastActivityTime = Date.now();
    const timeoutDuration = 60000;

    ws.id = await crud.smallestUnusedID()
    console.log(ws.id);
    ws.subbedClients = [];
    
    robots.push(ws);   
    crud.AddRobot("test","pass", ws.id);
    console.log("adding robot to database")

    ws.on('message', data => {
        lastActivityTime = Date.now();

        ws.subbedClients.forEach((wsc,i)=>{
        // for (let i=0;i<ws.subbedClients.length;i++){
            // let wsc = ws.subbedClients[i]; 
            if(wsc.readyState === wsc.OPEN){
                wsc.send(data);
            }else{
                ws.subbedClients.splice(i ,1);
                // break;
                
            }
        })
    })

    ws.on('close', data => {
        console.log("robot websocket closed");
        if(req.headers['type'] == "robot")
            crud.deleteRow(req.headers['id']);
    })

    let timeout = setInterval(() => {
        const elapsedTime = Date.now() - lastActivityTime;
        // console.log("test connections ");
        if (elapsedTime >= timeoutDuration) {
          ws.terminate(); // Close the WebSocket connection
          clearInterval(timeout);
        }
    }, 10000);
}
async function handleWebConnection(ws,req)
{
    connectedClients.push(ws);

    let queryParams = url.parse(req.url, true).query;
    console.log('Query parameters:', queryParams);

    let robot = robots.find(r => r.id == queryParams['id']);
    if(robot == null){
        console.error("could not find selected robot")
        ws.terminate();
        return;
    }
    robot.subbedClients.push(ws)

    // console.log(robot)
    ws.on('message', data => {
        if (robot != null){
            // console.log("sending to robot");
            robot.send(data);
        }
    })
}
// console.log(Object.keys(base[i]));

const crud = require('./CRUD.js');
const path = require('path');
const express = require('express');
const WebSocket = require('ws');
const url = require('url');
const dotenv = require('dotenv');
const fs = require('fs');
const IP = require('ip');
const app = express();
dotenv.config();

app.use(express.static('Pages'));

const WS_PORT  = 65080;
const HTTP_PORT = 80;

const wsServer = new WebSocket.Server({port: WS_PORT}, ()=> console.log(`WS Server is listening at ${WS_PORT}`));

let connectedClients = [];
const robots = []
async function start() {
    let test = [1,2,3,4,5,6]
    test.splice(2,1)
    crud.ClearTable();
    // const serverIP = req.socket.localAddress;
    // console.log(app);
}start()

wsServer.on('connection', async (ws, req)=>{
    
    console.log('Connected to: ' + req.headers['type']);
    // console.log(JSON.stringify(req.headers));
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
        console.log(req.headers["mac"])
    }else{
        // handle client dataIn
        handleWebConnection(ws,req);
    }
  

   
});

app.get('/',(req,res)=>res.sendFile(path.resolve(__dirname, './Pages/index.html')));
app.get('/LobbyList',(req,res)=>res.sendFile(path.resolve(__dirname, './LobbyList.html')));
app.get('/client',(req,res)=> {
    const filePath = './Pages/client.js';
    fs.readFile(filePath, 'utf8', (err, fileContent) => {
        if (err) {
        console.error(err);
        res.status(500).send('Internal Server Error');
        } else {
        // Set the appropriate content type
        res.setHeader('Content-Type', 'application/javascript');
        // Send the JavaScript file content as a response
        console.log(fileContent);
        res.send("fileContent");
        }
    });
// res.sendFile(path.resolve(__dirname, './Pages/controls.html'))
});
app.listen(HTTP_PORT, ()=> {
    
    console.log(`HTTP server listening at ${HTTP_PORT}`)
    console.log(`ip of server is ${IP.address()}`)
});



async function handleRobotConnection(ws,req)
{
    let lastActivityTime = Date.now();
    const timeoutDuration = 10000;

    // await crud.smallestUnusedID()
    ws.id = req.headers["mac"];
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
            console.log("Removeing Robot #" + ws.id);
            crud.deleteRow(ws.id);
    })

    let timeout = setInterval(() => {
        const elapsedTime = Date.now() - lastActivityTime;
        // console.log("test connections ");
        if (elapsedTime >= timeoutDuration) {
            crud.deleteRow(req.headers['id']);  
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
// (Object.keys(base[i]));

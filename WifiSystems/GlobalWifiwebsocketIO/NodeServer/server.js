const path = require('path');
const express = require('express');
const WebSocket = require('ws');
const app = express();

const WS_PORT  = 8888;
const HTTP_PORT = 8000;

const wsServer = new WebSocket.Server({port: WS_PORT}, ()=> console.log(`WS Server is listening at ${WS_PORT}`));

let connectedClients = [];
let robot = null
wsServer.on('connection', (ws, req)=>{
    console.log('Connected');
    
    if(req.headers['type'] == "robot")
    {
        robot = ws;   
    }else{
        connectedClients.push(ws);
    }

    ws.on('message', data => {
        
        if(req.headers['type'] == "robot")
        {
            connectedClients.forEach((ws,i)=>{
                if(ws.readyState === ws.OPEN){
                    ws.send(data);
                }else{
                    connectedClients.splice(i ,1);
                }
            })
        }else{
            
            Object.keys(req.headers).forEach((key) => {
                const value = req.headers[key];
                console.log(`${key}: ${value}`);
            }); 
            if (robot != null){
                console.log("sending to robot");
                robot.send(data);
            }
          
        }
    });
});

app.get('/client',(req,res)=>res.sendFile(path.resolve(__dirname, './client.html')));
app.listen(HTTP_PORT, ()=> console.log(`HTTP server listening at ${HTTP_PORT}`));
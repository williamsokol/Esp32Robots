const crud = require('./CRUD.js');
const path = require('path');
const express = require('express');
const WebSocket = require('ws');
const url = require('url');
const fs = require('fs');
const IP = require('ip');
const app = express();
const dotenv = require('dotenv');
dotenv.config();

app.use('/Images', express.static('Images'));
app.use(express.static('Pages'));

const WS_PORT  = process.env.WS_PORT;
const HTTP_PORT = process.env.PORT;

const wsServer = new WebSocket.Server({port: WS_PORT}, ()=> console.log(`WS Server is listening at ${WS_PORT}`));

let connectedClients = [];
var robots = []
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
app.get('/LobbyList', (req, res) => {
    const fs = require('fs');
    let html = fs.readFileSync('./LobbyList.html', 'utf8');
    
    // Inject environment variables
    html = html.replace(
        `const supabaseUrl = 'https://croqanlnwwpwfrajjgos.supabase.co/rest/v1';
            const supabaseKey = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImNyb3Fhbmxud3dwd2ZyYWpqZ29zIiwicm9sZSI6ImFub24iLCJpYXQiOjE2ODU4MjQ1MjQsImV4cCI6MjAwMTQwMDUyNH0.yKni6eV7n4gKqjtC5uQgTLhCusLW3SymCWgub954upc';`,
        `const supabaseUrl = '${process.env.supabaseUrl}/rest/v1';
            const supabaseKey = '${process.env.supabaseKey}';`
    );
    
    res.send(html);
});
app.get('/client', (req, res) => {
    const fs = require('fs');
    let js = fs.readFileSync('./Pages/client.js', 'utf8');
    
    // Inject environment variables
    js = js.replace(
        `const WS_IP = process.env.IP;`,
        `const WS_IP = '${process.env.IP}';`
    );
    js = js.replace(
        `const WS_PORT = process.env.WS_PORT || '65080';`,
        `const WS_PORT = '${process.env.WS_PORT}';`
    );
    
    res.set('Content-Type', 'application/javascript');
    res.send(js);
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
            robots = robots.filter(function (robotws) {
                return robotws.id !== ws.id;
            });
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

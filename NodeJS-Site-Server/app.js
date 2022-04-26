const express = require('express');
const app = express();
const port = 8080;

const demoapphtmlfile = require("fs").readFileSync("./demo.html","utf8");

app.get("/",(req,res)=>{
    res.send(demoapphtmlfile);
});

app.listen(port);
console.log(robotID);

let robotname = document.getElementById("ballybot_name");

robotname.innerText = robotname.innerText + Math.trunc(robotID/1000000000);
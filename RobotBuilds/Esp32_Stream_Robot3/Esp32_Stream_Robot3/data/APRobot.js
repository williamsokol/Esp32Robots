
start();
async function start(){
    // make all the settings work
    var robotSettings = await fetch('/get?confirmedclient', {
        method: 'GET',
    })

    let b = robotSettings.headers.get("robotName");
    console.log(robotSettings)
    console.log(b)

    let name = document.getElementById("ballybot_name");
    name.innerText = b
    
}



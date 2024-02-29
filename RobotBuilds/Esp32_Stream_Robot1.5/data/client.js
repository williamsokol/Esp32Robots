
const WS_IP = '192.168.4.1'
const WS_PORT = '65080'


fetch('/get?confirmedclient', {
  method: 'GET',
})

const params = new URLSearchParams(window.location.search);
const robotID = params.get('id'); // Retrieves the value of the 'name' parameter (John)
let wsinterval;

const img = document.querySelector('img');
const headers = {
    'type': 'Website',
    'id': robotID
};
const searchParams = new URLSearchParams(headers);
const queryString = searchParams.toString();
const WS_URL = `ws:///${WS_IP}:${WS_PORT}?${queryString}`;
const ws = new WebSocket(WS_URL);
let urlObject;

// Events
document.getElementById('lightBtn').addEventListener('click', function() {
    console.log('Button clicked!');
    ws.send('Button clicked!');
});


ws.onopen = () => {
  wsinterval = setInterval(function(){ sendInputData(); }, 50);
};
ws.onclose = () => {
  clearInterval(wsinterval);
};

ws.onmessage = message => {
    const arrayBuffer = message.data;
    if(urlObject){
        URL.revokeObjectURL(urlObject);
    }
    urlObject = URL.createObjectURL(new Blob([arrayBuffer]));
    img.src = urlObject;
}

var oldInputs = null;
function sendInputData()
{
  var inputs;
  let raw = readInputData();
  
  inputs = raw.x+","+raw.y;
  
  if(oldInputs == inputs){
    return;
  }
  oldInputs = inputs
  console.log(inputs);
  ws.send(inputs);
}

function readInputData()
{
  result = new Object;
  result.x = 0;
  result.y = 0;
  
  if(joy.GetDir() != "C"){
    result.x = joy.GetX();
    result.y = joy.GetY();
    return result;
  }

  if(KEYS_PRESSED['ArrowLeft'] || KEYS_PRESSED['a']){
    result.x = -100
    result.y = 100
  }else if(KEYS_PRESSED['ArrowRight']|| KEYS_PRESSED['d']){
    result.x = 100
    result.y = 100
  }
  if(KEYS_PRESSED['ArrowUp']|| KEYS_PRESSED['w']){
    result.y = 100
  }else if(KEYS_PRESSED['ArrowDown']|| KEYS_PRESSED['s']){
    result.y = -100
  }

  return result
}
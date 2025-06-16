#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "BallyBot_AP";  /* Enter SSID here */
const char* password = "12345678";  /* Enter Password here */

/* Put IP Address details */
IPAddress local_ip(192,168,1,1); /*type 192.168.1.1 into url once connected*/
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

void setup() {
    pinMode(14, OUTPUT); /* must set pin as it is on by default */
    pinMode(12, OUTPUT);
    pinMode(2, OUTPUT);
    Serial.begin(115200);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);
    server.on("/", handle_OnConnect);
    server.on("/forward", h_forward);
    server.on("/left", h_left);
    server.on("/right", h_right);
    server.on("/backward", h_backward);
    server.onNotFound(handle_NotFound);
    server.begin();
    Serial.println("HTTP server started");
}
void loop() {
    server.handleClient();
}
void handle_OnConnect() {
    Serial.println("new Client Connected!");
    server.send(200, "text/html", SendHTML());
}
void h_forward(){
    move(HIGH,HIGH,LOW,100);
    move(LOW,LOW,LOW,0);
}
void h_left(){
    move(HIGH,LOW,LOW,100);
    move(LOW,LOW,LOW,0);
}
void h_right(){
    move(LOW,HIGH,LOW,100);
    move(LOW,LOW,LOW,0);
}
void h_backward(){
    move(LOW,LOW,HIGH,100);
    move(LOW,LOW,LOW,0);
}
void handle_NotFound(){
    server.send(404, "text/plain", "Not found");
}
String SendHTML(){
    const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>BallyBot Controller</title>
    <style>
        button { padding: 20px; margin: 10px; }
    </style>
</head>
<body>
    <h1>BallyBot Controller</h1>
    <button onclick="move('forward')">Forward</button><br>
    <button onclick="move('left')">Left</button>
    <button onclick="move('right')">Right</button><br>
    <button onclick="move('backward')">Backward</button><br>
<script>
    function move(direction) {
        fetch('/' + direction); // Send request to /forward, /left, etc.
    }
</script>
</body>
</html>
)rawliteral";
    return index_html;
}
void move(bool l, bool r, bool b, int time){
    digitalWrite(2, l);   /* left motor forward */
    digitalWrite(12, r);  /* right motor forward */
    digitalWrite(14, b);  /* both motors backward */
    delay(time);        /* time till next step */
}
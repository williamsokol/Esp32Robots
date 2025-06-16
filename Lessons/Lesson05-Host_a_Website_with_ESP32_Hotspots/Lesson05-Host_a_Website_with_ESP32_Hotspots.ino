#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "BallyBot_AP";  /* Enter SSID here */
const char* password = "12345678";  /* Enter Password here */

/* Put IP Address settings */
IPAddress local_ip(192,168,1,1); /*type 192.168.1.1 into url once connected*/
IPAddress gateway(192,168,1,1);  /* Wifi detail */
IPAddress subnet(255,255,255,0); /* Wifi detail */

WebServer server(80);

void setup() {
    pinMode(14, OUTPUT); /* must set pin as it is on by default */
    Serial.begin(115200);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);
    server.on("/", handle_OnConnect);
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
void handle_NotFound(){
    server.send(404, "text/plain", "Not found");
}
String SendHTML(){
    const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML>
    <html><head>
        <title>ESP Input Form</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <body>
        Hello World!
    </body></html>
    )rawliteral";

    return index_html;
}
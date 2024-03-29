/*
  Esp32 Websockets Server

  This sketch:
        1. Connects to a WiFi network
        2. Starts a websocket server on port 80
        3. Waits for connections
        4. Once a client connects, it wait for a message from the client
        5. Sends an "echo" message to the client
        6. closes the connection and goes back to step 3

  Hardware:
        For this sketch you only need an ESP32 board.

  Created 15/02/2019
  By Gil Maimon
  https://github.com/gilmaimon/ArduinoWebsockets
*/
#include<vector>
#include "ESPAsyncWebSrv.h"
#include <DNSServer.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include <ESPmDNS.h> 
#include <Preferences.h>


//these 2 libs down are to stop brownout on esp32
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


using namespace std;
using namespace websockets;

String ssid;
String password;
bool is_setup_done = false;
bool client_connected = false;

bool valid_ssid_received = false;
bool valid_password_received = false;
String ssidString;// = String(ssid);
String passwordString;// = String(password);

bool wifi_timeout = false;
// const char* ssid = "William"; //Enter SSID
// const char* password = "12345678"; //Enter Password
DNSServer dnsServer;
WebsocketsServer xserver;
WebsocketsClient xclient;
AsyncWebServer server(80);

Preferences preferences;

int frameCount = 0;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Captive Portal Demo</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h3>Captive Portal Demo</h3>
  <br><br>
  <form action="/get">
    <br>
    SSID: <input type="text" name="ssid">
    <br>
    Password: <input type="text" name="password">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

const char index_html12[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Captive Portal Demo</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h3>Captive Portal Demo</h3>
  <script>
    if (window.matchMedia("(orientation: portrait)").matches) {
       // you're in PORTRAIT mode
      console.log("Going to phonecap");
      document.getElementById("thing").innerHTML = "Now in portrait";
      window.location.href = 'http://192.168.4.1/phoneCap.html';
    }
    else {
      console.log("Going to controls");
      window.location = 'http://192.168.4.1/controls.html';
    }
  </script>
</body></html>)rawliteral";




void callbackfunc(WebsocketsClient& xclient, WebsocketsMessage msg)
{
    char buff[20];
    msg.data().toCharArray(buff, 20);
    Serial.print("callback ");
    Serial.println(buff);

    
    int x,y;
    if (sscanf(buff, "%d,%d", &x, &y) == 2) {
      //Serial.println(x);

      float xf = (float)x/(float)100;
      
      float yf = (float)y/(float)100;
      
      controllMotors(xf,yf);
    }
    else {
      Serial.println(buff);
      //flip light
      static bool lightOn = false;
      lightOn = !lightOn;
      ledcWrite(7,lightOn? 5:0); 
    }
}
void setupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/controls.html");
//    Serial.println("Client Connected");
//    client_connected = true;

  });
  server.on("/connect", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
//    initWifi();
  });
  

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    String inputParam;
    Serial.println("got get request");
    if (request->hasParam("ssid")) {
      inputMessage = request->getParam("ssid")->value();
      inputParam = "ssid";
      ssidString = inputMessage;
      Serial.println(inputMessage);

      valid_ssid_received = true;
    }

    if (request->hasParam("password")) {
      inputMessage = request->getParam("password")->value();
      inputParam = "password";
      passwordString = inputMessage;
      Serial.println(inputMessage);
      valid_password_received = true;
//      WiFiStationSetup(ssidString, passwordString);
    }
    if (request->hasParam("confirmedclient")) {
      client_connected = true;
      Serial.println("Client Connected");
    }
    
    request->send(200, "text/html", "The values entered by you have been successfully sent to the device. It will now attempt WiFi connection");
  });
  
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("controls.html");
}
class CaptiveRequestHandler : public AsyncWebHandler {
  public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request) {
      //request->addInterestingHeader("ANY");
      return true;
    }
    
    void handleRequest(AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/phoneCap.html");
//      request->send_P(200, "text/html", index_html12);
//      request->redirect("/");
//      Serial.println("Client Connected");
//      client_connected = true;

    }
};
void WiFiSoftAPSetup()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP("BallyBot2");
  Serial.print("AP IP address: "); Serial.println(WiFi.softAPIP());
}
void StartCaptivePortal() {
  Serial.println("Setting up AP Mode");
  WiFiSoftAPSetup();
  Serial.println("Setting up Async WebServer");
  setupServer();
  Serial.println("Starting DNS Server");
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP
  server.begin();
  dnsServer.processNextRequest();
}
void StopCaptivePortal(){
//  server.end();
//    xserver
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // prevent brownouts by silencing them
  
  Serial.begin(115200);

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  
  //set up motors
  initMotors();

  //set up camera
  initCamera();

  //set up mpu6050
  //initMPU();
  
  // Connect to wifi
  // WiFi.begin(ssid, password);
  // WiFi.softAP("BallyBots");

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

  StartCaptivePortal();
  if (!MDNS.begin("esp32")) {
      Serial.println("Error setting up MDNS responder!");
      while(1) {
          delay(1000);
      }
  }
  MDNS.addService("http", "tcp", 80);
  
  // start websocket server
  xserver.listen(65080);
  Serial.print("Is server live? ");
  Serial.println(xserver.available());

  // set up led DO NOT SET PIN TO OUT AFTER USING LEDC
  ledcSetup(7, 5000, 8); // set channl 7 
  ledcAttachPin(4, 7);  //pin4 is LED to channel 7

  
  for (int i=0;i<5;i++) 
  {
    ledcWrite(7,10);  // flash led
    delay(200);
    ledcWrite(7,0);
    delay(200);    
  }  
  while( !client_connected ){
    dnsServer.processNextRequest();
    delay(10);   
  }
//  delay(10000); 

}   

void loop() {
  
  //recive data from client
  xclient = xserver.accept();
  xclient.onMessage(&callbackfunc);
  
  Serial.println("testing 2");
  dnsServer.processNextRequest();
 
  // send data to client
  while(xclient.available()) {
    xclient.poll();
    if(frameCount%4 == 0){
        dnsServer.processNextRequest();
        WifiLoop();
        
    }else{
      videoLoop();  
    }

    frameCount += 1;
    //Serial.println("test ");
    delay(20);
  }
  
//  delay(1000);
}

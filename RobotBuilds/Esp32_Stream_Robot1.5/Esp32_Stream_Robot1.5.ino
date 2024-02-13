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

//these 2 libs down are to stop brownout on esp32
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


using namespace std;
using namespace websockets;

String ssid;
String password;
bool is_setup_done = false;
bool client_connected = false;
bool valid_password_received = false;
bool wifi_timeout = false;
// const char* ssid = "William"; //Enter SSID
// const char* password = "12345678"; //Enter Password
DNSServer dnsServer;
WebsocketsServer xserver;
WebsocketsClient xclient;
AsyncWebServer server(80);

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


void callbackfunc(WebsocketsClient& xclient, WebsocketsMessage msg)
{
    int x,y;
    char buff[10];
    msg.data().toCharArray(buff, 10);
//    Serial.print("callback ");
    //Serial.println(buff);

    if (sscanf(buff, "%d,%d", &x, &y) == 2) {
        //Serial.println(x);

        float xf = (float)x/(float)100;
        
        float yf = (float)y/(float)100;;
        
        controllMotors(xf,yf);
    }
}
void setupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    // request->send_P(200, "text/html", index_html);
    Serial.println("Client Connected");
    client_connected = true;

  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    String inputParam;

    if (request->hasParam("ssid")) {
      inputMessage = request->getParam("ssid")->value();
      inputParam = "ssid";
      ssid = inputMessage;
      Serial.println(inputMessage);

      // valid_ssid_received = true;
    }

    if (request->hasParam("password")) {
      inputMessage = request->getParam("password")->value();
      inputParam = "password";
      password = inputMessage;
      Serial.println(inputMessage);
      valid_password_received = true;
    }
    request->send(200, "text/html", "The values entered by you have been successfully sent to the device. It will now attempt WiFi connection");
  });
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
      request->send_P(200, "text/html", index_html);
      Serial.println("Client Connected");
      client_connected = true;

    }
};
void WiFiSoftAPSetup()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP("BallyBots");
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

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // prevent brownouts by silencing them
  
  Serial.begin(115200);



  //set up led
//  ledcSetup(7, 5000, 8);
//  ledcAttachPin(4, 7);  //pin4 is LED

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
  while( !client_connected ){
    dnsServer.processNextRequest();
    delay(10);   
  }
  
  xserver.listen(65080);
  Serial.print("Is server live? ");
  Serial.println(xserver.available());

  // Serial.println(HIGH);
  pinMode(4, OUTPUT);
  
  
  for (int i=0;i<5;i++) 
  {
    ledcWrite(7,10);  // flash led
    delay(200);
    ledcWrite(7,0);
    delay(200);    
  }  
  
}   

void loop() {
  
  //recive data from client
  xclient = xserver.accept();
  xclient.onMessage(&callbackfunc);
  
  Serial.println("testing 2");

 
  // send data to client
  while(xclient.available()) {
    xclient.poll();
    if(frameCount%10 == 0){
      MPULoop();
    }else{
      videoLoop();  
    }

    frameCount += 1;
    //Serial.println("test ");
    delay(20);
  }
  
  delay(1000);
}

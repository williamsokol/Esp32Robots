/*
  Minimal Esp32 Websockets Server

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

#include <ArduinoWebsockets.h>
#include <WiFi.h>
//#include "cameraStuff"

const char* ssid = "falcon1234"; //Enter SSID
const char* password = "Tablelamp!"; //Enter Password


using namespace websockets;

WebsocketsServer xserver;
WebsocketsClient xclient;

//void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
//  // calculate duty, 8191 from 2 ^ 13 - 1
//  uint32_t duty = (8191 / valueMax) * min(value, valueMax);
//
//  // write duty to LEDC
//  ledcWrite(channel, duty);
//}

void callbackfunc(WebsocketsClient& xclient, WebsocketsMessage msg)
{
   
   Serial.println("callback " + msg.data());
   
   //ledcAnalogWrite(0, msg.data().toInt());
   //xclient.send(msg.data());
   
}




void setup() {
  Serial.begin(115200);
  // Connect to wifi

  initCamera();
  
  
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for(int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
      Serial.print(".");
      delay(1000);
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

  xserver.listen(80);
  Serial.print("Is server live? ");
  Serial.println(xserver.available());

  Serial.println(HIGH);
  pinMode(4, OUTPUT);
//
//  ledcSetup(0, 5000, 13);
//  ledcAttachPin(4, 0);
//  
//  ledcAnalogWrite(0, 10);
}

void loop() {
  
  

  xclient = xserver.accept();
  xclient.onMessage(&callbackfunc);
  
  Serial.println("testing 2");
  while(xclient.available()) {
    xclient.poll();   
    videoLoop();
    //Serial.println("test ");
    delay(1);
  }
  
  delay(1000);
}

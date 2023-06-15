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

#include <ArduinoWebsockets.h>
#include <WiFi.h>

//these 2 libs down are to stop brownout on esp32
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

//My created project files:
#include "cameraStuff.h"
#include "motorStuff.h"

//local internet login
const char* ssid = "William"; //Enter SSID
const char* password = "12345678"; //Enter Password
//server ip & port
const char* websocket_server_host = "34.125.16.23";
const uint16_t websocket_server_port = 65080;

int frameCount = 0;

WebsocketsClient xclient;

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


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // prevent brownouts by silencing them
  
  Serial.begin(115200);

//  set up led
 ledcSetup(7, 5000, 8); // set channl 7 
 ledcAttachPin(4, 7);  //pin4 is LED to channel 7

  //set up motors
  initMotors();

  //set up camera
  initCamera();

  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for(int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
      Serial.print(".");
      delay(1000);
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("Local IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

  //xserver.listen(80);
  //Serial.print("Is server live? ");
  //Serial.println(xserver.available());
  xclient.addHeader("type", "robot");
  while(!xclient.connect(websocket_server_host, websocket_server_port, "/")){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Websocket Connected!");

  Serial.println(HIGH);
  
  
  pinMode(4, OUTPUT);
  for (int i=0;i<5;i++) 
  {
    // Serial.println("Test");
    ledcWrite(7,5);  // flash led
    delay(200);
    ledcWrite(7,0);
    delay(200);    
  }  
  
}   

void loop() {
  // Serial.println((uintptr_t)&xclient, HEX);
  videoLoop();

  xclient.onMessage(&callbackfunc);
  xclient.poll();
  //recive data from client
  // xclient = xserver.accept();
  // xclient.onMessage(&callbackfunc);
  // xclient.poll();
  

  // 
 
  // // send data to client
  // while(!xclient.available()) {
    
    

  //   frameCount += 1;
  //   //Serial.println("test ");
  //   delay(20);
  // }
  
  // delay(20);
}


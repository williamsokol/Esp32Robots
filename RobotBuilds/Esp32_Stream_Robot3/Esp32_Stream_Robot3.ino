
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
#include "wifiStuff.h"

Preferences preferences;
//Robot name
const char* RobotName = "Ballyb";
//local internet login
// String ssid = "falcon1234"; //Enter SSID
// String password = "Tablelamp!"; //Enter Password

//server ip & port
const char* websocket_server_host = "34.42.135.118";
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
  WiFi.setSleep(false);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // prevent brownouts by silencing them
  
  Serial.begin(115200);

  // set up led DO NOT SET PIN TO OUT AFTER USING LEDC
  ledcSetup(7, 5000, 8); // set channl 7 
  ledcAttachPin(4, 7);  //pin4 is LED to channel 7

  //set up motors
  initMotors();

  //set up camera
  initCamera();

  //set up wifi
  initWifi();

  Serial.println("done with inits");
  preferences.begin("my-pref", false);
  
  // ssid = preferences.getString("rec_ssid", "Sample_SSID");
  // password = preferences.getString("rec_password", "abcdefgh");


  // // Connect to wifi
  // WiFi.begin(ssid.c_str(), password.c_str());
 
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
  Serial.print("Connecting to server: ");
  Serial.println(websocket_server_host);
  xclient.addHeader("type", "robot");
  while(!xclient.connect(websocket_server_host, websocket_server_port, "/")){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Websocket Connected!");
  

  flashLED(1);
  
  
}   

void loop() {
  static int frames = 0;
  frames += 1;
  if(frames%100 == 0){
    static int oldMillis = 0;
    int time = millis();
    Serial.print("on frame: ");
    Serial.print(frames);
    Serial.print(" at time: ");
    Serial.println((time-oldMillis)/100 );
    oldMillis = time;
  }

  videoLoop();

  xclient.onMessage(&callbackfunc);
  xclient.poll();

}

void flashLED(int mode) {
  for (int i=0;i<3;i++) 
  {
    // Serial.println("Test");
    ledcWrite(7,5);  // flash led
    delay(200);
    ledcWrite(7,0);
    delay(200);    
  }  
}

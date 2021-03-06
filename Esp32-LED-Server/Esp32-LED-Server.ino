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

//these 2 libs down are to stop brownout on esp32
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

const char* ssid = "falcon1234"; //Enter SSID
const char* password = "Tablelamp!"; //Enter Password

using namespace websockets;

WebsocketsServer server;
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // prevent brownouts by silencing them
  
  Serial.begin(115200);
  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for(int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
      Serial.print(".");
      delay(1000);
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP
  }else{
    Serial.println("did not connect to wifi");  
  }
  server.listen(80);
  Serial.print("Is server live? ");
  Serial.println(server.available());

  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  
  // set up pin
  ledcSetup(0, 5000, 13);
  ledcAttachPin(2, 0);

  //default value
  ledcAnalogWrite(0, 255);
  
    
}

void loop() {
  
  
  WebsocketsClient client = server.accept();
  while(client.available()) {
    WebsocketsMessage msg = client.readBlocking();

    // log
    Serial.print("Got Message: ");
    Serial.println(msg.data());

    // return echo
    client.send("Echo: " + msg.data());
    //digitalWrite(4, msg.data().toInt());
    ledcAnalogWrite(0, msg.data().toInt());
    
    
    // close the connection
    // client.close();
  }
  Serial.println("test");
  delay(1000);
}

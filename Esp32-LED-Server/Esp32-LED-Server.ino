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


void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

const char* ssid = "sokol"; //Enter SSID
const char* password = "falcon99"; //Enter Password

using namespace websockets;

WebsocketsServer server;
void setup() {
  Serial.begin(115200);
  // Connect to wifi
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

  server.listen(80);
  Serial.print("Is server live? ");
  Serial.println(server.available());

  pinMode(4, OUTPUT);
  ledcSetup(0, 5000, 13);
  ledcAttachPin(4, 0);
  
  ledcAnalogWrite(0, 10);
  
    
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

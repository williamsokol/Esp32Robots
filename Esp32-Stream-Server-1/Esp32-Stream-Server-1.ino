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

const char* ssid = "sokol"; //Enter SSID
const char* password = "falcon99"; //Enter Password

int mess = 0;

using namespace websockets;

WebsocketsServer xserver;
WebsocketsClient xclient;
WebsocketsMessage msg ;

void callbackfunc(WebsocketsClient& x, WebsocketsMessage cmsg)
{
   mess++;
   Serial.println("callback " + cmsg.data() );
}



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

  xserver.listen(80);
  Serial.print("Is server live? ");
  Serial.println(xserver.available());

  Serial.println(HIGH);
  pinMode(4, OUTPUT);

}

void loop() {
  
  

  xclient = xserver.accept();
  xclient.onMessage(&callbackfunc);
  
  Serial.println("testing 2");
  while(xclient.available()) {
    
    if(xclient.poll()){
       
      //msg = xclient.readBlocking();
      Serial.println("polled" + msg.data() + " " + mess );
      xclient.send("Echo: " + msg.data());
    }
    
  }
  Serial.println("test " + mess);
  delay(1000);
}

#include "wifiStuff.h"
#include <Arduino.h>

DNSServer dnsServer;
AsyncWebServer server(80);


//externs
String ssidString;// = String(ssid);
String passwordString;// = String(password);
bool client_connected;
enum internetStatus OnInternet = None;

bool is_setup_done = false;
bool valid_ssid_received = false;
bool valid_password_received = false;
bool wifi_timeout = false;
char defaultAP [30] = "BallyBot";

String processor(const String& var);

void setupServer() {
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("controls.html");
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    
    request->send(SPIFFS, "/controls.html");
      //request->send(SPIFFS, "/controls.html", String(), false, processor);

    Serial.println("Client Connected");
  });

  server.on("/connect", HTTP_GET, [] (AsyncWebServerRequest * request) {
    
    OnInternet = Connecting;
    Serial.println("trying to connect to internet");
    Serial.print("OnInternet: ");
    Serial.println(OnInternet);
    
    request->send_P(200, "text/html", index_html, processor);
  });
  
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    String inputParam;
    // Serial.print("recived data");
    if (request->hasParam("ssid")) {
      inputMessage = request->getParam("ssid")->value();
      inputParam = "ssidString";
      ssidString = inputMessage;
      Serial.println(inputMessage);
      valid_ssid_received = true;
    }

    if (request->hasParam("password")) {
      inputMessage = request->getParam("password")->value();
      inputParam = "passwordString";
      passwordString = inputMessage;
      Serial.println(inputMessage);
      valid_password_received = true;
    }
    
    if (request->hasParam("confirmedclient")) {
      client_connected = true;
      OnInternet = AP;
      Serial.println("Client confirmed");
      AsyncWebServerResponse *response = request->beginResponse(200);
      response->addHeader("robotName", defaultAP);
      request->send(response);
      return;
    }
    // Serial.print(valid_ssid_received);
    // Serial.println(valid_password_received);
    request->send(200, "text/html", "The values entered by you have been successfully sent to the device. It will now attempt WiFi connection");
  });
  
}

void WiFiSoftAPSetup()
{
  WiFi.mode(WIFI_AP);
  
  
  WiFi.softAP(defaultAP);
  Serial.print("AP IP address: "); Serial.println(WiFi.softAPIP());
  OnInternet = SeekingClient;
}

void WiFiStationSetup(String rec_ssid, String rec_password)
{
  wifi_timeout = false;
  WiFi.mode(WIFI_STA);
  char ssid_arr[20];
  char password_arr[20];
  rec_ssid.toCharArray(ssid_arr, rec_ssid.length() + 1);
  rec_password.toCharArray(password_arr, rec_password.length() + 1);
  Serial.print("Received SSID: "); Serial.println(ssid_arr); Serial.print("And password: "); Serial.println(password_arr);
  WiFi.begin(ssid_arr, password_arr);

  uint32_t t1 = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
    if (millis() - t1 > 10000) //50 seconds elapsed connecting to WiFi
    {
      Serial.println();
      Serial.println("Timeout connecting to WiFi. The SSID and Password seem incorrect.");
      valid_ssid_received = false;
      valid_password_received = false;
      is_setup_done = false;
      preferences.putBool("is_setup_done", is_setup_done);
      preferences.putString("rec_ssid", "failed");

      StartCaptivePortal();
//      WiFiSoftAPSetup();
      wifi_timeout = true;
      break;
    }
  }
  if (!wifi_timeout)
  {
    is_setup_done = true;
    Serial.println("");  Serial.print("WiFi connected to: "); Serial.println(rec_ssid);
    Serial.print("IP address: ");  Serial.println(WiFi.localIP());
    preferences.putBool("is_setup_done", is_setup_done);
    preferences.putString("rec_ssid", rec_ssid);
    preferences.putString("rec_password", rec_password);
    OnInternet = Internet;
  }
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

void initWifi() {
  //your other setup stuff...
  preferences.begin("my-pref", false);

  is_setup_done = preferences.getBool("is_setup_done", false);
  ssidString = preferences.getString("rec_ssid", "Sample_SSID");
  passwordString = preferences.getString("rec_password", "abcdefgh");
  strcat(defaultAP, std::to_string(ESP.getEfuseMac()/1000000000).c_str());
  //  if(ssidString != "failed"){
//    valid_ssid_received = true;
//    valid_password_received = true; 
//  }
  
  StartCaptivePortal();

  xserver.listen(65080);
  Serial.print("Is server live? ");
  Serial.println(xserver.available());

}
void CheckForRouter(){
    Serial.println("Test1");
    if (valid_ssid_received && valid_password_received)
    {
      Serial.println("Attempting WiFi Connection!");
      WiFiStationSetup(ssidString, passwordString);
      if(OnInternet == Internet){ 
        CheckForServer();
      }
    }
     
}
void CheckForServer(){
  WebsocketsClient xclient2;
  for(int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("Local IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

//  xserver.listen(80);
//  Serial.print("Is server live? ");
//  Serial.println(xserver.available());
  Serial.print("Connecting to server: ");
  Serial.println(websocket_server_host);
  xclient2.addHeader("type", "robot");
  xclient2.addHeader("mac", std::to_string(ESP.getEfuseMac()).c_str());
  while(!xclient2.connect(websocket_server_host, websocket_server_port, "/")){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Websocket Connected!");
  xclient = xclient2;
  OnInternet = InServer;
  
}

String processor(const String& var){
  //Serial.println(var);
  if(var == "SSID"){
    return ssidString;
  }
  if(var == "PASS"){
    return passwordString;
  }
  return String();
}

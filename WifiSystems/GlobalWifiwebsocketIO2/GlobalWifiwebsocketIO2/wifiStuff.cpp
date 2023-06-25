#include "wifiStuff.h"
#include <Arduino.h>

DNSServer dnsServer;
AsyncWebServer server(80);

String ssidString;// = String(ssid);
String passwordString;// = String(password);
bool is_setup_done = false;
bool valid_ssid_received = false;
bool valid_password_received = false;
bool wifi_timeout = false;

void setupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
    Serial.println("Client Connected");
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
    // Serial.print(valid_ssid_received);
    // Serial.println(valid_password_received);
    request->send(200, "text/html", "The values entered by you have been successfully sent to the device. It will now attempt WiFi connection");
  });
}

void WiFiSoftAPSetup()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP("esp-captive");
  Serial.print("AP IP address: "); Serial.println(WiFi.softAPIP());
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
    if (millis() - t1 > 50000) //50 seconds elapsed connecting to WiFi
    {
      Serial.println();
      Serial.println("Timeout connecting to WiFi. The SSID and Password seem incorrect.");
      valid_ssid_received = false;
      valid_password_received = false;
      is_setup_done = false;
      preferences.putBool("is_setup_done", is_setup_done);

      StartCaptivePortal();
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

  if (!is_setup_done)
  {
    StartCaptivePortal();
  }
  else
  {
    Serial.println("Using saved SSID and Password to attempt WiFi Connection!");
    Serial.print("Saved SSID is ");Serial.println(ssidString);
    Serial.print("Saved Password is ");Serial.println(passwordString);
    WiFiStationSetup(ssidString, passwordString);
  }

  while (!is_setup_done)
  {
    dnsServer.processNextRequest();
    delay(10);
    if (valid_ssid_received && valid_password_received)
    {
      Serial.println("Attempting WiFi Connection!");
      WiFiStationSetup(ssidString, passwordString);
    }
  }

  Serial.println("All Done!");

  //Your Additional Setup code
}

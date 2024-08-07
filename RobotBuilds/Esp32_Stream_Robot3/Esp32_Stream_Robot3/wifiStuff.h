#pragma once

#include "GlobalVar.h" 
#include <Arduino.h>

extern String ssidString;
extern String passwordString;

extern bool client_connected;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Captive Portal Demo</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h3>Captive Portal Demo</h3>
  <br><br>
  <form action="/get">
    <br>
    SSID: <input type="text" name="ssid" value="%SSID%">
    <br>
    Password: <input type="text" name="password" value="%PASS%">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

class CaptiveRequestHandler : public AsyncWebHandler {
  public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request) {
      //request->addInterestingHeader("ANY");
      return true;
    }

    void handleRequest(AsyncWebServerRequest *request) {
//      request->send_P(200, "text/html", index_html);
      request->send(SPIFFS, "/phoneCap.html");
    }
};

void initWifi();

void setupServer();
void WiFiSoftAPSetup();
void WiFiStationSetup(String rec_ssid, String rec_password);
void StartCaptivePortal();
void CheckForRouter();
void CheckForServer();
// String processor();

#pragma once

#include <Arduino.h>
#include <DNSServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <Preferences.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include "SPIFFS.h"



using namespace std;
using namespace websockets;

//local internet login
extern String ssid; //Enter SSID
extern String password; //Enter Password

extern WebsocketsServer xserver;
extern WebsocketsClient xclient;
extern Preferences preferences;

extern DNSServer dnsServer;


extern const char* websocket_server_host;
extern const uint16_t websocket_server_port;

enum internetStatus { None, AP, Wifi, Connecting, Internet, InServer};
extern enum internetStatus OnInternet;
// };

// using namespace Globals;

#pragma once

#include <Arduino.h>
#include <DNSServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <Preferences.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>

using namespace std;
using namespace websockets;

//local internet login
extern String ssid; //Enter SSID
extern String password; //Enter Password

static WebsocketsServer xserver;
extern WebsocketsClient xclient;
extern Preferences preferences;


extern DNSServer dnsServer;


// };

// using namespace Globals;
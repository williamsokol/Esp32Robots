#include <ArduinoWebsockets.h>
#include <WebServer.h>
#include <WiFi.h>

#include "esp_camera.h"
#include "fb_gfx.h"
#include "human_face_detect_mnp01.hpp" /* for face detection */
#include "human_face_detect_msr01.hpp" /* Add these two lines */

#define CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

/* Add face detection objects */
HumanFaceDetectMSR01 s1(0.1F, 0.5F, 10, 0.2F);
HumanFaceDetectMNP01 s2(0.5F, 0.3F, 5);
#define FACE_COLOR_YELLOW 0x00FF00

/* Settings */
const char *ssid = "BallyBot_AP";
const char *password = "12345678";

/* WebSocket variables */
using namespace websockets;
WebsocketsServer wserver;
WebsocketsClient wclient;
bool ws_client_connected = false;
const uint16_t ws_server_port = 65080;

/* Normal Server */
WebServer server(80);

void setup() {
  pinMode(14, OUTPUT); /* must set pin as it is on by default */
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("hello world");

  CameraSetup();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  wserver.listen(ws_server_port);

  server.on("/", handle_OnConnect);
  server.begin();

  while (!ws_client_connected) {
    server.handleClient();
  }

  /* WebsocketsServer.accept() will delay everything until recieving a
   * connection */
  wclient = wserver.accept();
  Serial.print("wclient: ");
  Serial.println(wclient.available());
  Serial.println("Websocket Connected!");

  Serial.print(WiFi.localIP());
}

void loop() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    esp_camera_fb_return(fb);
    return;
  }

  /* Add face detection */
  std::list<dl::detect::result_t> results =
      s1.infer((uint16_t *)fb->buf, {(int)fb->height, (int)fb->width, 3});
  results = s2.infer((uint16_t *)fb->buf, {(int)fb->height, (int)fb->width, 3},
                     results);

  /* Draw face boxes */
  if (!results.empty()) {
    fb_data_t rfb;
    rfb.width = fb->width;
    rfb.height = fb->height;
    rfb.data = fb->buf;
    rfb.bytes_per_pixel = 2;
    rfb.format = FB_RGB565;

    for (auto &face : results) {
      int x = face.box[0];
      int y = face.box[1];
      int w = face.box[2] - x;
      int h = face.box[3] - y;

      /* Draw rectangle */
      fb_gfx_drawFastHLine(&rfb, x, y, w, FACE_COLOR_YELLOW);
      fb_gfx_drawFastHLine(&rfb, x, y + h, w, FACE_COLOR_YELLOW);
      fb_gfx_drawFastVLine(&rfb, x, y, h, FACE_COLOR_YELLOW);
      fb_gfx_drawFastVLine(&rfb, x + w, y, h, FACE_COLOR_YELLOW);
    }
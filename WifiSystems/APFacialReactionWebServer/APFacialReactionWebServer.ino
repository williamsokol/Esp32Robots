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
  pinMode(4, OUTPUT);  /* LED pin */

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

  /* WebsocketsServer.accept() will delay everything until recieving a connection */
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
  results = s2.infer((uint16_t *)fb->buf, {(int)fb->height, (int)fb->width, 3}, results);

  /* Draw face boxes */
  if (!results.empty()) {
    Serial.println("Seeing face!!!");
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

      int xMid = x + (w / 2);

      fb_gfx_drawFastVLine(&rfb, xMid, y, h, FACE_COLOR_YELLOW);
    }
    digitalWrite(4, HIGH);

  } else {
    digitalWrite(4, LOW);
  }

  /* Convert to JPEG before sending */
  size_t jpg_len = 0;
  uint8_t *jpg_buf = NULL;
  fmt2jpg(fb->buf, fb->len, fb->width, fb->height, PIXFORMAT_RGB565, 80, &jpg_buf, &jpg_len);

  if (jpg_buf && jpg_len > 0) {
    wclient.sendBinary((const char *)jpg_buf, jpg_len);
    free(jpg_buf);
  }

  esp_camera_fb_return(fb);
}
void handle_OnConnect() {
  Serial.println("a client connected to website url");
  server.send(200, "text/html", SendHTML());
  ws_client_connected = true;
}
String SendHTML() {
  const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
  <html><head>
    <title>ESP Input Form</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
  </head>
  <body>
    Hello Client
    <img id="stream"  src="PlaceHolder.jpg">
  <script>
    const WS_IP = '192.168.4.1'
    const WS_PORT = '65080'

    const img = document.getElementById("stream")
    const WS_URL = `ws:///${WS_IP}:${WS_PORT}`;
    const ws = new WebSocket(WS_URL);
    let urlObject;

    ws.onopen = () => {
    };
    ws.onclose = () => {
    };
    ws.onmessage = message => {
        const arrayBuffer = message.data;
        if(urlObject){
            URL.revokeObjectURL(urlObject);
        }
        urlObject = URL.createObjectURL(new Blob([arrayBuffer]));
        img.src = urlObject;
    }
  </script>
  </body></html>
  )rawliteral";

  return index_html;
}
void CameraSetup() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_RGB565;
  /* init with high specs to pre-allocate larger buffers */
  if (psramFound()) {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 40;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  /* camera init */
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

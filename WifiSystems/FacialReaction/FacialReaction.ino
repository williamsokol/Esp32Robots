#include "esp_camera.h"
#include "human_face_detect_mnp01.hpp" 
#include "human_face_detect_msr01.hpp" 

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

#define LED_GPIO_NUM      4 // Assuming LED is connected to GPIO 4

HumanFaceDetectMSR01 s1(0.1F, 0.5F, 10, 0.2F);
HumanFaceDetectMNP01 s2(0.5F, 0.3F, 5);

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  pinMode(LED_GPIO_NUM, OUTPUT);
  digitalWrite(LED_GPIO_NUM, LOW);

  CameraSetup();
}

void loop() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  std::list<dl::detect::result_t> results = 
      s1.infer((uint16_t *)fb->buf, {(int)fb->height, (int)fb->width, 3});
  results = s2.infer((uint16_t *)fb->buf, {(int)fb->height, (int)fb->width, 3}, results);

  if (!results.empty()) {
    Serial.println("Face detected!");
    digitalWrite(LED_GPIO_NUM, HIGH);
    
  } else {
    digitalWrite(LED_GPIO_NUM, LOW);
  }

  esp_camera_fb_return(fb);
  delay(50); 
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
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 40;
  config.fb_count = 2;
  /* camera init */
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}
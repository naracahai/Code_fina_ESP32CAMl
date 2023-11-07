#include <Arduino.h>
#include <WiFi.h>
#include "esp_camera.h"
#include "esp_system.h"


#include <TridentTD_LineNotify.h>
#define SSID        "....."
#define PASSWORD    "......" 
#define LINE_TOKEN  "......." 

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const int Led_Flash = 4;
const int infrared = 13;
const int buzzerPin = 15;
camera_fb_t *fb;

void setup() {

  Serial.begin(115200);
  pinMode(Led_Flash, OUTPUT);
  pinMode(infrared, INPUT);
  pinMode(buzzerPin, OUTPUT);
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while(WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(400); }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());  
  LINE.setToken(LINE_TOKEN);
  
  camera_config_t config;
  bool camera_initialized = false;

  
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
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 2; 
  config.fb_count = 1;
  
 if (!camera_initialized) {
    // กำหนดค่าเริ่มต้นของกล้อง
    esp_err_t err = esp_camera_init(&config);

    if (err != ESP_OK) {
      // จัดการข้อผิดพลาดในการเริ่มต้นกล้อง
      Serial.println("การเริ่มต้นกล้องล้มเหลว");
    } else {
      // กล้องเริ่มต้นเรียบร้อย
      Serial.println("กล้องเริ่มต้นเรียบร้อย");
      camera_initialized = true;

    
    }
  }

  //Camera_capture();
  
}
void loop() {
  static int previousInfraredState = HIGH;
  int currentInfraredState = digitalRead(infrared);

  if (currentInfraredState == LOW && previousInfraredState == HIGH) {
     delay(100);
     int secondReading = digitalRead(infrared);
    
    if (secondReading == currentInfraredState) {
    tone(buzzerPin, 2000, 1000); 
    noTone(buzzerPin); 
    for (int i = 0; i < 3; i++) {
    Camera_capture();
    }
    Serial.println("Infrared LOW");
    }
  }
  
  previousInfraredState = currentInfraredState;
  delay(1000);
}

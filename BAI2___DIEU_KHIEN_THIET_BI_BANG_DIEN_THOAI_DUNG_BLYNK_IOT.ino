#define BLYNK_TEMPLATE_ID "TMPL6RbkJiJh_"
#define BLYNK_TEMPLATE_NAME "Congtachoca"
#define BLYNK_DEVICE_NAME "Congtachoca"

// QUAN TRỌNG: Mỗi lần nạp code online (OTA), hãy tăng số này lên (VD: "0.1.1", "0.1.2"...)
#define BLYNK_FIRMWARE_VERSION        "0.1.1"

#define BLYNK_PRINT Serial
#define APP_DEBUG
#define USE_NODE_MCU_BOARD

#include "BlynkEdgent.h"

WidgetLED LedConnect(V0);
unsigned long timeUpdate = 0;
unsigned long lastConnectedTime = 0;

// Thời gian tối đa chờ mạng trước khi tự khởi động lại mạch (3 phút = 180.000 ms)
const unsigned long RECONNECT_TIMEOUT = 180000; 

void setup()
{
  Serial.begin(115200);
  delay(100);

  // Khai báo chân điều khiển TRƯỚC hoặc SAU BlynkEdgent.begin() sau khi đã sửa Settings.h
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);

  BlynkEdgent.begin();
  lastConnectedTime = millis();
}

void loop() {
  BlynkEdgent.run();

  // 1. Chỉ gửi dữ liệu nháy đèn V0 khi ĐÃ KẾT NỐI Blynk để tránh treo bộ đệm khi mất mạng
  if (Blynk.connected()) {
    lastConnectedTime = millis(); // Cập nhật mốc thời gian đang có mạng

    if (millis() - timeUpdate > 1000) {
      if (LedConnect.getValue()) {
        LedConnect.off();
      } else {
        LedConnect.on();
      }
      timeUpdate = millis();
    }
  } 
  // 2. Nếu mất mạng/cục WiFi khởi động chậm quá 3 phút -> Tự Reset lại ESP8266 để bắt lại WiFi
  else {
    // Chỉ tự Reset khi mạch đã từng được cấu hình WiFi (tránh Reset khi đang cài WiFi lần đầu)
    if (BlynkState::get() != MODE_WAIT_CONFIG && BlynkState::get() != MODE_CONFIGURING) {
      if (millis() - lastConnectedTime > RECONNECT_TIMEOUT) {
        Serial.println("Mat ket noi qua lau, dang khoi dong lai mach de ket noi lai WiFi...");
        delay(500);
        ESP.restart();
      }
    } else {
      lastConnectedTime = millis(); // Đang ở chế độ cài đặt WiFi thì không tự Reset
    }
  }
}

BLYNK_CONNECTED() {
  Blynk.syncAll();
}

BLYNK_WRITE(V1) {
  int p = param.asInt();
  digitalWrite(D1, p);
}

BLYNK_WRITE(V2) {
  int p = param.asInt();
  digitalWrite(D2, p);
}

BLYNK_WRITE(V3) {
  int p = param.asInt();
  digitalWrite(D5, p);
}

BLYNK_WRITE(V4) {
  int p = param.asInt();
  digitalWrite(D6, p);
}
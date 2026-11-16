# 🐟 Hệ Thống Điều Khiển Thiết Bị Hồ Cá Qua Điện Thoại (Blynk IoT - ESP8266)

Dự án điều khiển 4 thiết bị (Relay/Công tắc) từ xa qua ứng dụng **Blynk IoT** sử dụng vi điều khiển **NodeMCU ESP8266** và bộ khung **Blynk Edgent**.

---

## ✨ Tính năng nổi bật

* 📱 **Điều khiển 4 kênh độc lập** từ xa qua điện thoại (Blynk App) và giao diện Web (Blynk Cloud).
* 📶 **Cấu hình WiFi thông minh (WiFi Provisioning):** Tự phát WiFi để cài đặt mạng ngay trên điện thoại, không cần sửa tên WiFi/mật khẩu cứng trong code.
* 🔄 **Tự động kết nối lại khi mất điện / rớt mạng:** Tích hợp bộ đếm Watchdog tự động khởi động lại mạch nếu mất kết nối quá 3 phút (khắc phục lỗi cục phát WiFi khởi động chậm hơn mạch sau khi mất điện) và tự đồng bộ lại trạng thái công tắc (`Blynk.syncAll()`).
* ☁️ **Cập nhật code từ xa (OTA - Blynk.Air):** Nạp code mới trực tiếp qua mạng Internet mà không cần tháo mạch hay cắm cáp USB.
* 💡 **Đèn báo trạng thái trực quan:** Widget LED (`V0`) nhấp nháy mỗi giây trên ứng dụng để kiểm tra mạch đang hoạt động (Online).

---

## 🔌 Sơ đồ đấu nối phần cứng (Pinout)

### 1. Chân điều khiển thiết bị (Relay / Công tắc)

| Thiết bị | Chân trên NodeMCU | Chân GPIO (ESP8266) | Chân Virtual (Blynk) | Kiểu dữ liệu |
| :--- | :---: | :---: | :---: | :---: |
| **Kênh 1** | `D1` | `GPIO 5` | `V1` | Integer (0 / 1) |
| **Kênh 2** | `D2` | `GPIO 4` | `V2` | Integer (0 / 1) |
| **Kênh 3** | `D5` | `GPIO 14` | `V3` | Integer (0 / 1) |
| **Kênh 4** | `D6` | `GPIO 12` | `V4` | Integer (0 / 1) |
| **LED Trạng thái** | — | — | `V0` | Widget LED |

### 2. Chân hệ thống Blynk Edgent (Cấu hình trong `Settings.h`)

> ⚠️ **Lưu ý quan trọng:** Để tránh xung đột với chân `D1`, `D2` đang dùng cho Relay, phần khai báo `#if defined(USE_NODE_MCU_BOARD)` trong file `Settings.h` phải được chỉnh về nút nhấn `FLASH` và đèn `LED` tích hợp sẵn trên mạch:

```cpp
#if defined(USE_NODE_MCU_BOARD)

  #define BOARD_BUTTON_PIN            0     // GPIO0 (Nút FLASH sẵn trên mạch)
  #define BOARD_BUTTON_ACTIVE_LOW     true

  #define BOARD_LED_PIN               2     // GPIO2 (Đèn LED xanh trên module ESP)
  #define BOARD_LED_INVERSE           true
  #define BOARD_LED_BRIGHTNESS        255

#endif
```

---

## 🛠️ Cài đặt môi trường & Nạp code lần đầu (Qua cáp USB)

### 1. Yêu cầu cài đặt
* **Phần mềm:** [Arduino IDE](https://www.arduino.cc/en/software)
* **Board Manager:** Cài đặt gói **ESP8266 Community** trong Arduino IDE.
* **Thư viện (Library):** Thư viện **Blynk** (phiên bản **1.1.0** để tương thích hoàn toàn với các tab phụ của `BlynkEdgent`).
* **Driver USB (CH340):** Nếu máy tính chưa nhận cổng COM (báo chấm than vàng `USB Serial` trong Device Manager), tải và cài đặt [Driver CH341SER.EXE tại đây](https://www.wch-ic.com/downloads/CH341SER_EXE.html).

### 2. Thông số thiết lập khi nạp trong Arduino IDE
* **Board:** `NodeMCU 1.0 (ESP-12E Module)`
* **Upload Speed:** `115200`
* **Port:** Chọn đúng cổng `COMx` của chip `USB-SERIAL CH340` (Không chọn cổng COM ảo của Bluetooth).

---

## 📱 Hướng dẫn sử dụng & Kết nối WiFi

### 1. Kết nối mạch với WiFi lần đầu
1. Cấp nguồn cho mạch NodeMCU. Đèn LED xanh trên mạch sẽ **nhấp nháy nhanh liên tục** (chế độ `WAIT_CONFIG`).
2. Mở ứng dụng **Blynk IoT** trên điện thoại (bật sẵn WiFi và Vị trí/GPS).
3. Nhấn **+ Add New Device** -> **Find Devices Nearby**.
4. Kết nối vào mạng WiFi do mạch phát ra có tên: **`Blynk Congtachoca-XXXX`**.
5. Chọn mạng WiFi gia đình (**Lưu ý:** ESP8266 chỉ hỗ trợ mạng **WiFi 2.4GHz**, không hỗ trợ 5GHz), nhập mật khẩu và nhấn **Continue**.
6. Sau khi kết nối thành công, đèn `V0` trên app sẽ nhấp nháy đều mỗi giây 1 lần.

### 2. Đổi mạng WiFi mới (Reset cấu hình WiFi)
* Khi đổi mật khẩu WiFi hoặc di chuyển mạch sang vị trí mới: Nhấn và giữ nút **FLASH** trên mạch NodeMCU trong **10 giây** cho đến khi đèn LED trên mạch nhấp nháy nhanh trở lại, sau đó cấu hình lại trên ứng dụng Blynk IoT.

---

## 🚀 Hướng dẫn cập nhật code từ xa (OTA qua Blynk.Air)

Không cần tháo mạch hay cắm cáp USB, thực hiện 3 bước sau để nạp code qua mạng:

1. **Tăng phiên bản Firmware trong code:**
   Mở file `.ino` chính, tìm dòng `#define BLYNK_FIRMWARE_VERSION` và nâng số phiên bản lên cao hơn bản hiện tại:
   ```cpp
   #define BLYNK_FIRMWARE_VERSION "0.1.2" // Tăng lên mỗi lần cập nhật (VD: 0.1.1 -> 0.1.2)
   ```
2. **Xuất file Binary (`.bin`):**
   Trên Arduino IDE, chọn menu **Sketch** -> **Export Compiled Binary** (hoặc nhấn `Ctrl + Alt + S`). File `.ino.bin` sẽ được tạo ra trong thư mục chứa code (hoặc thư mục con `build`).
3. **Đẩy code lên mạch qua Blynk Cloud:**
   * Đăng nhập vào [Blynk.Cloud](https://blynk.cloud/) trên máy tính.
   * Chọn mục **Blynk.Air** (biểu tượng máy bay giấy ở thanh menu bên trái) -> Nhấn **+ New Shipping**.
   * Tải file **`.bin`** vừa xuất lên, chọn thiết bị **`Congtachoca`** và nhấn **Start Shipping**.
   * Đợi khoảng 30–60 giây để mạch tự tải bản cập nhật và khởi động lại.

---

## 🔧 Xử lý sự cố thường gặp (Troubleshooting)

| Sự cố / Lỗi | Nguyên nhân | Cách khắc phục |
| :--- | :--- | :--- |
| **`Please specify your BLYNK_TEMPLATE_ID and BLYNK_TEMPLATE_NAME`** | Thư viện Blynk mới đổi tên biến từ `BLYNK_DEVICE_NAME` sang `BLYNK_TEMPLATE_NAME`. | Khai báo cả 2 dòng `#define BLYNK_TEMPLATE_NAME` và `#define BLYNK_DEVICE_NAME` ở đầu chương trình. |
| **`'class BlynkConsole' has no member named 'init'`** | Xung đột phiên bản thư viện Blynk mới (1.2.x/1.3.x) với file `Console.h` cũ. | Vào **Library Manager** hạ cấp thư viện **Blynk** về bản **1.1.0**, hoặc đổi `.init()` thành `.begin()` trong `Console.h`. |
| **`A fatal esptool.py error occurred: Write timeout`** | Chọn nhầm cổng COM của Bluetooth, thiếu Driver CH340 hoặc tốc độ nạp quá cao. | Cài driver CH340, chọn đúng cổng COM của mạch và hạ `Upload Speed` về `115200`. |
| **Mất điện xong có lại mạch nháy đèn liên tục, mất WiFi** | Chân `D1` (GPIO5) trùng với nút Reset WiFi mặc định trong `Settings.h`. | Đổi `BOARD_BUTTON_PIN` sang `0` và `BOARD_LED_PIN` sang `2` trong tab `Settings.h`. |

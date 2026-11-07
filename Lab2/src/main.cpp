// Thu vien cung cap cac ham Arduino.
#include <Arduino.h>

// Khai bao chan GPIO dieu khien LED.
constexpr uint8_t LED_PIN = GPIO_NUM_48;
// Dat thoi gian nhap nhay la 1000 ms.
constexpr uint32_t BLINK_INTERVAL_MS = 1000;

// Ham in trang thai LED ra Serial Monitor.
void printLedStatus(bool ledOn) {
  // In chu "LED:".
  Serial.print("LED: ");
  // In trang thai ON hoac OFF va xuong dong.
  Serial.println(ledOn ? "ON" : "OFF");
}

// Ham chay mot lan khi khoi dong.
void setup() {
  // Khoi tao giao tiep Serial voi toc do 115200.
  Serial.begin(115200);
  // Cau hinh chan LED la chan dau ra.
  pinMode(LED_PIN, OUTPUT);
  // Bat dau voi LED tat.
  digitalWrite(LED_PIN, LOW);
  // Cho 1 giay truoc khi bat dau nhap nhay.
  delay(1000);
  // Thong bao chuong trinh bat dau chay.
  Serial.println("LED blink started");
  // In trang thai LED OFF ban dau.
  printLedStatus(false);
}

// Ham lap lai lien tuc sau setup.
void loop() {
  // Luu thoi diem LED thay doi lan cuoi.
  static uint32_t lastChange = 0;
  // Luu trang thai hien tai cua LED.
  static bool ledOn = false;

  // Lay thoi gian da chay cua bo.
  const uint32_t now = millis();
  // Kiem tra da du 1 giay chua.
  if (now - lastChange >= BLINK_INTERVAL_MS) {
    // Cap nhat thoi diem thay doi.
    lastChange = now;
    // Dao trang thai LED ON sang OFF hoac nguoc lai.
    ledOn = !ledOn;
    // Xuat muc logic ON/OFF ra chan LED.
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    // In trang thai LED moi ra Serial Monitor.
    printLedStatus(ledOn);
  }
}
//
// FaBo Brick Example
//
// FaBo RTC PCF2129
//

#include <Wire.h>
#include <FaBoRTC_PCF2129.h>

FaBoRTC_PCF2129 faboRtc;

void setup() {
  Serial.begin(9600); // シリアルの開始
  Serial.println();
  Serial.println("RESET");

  // デバイス初期化
  Serial.println("Checking I2C device...");
  if (faboRtc.searchDevice()) {
    Serial.println("configuring RTC PCF2129");
    faboRtc.configure();
  } else {
    Serial.println("device not found");
    while(1);
  }

  // 日付時刻の設定
  Serial.println("set date/time");
  faboRtc.setDate(2016,2,8,17,51,50);

}

void loop() {
  // 日付時刻の取得
  DateTime now = faboRtc.now();

  // 日付時刻の表示
  Serial.print("Time: ");
  Serial.print(now.year());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.day());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.print(now.second());
  Serial.println();

  delay(1000);
}

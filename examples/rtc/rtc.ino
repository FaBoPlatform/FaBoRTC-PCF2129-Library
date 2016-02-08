//
// FaBo Brick Example
//
// FaBo RTC PCF2129
//

#include <Wire.h>
#include <FaBoRTC_PCF2129.h>

FaBoRTC_PCF2129 fabo_rtc;

void setup() {
  Serial.begin(9600); // シリアルの開始
  Serial.println();
  Serial.println("RESET");

  // デバイス初期化
  Serial.println("Checking I2C device...");
  if (fabo_rtc.searchDevice()) {
    Serial.println("configuring RTC PCF2129");
    fabo_rtc.configure();
  } else {
    Serial.println("device not found");
    while(1);
  }

  // 日付時刻の設定
  Serial.println("set date/time");
  fabo_rtc.setDate(2016,2,8,17,51,50);

}

void loop() {
  // 日付時刻の取得
  DateTime now = fabo_rtc.now();

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

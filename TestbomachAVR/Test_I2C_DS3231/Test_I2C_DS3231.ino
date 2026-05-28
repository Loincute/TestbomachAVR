#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup()
{
    Serial.begin(9600);

    Wire.begin();

    rtc.begin();
}

void loop()
{
    DateTime now = rtc.now();

    Serial.print(now.hour());
    Serial.print(":");

    Serial.print(now.minute());
    Serial.print(":");

    Serial.println(now.second());

    delay(1000);
}
#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20 nối vào chân PD6
#define ONE_WIRE_BUS PIN_PD6   // Nếu lỗi, xem phần ghi chú bên dưới

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("Test DS18B20 on PD6");

  sensors.begin();

  int deviceCount = sensors.getDeviceCount();
  Serial.print("So cam bien tim thay: ");
  Serial.println(deviceCount);

  if (deviceCount == 0) {
    Serial.println("Khong tim thay DS18B20. Kiem tra chan PD6, dien tro 4.7k, day DQ.");
  }
}

void loop() {
  sensors.requestTemperatures();

  float tempC = sensors.getTempCByIndex(0);

  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Loi: Khong doc duoc DS18B20");
  } else {
    Serial.print("Nhiet do: ");
    Serial.print(tempC);
    Serial.println(" *C");
  }

  delay(1000);
}

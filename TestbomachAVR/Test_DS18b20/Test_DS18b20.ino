#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20 noi vao PB0
#define ONE_WIRE_BUS PB0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{
    Serial.begin(9600);

    Serial.println("DS18B20 Test");

    sensors.begin();

    Serial.print("So cam bien: ");
    Serial.println(sensors.getDeviceCount());
}

void loop()
{
    sensors.requestTemperatures();

    float tempC = sensors.getTempCByIndex(0);

    Serial.print("Nhiet do: ");
    Serial.print(tempC);
    Serial.println(" C");

    delay(1000);
}
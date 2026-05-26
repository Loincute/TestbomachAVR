#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(6); // PD6
DallasTemperature ds(&oneWire);

void setup()
{
    Serial.begin(9600);
    ds.begin();
}

void loop()
{
    ds.requestTemperatures();

    Serial.print("Temp = ");
    Serial.print(ds.getTempCByIndex(0));
    Serial.println(" C");

    delay(1000);
}
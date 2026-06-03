void setup() {
    Serial.begin(9600);
    Serial.println("ATmega32 UART Ready!");
}

void loop() {
    // Gui du lieu len may tinh moi 1 giay
    Serial.println("Hello from ATmega32!");
    delay(1000);

    // Nhan lenh tu may tinh
    if(Serial.available()) {
        char c = Serial.read();
        Serial.print("Nhan duoc: ");
        Serial.println(c);
    }
}
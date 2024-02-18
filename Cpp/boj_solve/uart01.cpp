// #include <HardwareSerial.h>
#include <Wire.h>
// #include <SoftwareSerial.h>
// const byte rx = 0;
// const byte tx = 1;
// SoftwareSerial mySerial (rx, tx);
// HardwareSerial mySeria(rx, tx);
#define SEALEVELPRESSURE_HPA (1013.25)
// HardwareSerial& dwin = Serial1; // Serial1을 사용하도록 수정
unsigned char Buffer[9];

int light = 7;

void setup() {
    Serial.begin(115200);
    // mySerial.begin(115200);
    // dwin.begin(115200, SERIAL_8N1, rx, tx);
    pinMode(light, OUTPUT);
    digitalWrite(light, LOW);
}

void led_control() {
    for (int i = 0; i <= 8; i++) {
        Serial.print(Buffer[i]);
        Serial.print(" ");
    }
    Serial.println("");
    if (Serial.available()) {
        Serial.println("1");
        for (int i = 0; i <= 8; i++) Buffer[i] = Serial.read();
        Serial.println(Buffer[4]);
        if (Buffer[0] == 0X5A) {
            switch (Buffer[4]) {
            case 0x65:
                if (Buffer[8] == 1) {
                    digitalWrite(light, HIGH);
                    Serial.println("ON");
                }
                else {
                    digitalWrite(light, LOW);
                    Serial.println("OFF");
                }
                break;
            }

            // default:
            //   Serial.println("No data...");
        }
    }
    delay(10);
}

void loop() {
    // put your main code here, to run repeatedly:
    led_control();
    delay(500);
}

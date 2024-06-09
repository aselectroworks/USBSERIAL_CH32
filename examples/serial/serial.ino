#include "USBSerial_CH32.h"

void setup() {
    Serial.begin(115200);
    Serial.printf("TEST\r\n");
    Serial.printf("SystemClk:%c\r\n", SystemCoreClock);
    // Serial.printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    Serial.printf("This is mot_remote project\r\n");

    // Configure USB CDC
    SerialUSB.begin();

    Serial.printf("Finish Setup\r\n");
}

void loop() {
/*
    int8_t c = SerialUSB.read();
    if (c != -1) {
         Serial.printf("Recv: %d\r\n", c);
        SerialUSB.write("TEST\r\n", 7);
        SerialUSB.printf("TEST: %c\r\n", c);
    }
*/
    uint8_t buf[64] = {0};
    if(SerialUSB.readBytesUntil('\n', buf, 64)) {
        Serial.printf("Recv: %s\r\n", (char *)buf); 
        SerialUSB.printf("Recv: %s\r\n", (char *)buf); 
    }
}
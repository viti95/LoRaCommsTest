#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "../Common/vars.h"

int buttonState = 0;  // variable for reading the pushbutton status

void setup() {

  pinMode(PRG_BUTTON, INPUT);

  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Sender");

  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setTxPower(17);
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(8); 
  LoRa.enableCrc();
}

void loop() {
  Serial.println("Loop");

  buttonState = digitalRead(PRG_BUTTON);

  if (buttonState == LOW) { // WTF

    Serial.println("Send packet!");

    // send packet
    LoRa.beginPacket();
    LoRa.print(BOX_MESSAGE);
    LoRa.endPacket();

    delay(1000);
  }

}

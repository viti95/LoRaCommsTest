#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "../Common/vars.h"

// Buttons
#define BUTTON_0 36
#define BUTTON_1 37
#define BUTTON_2 38
#define BUTTON_3 39
#define BUTTON_4 32
#define BUTTON_5 33
#define BUTTON_6 12
#define BUTTON_7 13

int buttonState = 0;  // variable for reading the pushbutton status

void setup() {

  pinMode(BUTTON_0, INPUT);

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

  buttonState = digitalRead(BUTTON_0);

  if (buttonState == LOW) {

    Serial.println("Send packet!");

    // send packet
    LoRa.beginPacket();
    LoRa.print(BOX_MESSAGE);
    LoRa.endPacket();

    delay(1000);
  }

}

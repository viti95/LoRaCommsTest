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
#define BUTTON_6 17
#define BUTTON_7 13

int buttonState0 = 0;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;
int buttonState6 = 0;
int buttonState7 = 0;

void setup() {

  pinMode(BUTTON_0, INPUT);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(BUTTON_4, INPUT);
  pinMode(BUTTON_5, INPUT);
  pinMode(BUTTON_6, INPUT);
  pinMode(BUTTON_7, INPUT);

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

  buttonState0 = digitalRead(BUTTON_0);

  if (buttonState0 == LOW) {

    Serial.println("Button K1, send packet!");

    // send packet
    LoRa.beginPacket();
    LoRa.print(CMD_BOX);
    LoRa.endPacket();

    delay(1000);
  }

  buttonState1 = digitalRead(BUTTON_1);

  if (buttonState1 == LOW) {

    Serial.println("Button K2, send packet!");

    // send packet
    LoRa.beginPacket();
    LoRa.print(CMD_YELLOW_FLAG);
    LoRa.endPacket();

    delay(1000);
  }

  buttonState2 = digitalRead(BUTTON_2);

  if (buttonState2 == LOW) {

    Serial.println("Button K3, send packet!");

    // send packet
    LoRa.beginPacket();
    LoRa.print(CMD_SLOW);
    LoRa.endPacket();

    delay(1000);
  }

  buttonState3 = digitalRead(BUTTON_3);

  if (buttonState3 == LOW) {

    Serial.println("Button K4, send packet!");

    // send packet
    LoRa.beginPacket();
    LoRa.print(CMD_PUSH);
    LoRa.endPacket();

    delay(1000);
  }

  buttonState4 = digitalRead(BUTTON_4);

  if (buttonState4 == LOW) {

    Serial.println("Button K5, send packet!");

    // send packet
    LoRa.beginPacket();
    LoRa.print(CMD_DRIVE_THROUGH);
    LoRa.endPacket();

    delay(1000);
  }

  buttonState5 = digitalRead(BUTTON_5);

  if (buttonState5 == LOW) {

    Serial.println("Button K6, send packet!");

    // send packet
    /*LoRa.beginPacket();
    LoRa.print();
    LoRa.endPacket();*/

    delay(1000);
  }

  buttonState6 = digitalRead(BUTTON_6);

  if (buttonState6 == LOW) {

    Serial.println("Button K7, send packet!");

    // send packet
    LoRa.beginPacket();
    LoRa.print(CMD_BLOCK);
    LoRa.endPacket();

    delay(1000);
  }

  buttonState7 = digitalRead(BUTTON_7);

  if (buttonState7 == LOW) {

    Serial.println("Button K8, send packet!");

    // send packet
    /*LoRa.beginPacket();
    LoRa.print();
    LoRa.endPacket();*/

    delay(1000);
  }
}

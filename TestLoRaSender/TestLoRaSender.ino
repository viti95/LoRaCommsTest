#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

int buttonState = 0;  // variable for reading the pushbutton status

#define LORA_BAND 866E6

#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_CS 18
#define LORA_RST 14
#define LORA_DIO0 26
#define LORA_DIO1 35
#define LORA_DIO2 34

#define PRG_BUTTON 0

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
    LoRa.print("MVD BOX");
    LoRa.endPacket();

    delay(1000);
  }

}

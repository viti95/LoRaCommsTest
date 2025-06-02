#include <SPI.h>
#include <LoRa.h>
#include <U8g2lib.h>

#define LORA_BAND 866E6

#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_CS 18
#define LORA_RST 14
#define LORA_DIO0 26
#define LORA_DIO1 35
#define LORA_DIO2 34

#define OLED_CLOCK 15
#define OLED_DATA 4
#define OLED_RESET 16

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  u8g2.begin();
  u8g2.clear();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.setCursor(0,u8g2.getFontAscent() - u8g2.getFontDescent());
  u8g2.print("HELLO WORLD!");
  u8g2.sendBuffer();

  Serial.println("LoRa Receiver");

  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

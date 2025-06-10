#include <SPI.h>
#include <LoRa.h>
#include <U8g2lib.h>
#include <FastLED.h>
#include "../Common/vars.h"

#define NUM_LEDS 64
#define PIN_DATA_MATRIX 21

CRGBArray<NUM_LEDS> leds;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA);

int fontHeight = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Init WS2812 matrix led");

  FastLED.addLeds<NEOPIXEL, PIN_DATA_MATRIX>(leds, NUM_LEDS);

  Serial.println("Init SSD1306 display");

  u8g2.begin();
  u8g2.setContrast(255/4);
  u8g2.setFont(u8g2_font_logisoso50_tr);
  fontHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

  Serial.println("LoRa Receiver");

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
  // try to parse packet
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    // received a packet
    String text = LoRa.readString();

    Serial.println(text);

    if (text == BOX_MESSAGE) {
      Serial.println("Comparativa OK");

      for (int i = 0; i < 40; i++) {
        u8g2.clear();

        leds.fill_solid(CRGB::Black);
        
        if ((i % 2) == 0) {
          u8g2.setCursor(0,fontHeight);
          u8g2.print("BOX!");

          leds.fill_solid(CRGB::Red);
        }
          
        FastLED.show();
        u8g2.sendBuffer();

        delay(500);
      }

    } else {
      Serial.println("Comparativa MAL");
    }

    // print RSSI of packet
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());
  }
}

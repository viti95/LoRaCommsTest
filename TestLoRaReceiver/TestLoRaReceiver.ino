#include <SPI.h>
#include <LoRa.h>
#include <U8g2lib.h>
#include <FastLED.h>
#include "../Common/vars.h"

#define NUM_LEDS 64
#define PIN_DATA_MATRIX 13
#define OLED_BRIGHTNESS 255
#define RGB_BRIGHTNESS 255 / 8

#define MESSAGE_BOX "BOX"
#define MESSAGE_SLOW "SLOW"
#define MESSAGE_YELLOW_FLAG "YELLOW"
#define MESSAGE_PUSH "PUSH"
#define MESSAGE_DRIVE_THROUGH "DRV TR"
#define MESSAGE_BLOCK "BLOCK"

CRGBArray<NUM_LEDS> leds;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA);

int fontHeight = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Init WS2812 matrix led");

  FastLED.addLeds<NEOPIXEL, PIN_DATA_MATRIX>(leds, NUM_LEDS);
  FastLED.setBrightness(RGB_BRIGHTNESS);

  Serial.println("Init SSD1306 display");

  u8g2.begin();
  u8g2.setContrast(OLED_BRIGHTNESS);

  Serial.println("LoRa Receiver");

  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  LoRa.setTxPower(17);
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(8);
  LoRa.enableCrc();
}

void cmd_box() {

  u8g2.setFont(u8g2_font_logisoso50_tr);
  fontHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

  for (int i = 0; i < 10; i++) {
    u8g2.clear();

    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      u8g2.setCursor(0, fontHeight);
      u8g2.print(MESSAGE_BOX);

      leds.fill_solid(CRGB::Red);
    }

    FastLED.show();
    u8g2.sendBuffer();

    delay(250);
  }
}

void cmd_yellow_flag() {

  u8g2.setFont(u8g2_font_logisoso34_tr);
  fontHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

  for (int i = 0; i < 10; i++) {
    u8g2.clear();

    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      u8g2.setCursor(0, fontHeight);
      u8g2.print(MESSAGE_YELLOW_FLAG);
      leds.fill_solid(CRGB::Yellow);
    }

    FastLED.show();
    u8g2.sendBuffer();

    delay(250);
  }
}

void cmd_push() {

  u8g2.setFont(u8g2_font_logisoso50_tr);
  fontHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

  for (int i = 0; i < 10; i++) {
    u8g2.clear();

    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      u8g2.setCursor(0, fontHeight);
      u8g2.print(MESSAGE_PUSH);

      leds.fill_solid(CRGB::Green);
    }

    FastLED.show();
    u8g2.sendBuffer();

    delay(250);
  }
}

void cmd_drive_through() {

  u8g2.setFont(u8g2_font_logisoso34_tr);
  fontHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

  for (int i = 0; i < 10; i++) {
    u8g2.clear();

    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      u8g2.setCursor(0, fontHeight);
      u8g2.print(MESSAGE_DRIVE_THROUGH);

      leds.fill_solid(CRGB::OrangeRed);
    }

    FastLED.show();
    u8g2.sendBuffer();

    delay(250);
  }
}

void cmd_slow() {

  u8g2.setFont(u8g2_font_logisoso50_tr);
  fontHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

  for (int i = 0; i < 10; i++) {
    u8g2.clear();

    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      u8g2.setCursor(0, fontHeight);
      u8g2.print(MESSAGE_SLOW);

      leds.fill_solid(CRGB::Blue);
    }

    FastLED.show();
    u8g2.sendBuffer();

    delay(250);
  }

}

void cmd_block() {

  u8g2.setFont(u8g2_font_logisoso38_tr);
  fontHeight = u8g2.getFontAscent() - u8g2.getFontDescent();

  for (int i = 0; i < 10; i++) {
    u8g2.clear();

    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      u8g2.setCursor(0, fontHeight);
      u8g2.print(MESSAGE_BLOCK);

      leds.fill_solid(CRGB::White);
    }

    FastLED.show();
    u8g2.sendBuffer();

    delay(250);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    // received a packet
    String text = LoRa.readString();

    Serial.println(text);
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());

    if (text == CMD_BOX) {
      cmd_box();
    } else if (text == CMD_SLOW) {
      cmd_slow();
    } else if (text == CMD_YELLOW_FLAG) {
      cmd_yellow_flag();
    } else if (text == CMD_PUSH) {
      cmd_push();
    } else if (text == CMD_DRIVE_THROUGH) {
      cmd_drive_through();
    } else if (text == CMD_BLOCK) {
      cmd_block();
    }

  }
}

#include <SPI.h>
#include <LoRa.h>
#include <U8g2lib.h>
#include <FastLED.h>
#include "../Common/vars.h"

#define NUM_LEDS 64
#define PIN_DATA_MATRIX 13
#define OLED_BRIGHTNESS 255
#define RGB_BRIGHTNESS_HIGH 255
#define RGB_BRIGHTNESS_MEDIUM 60
#define RGB_BRIGHTNESS_LOW 10

#define MESSAGE_BRILLO_BAJO "Brillo BAJO"
#define MESSAGE_BRILLO_MEDIO "Brillo MEDIO"
#define MESSAGE_BRILLO_ALTO "Brillo ALTO"

#define MESSAGE_BOX "BOX"
#define MESSAGE_SLOW "SLOW"
#define MESSAGE_YELLOW_FLAG "YELLOW"
#define MESSAGE_PUSH "PUSH"
#define MESSAGE_DRIVE_THROUGH "DRV TR"
#define MESSAGE_BLOCK "BLOCK"

int buttonState = 0;  // variable for reading the pushbutton status
int brightness = 2;

CRGBArray<NUM_LEDS> leds;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA);

void setup() {

  pinMode(PRG_BUTTON, INPUT);

  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Init WS2812 matrix led");

  FastLED.addLeds<NEOPIXEL, PIN_DATA_MATRIX>(leds, NUM_LEDS);
  FastLED.setBrightness(RGB_BRIGHTNESS_HIGH);

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

  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(8);
  LoRa.enableCrc();
}

int get_y_cursor() {
  return 63;
}

void cmd_box() {

  u8g2.setFont(u8g2_font_logisoso50_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());
  u8g2.print(MESSAGE_BOX);
  u8g2.sendBuffer();

  for (int i = 0; i < 10; i++) {
    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      leds.fill_solid(CRGB::Red);
    }

    FastLED.show();

    delay(250);
  }

  u8g2.clear();
  u8g2.sendBuffer();
}

void cmd_yellow_flag() {

  u8g2.setFont(u8g2_font_logisoso34_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());
  u8g2.print(MESSAGE_YELLOW_FLAG);
  u8g2.sendBuffer();

  for (int i = 0; i < 10; i++) {
    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      leds.fill_solid(CRGB::Yellow);
    }

    FastLED.show();

    delay(250);
  }

  u8g2.clear();
  u8g2.sendBuffer();
}

void cmd_push() {

  u8g2.setFont(u8g2_font_logisoso50_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());
  u8g2.print(MESSAGE_PUSH);
  u8g2.sendBuffer();

  for (int i = 0; i < 10; i++) {
    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      leds.fill_solid(CRGB::Green);
    }

    FastLED.show();

    delay(250);
  }

  u8g2.clear();
  u8g2.sendBuffer();
}

void cmd_drive_through() {

  u8g2.setFont(u8g2_font_logisoso34_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());
  u8g2.print(MESSAGE_DRIVE_THROUGH);
  u8g2.sendBuffer();

  for (int i = 0; i < 10; i++) {
    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      leds.fill_solid(CRGB::OrangeRed);
    }

    FastLED.show();

    delay(250);
  }

  u8g2.clear();
  u8g2.sendBuffer();
}

void cmd_slow() {

  u8g2.setFont(u8g2_font_logisoso50_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());
  u8g2.print(MESSAGE_SLOW);
  u8g2.sendBuffer();

  for (int i = 0; i < 10; i++) {
    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      leds.fill_solid(CRGB::Blue);
    }

    FastLED.show();

    delay(250);
  }

  u8g2.clear();
  u8g2.sendBuffer();

}

void cmd_block() {

  u8g2.setFont(u8g2_font_logisoso38_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());
  u8g2.print(MESSAGE_BLOCK);
  u8g2.sendBuffer();

  for (int i = 0; i < 10; i++) {
    leds.fill_solid(CRGB::Black);

    if ((i % 2) == 0) {
      leds.fill_solid(CRGB::White);
    }

    FastLED.show();

    delay(250);
  }

  u8g2.clear();
  u8g2.sendBuffer();

}

void cmd_cambio_brillo() {
  
  brightness++;

  if (brightness == 3)
    brightness = 0;
  
  u8g2.setFont(u8g2_font_logisoso18_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());

  switch(brightness){
    case 0:
      u8g2.print(MESSAGE_BRILLO_BAJO);
      FastLED.setBrightness(RGB_BRIGHTNESS_LOW);  
      break;
    case 1:
      u8g2.print(MESSAGE_BRILLO_MEDIO);
      FastLED.setBrightness(RGB_BRIGHTNESS_MEDIUM);
      break;
    case 2:
      u8g2.print(MESSAGE_BRILLO_ALTO);
      FastLED.setBrightness(RGB_BRIGHTNESS_HIGH);
      break;
  }

  u8g2.sendBuffer();

  delay(1000);

  u8g2.clear();
  u8g2.sendBuffer();
}

void loop() {

  buttonState = digitalRead(PRG_BUTTON);

  // read button
  if (buttonState == LOW) { // WTF
    cmd_cambio_brillo();
  }

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

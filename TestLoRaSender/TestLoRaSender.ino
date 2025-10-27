#include <SPI.h>
#include <LoRa.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "../Common/vars.h"

#define OLED_BRIGHTNESS 255

#define MESSAGE_SENDING "SENDING"
#define MESSAGE_OK "OK"
#define MESSAGE_RECEIVED "RECEIVED"
#define MESSAGE_YES "YES"
#define MESSAGE_NO "NO"

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

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);

void setup()
{

  pinMode(BUTTON_0, INPUT);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(BUTTON_4, INPUT);
  pinMode(BUTTON_5, INPUT);
  pinMode(BUTTON_6, INPUT);
  pinMode(BUTTON_7, INPUT);

  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Init SSD1306 display");

  u8g2.begin();
  u8g2.setContrast(OLED_BRIGHTNESS);

  Serial.println("LoRa Sender");

  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_BAND))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setCodingRate4(8);
  LoRa.enableCrc();
}

int get_y_cursor()
{
  return 63;
}

void display_send_msg()
{
  u8g2.setFont(u8g2_font_logisoso30_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());
  u8g2.print(MESSAGE_SENDING);
  u8g2.sendBuffer();
}

void display_ok()
{
  u8g2.setFont(u8g2_font_logisoso50_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());
  u8g2.print(MESSAGE_OK);
  u8g2.sendBuffer();
}

void display_msg_received()
{
  u8g2.setFont(u8g2_font_logisoso24_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());
  u8g2.print(MESSAGE_RECEIVED);
  u8g2.sendBuffer();
}

void display_msg_yes()
{
  u8g2.setFont(u8g2_font_logisoso50_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());
  u8g2.print(MESSAGE_YES);
  u8g2.sendBuffer();
}

void display_msg_no()
{
  u8g2.setFont(u8g2_font_logisoso50_tr);
  u8g2.clear();
  u8g2.setCursor(0, get_y_cursor());
  u8g2.print(MESSAGE_NO);
  u8g2.sendBuffer();
}

void display_clear()
{
  u8g2.clear();
  u8g2.sendBuffer();
}

void cmd_button_1()
{

  Serial.println("Button K1, send packet!");

  display_send_msg();

  // send packet
  LoRa.beginPacket();
  LoRa.print(CMD_BOX);
  LoRa.endPacket();

  display_ok();

  delay(1000);

  display_clear();
}

void cmd_button_2()
{

  Serial.println("Button K2, send packet!");

  display_send_msg();

  // send packet
  LoRa.beginPacket();
  LoRa.print(CMD_YELLOW_FLAG);
  LoRa.endPacket();

  display_ok();

  delay(1000);

  display_clear();
}

void cmd_button_3()
{
  Serial.println("Button K3, send packet!");

  display_send_msg();

  // send packet
  LoRa.beginPacket();
  LoRa.print(CMD_SLOW);
  LoRa.endPacket();

  display_ok();

  delay(1000);

  display_clear();
}

void cmd_button_4()
{
  Serial.println("Button K4, send packet!");

  display_send_msg();

  // send packet
  LoRa.beginPacket();
  LoRa.print(CMD_PUSH);
  LoRa.endPacket();

  display_ok();

  delay(1000);

  display_clear();
}

void cmd_button_5()
{
  Serial.println("Button K5, send packet!");

  display_send_msg();

  // send packet
  LoRa.beginPacket();
  LoRa.print(CMD_DRIVE_THROUGH);
  LoRa.endPacket();

  display_ok();

  delay(1000);

  display_clear();
}

void cmd_button_6()
{

  Serial.println("Button K6, send packet!");

  // display_send_msg();

  // send packet
  /*LoRa.beginPacket();
  LoRa.print();
  LoRa.endPacket();*/

  // display_ok();

  // delay(1000);

  // display_clear();
}

void cmd_button_7()
{

  Serial.println("Button K7, send packet!");

  display_send_msg();

  // send packet
  LoRa.beginPacket();
  LoRa.print(CMD_BLOCK);
  LoRa.endPacket();

  display_ok();

  delay(1000);

  display_clear();
}

/*void cmd_button_8()
{
  Serial.println("Button K8, send packet!");

  // display_send_msg();

  // send packet
  LoRa.beginPacket();
  LoRa.print();
  LoRa.endPacket();

  // display_ok();

  // delay(1000);

  // display_clear();
}*/

void loop()
{

  buttonState0 = digitalRead(BUTTON_0);

  if (buttonState0 == LOW)
  {
    cmd_button_1();
  }

  buttonState1 = digitalRead(BUTTON_1);

  if (buttonState1 == LOW)
  {
    cmd_button_2();
  }

  buttonState2 = digitalRead(BUTTON_2);

  if (buttonState2 == LOW)
  {
    cmd_button_3();
  }

  buttonState3 = digitalRead(BUTTON_3);

  if (buttonState3 == LOW)
  {
    cmd_button_4();
  }

  buttonState4 = digitalRead(BUTTON_4);

  if (buttonState4 == LOW)
  {
    cmd_button_5();
  }

  buttonState5 = digitalRead(BUTTON_5);

  if (buttonState5 == LOW)
  {
    cmd_button_6();
  }

  buttonState6 = digitalRead(BUTTON_6);

  if (buttonState6 == LOW)
  {
    cmd_button_7();
  }

  // NO FUNCIONA ESTE BOTON EN EL HARDWARE
  /*buttonState7 = digitalRead(BUTTON_7);

  if (buttonState7 == LOW) {
    cmd_button_8();
  }*/

  // Leer serie, si llega un mensaje, mandarlo

  if (Serial.available())
  {

    // Esperar a que llegue todo
    delay(100);

    display_send_msg();

    String text;

    LoRa.beginPacket();

    LoRa.print(CMD_MSG);

    while (Serial.available())
    {
      char inChar = (char)Serial.read();
      LoRa.print(inChar);
    }

    LoRa.print('\n');
    LoRa.endPacket();

    display_ok();

    delay(1000);

    display_clear();
  }

  // Ver si ha llegado un mensaje de confirmación
  int packetSize = LoRa.parsePacket();

  if (packetSize)
  {
    // Mostrar mensaje ok
    String text = LoRa.readString();

    if (text == CMD_OK)
    {
      display_msg_received();
      delay(2000);
      display_clear();
    }

    if (text == CMD_YES)
    {
      display_msg_yes();
      delay(2000);
    }

    if (text == CMD_NO)
    {
      display_msg_no();
      delay(2000);
    }
  }
}

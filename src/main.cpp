/*
  * Code : Arduino Home Automation & Security
  * Author : Atick Faisal
  * License : GPL-3.0
*/

#include <Arduino.h>
#include <LiquidCrystal.h>

void __init_lcd_display();
void __on_bluetooth_message_receive();

#define REGESTER_SELECT 12
#define ENABLE 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2
#define LED_PIN 13
#define RELAY_PIN 8
#define RELAY_POWER 7
#define RELAY_GND 6

#define ADC_THRESHOLD 200

char incoming_byte;
unsigned int counter = 0;
unsigned int adc_value = 0;

LiquidCrystal lcd(REGESTER_SELECT, ENABLE, D4, D5, D6, D7);

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_POWER, INPUT);
  pinMode(RELAY_GND, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(RELAY_GND, LOW);

  __init_lcd_display();
}

void loop() {
  adc_value = analogRead(A1);
  if(adc_value > 200) {
    lcd.setCursor(9, 0);
    lcd.print(++counter);
    lcd.setCursor(0, 1);
    lcd.print("L : ON ");
    digitalWrite(13, HIGH);
    __on_bluetooth_message_receive();
  } else {
    digitalWrite(13, LOW);
    __on_bluetooth_message_receive();
  }
}

void __init_lcd_display() {
  lcd.begin(16, 2);
  lcd.print("PEOPLE : 0");
  lcd.setCursor(0, 1);
  lcd.print("L : OFF");
  lcd.setCursor(9, 1);
  lcd.print("F : OFF");
}

void __on_bluetooth_message_receive() {
  while(adc_value > 200) {
    adc_value = analogRead(A1);
    if(Serial.available() > 0) {
      incoming_byte = Serial.read();
      if(incoming_byte == '1') {
        digitalWrite(8, HIGH);
        lcd.setCursor(9, 1);
        lcd.print("F : ON ");
      }
      if(incoming_byte == '0') {
        digitalWrite(8, LOW);
        lcd.setCursor(9, 1);
        lcd.print("F : OFF");
      }
    }
  }
}
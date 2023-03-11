#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
const int SS_PIN = 10; //SDA RFID
const int RST_PIN = 9;
const int relay = 8;
const int buzzer = 7;
const int red_led = 6;
const int green_led = 5;

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 module_rfid(SS_PIN, RST_PIN);

void setup() {
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  lcd.init();
  lcd.backlight();
  SPI.begin();
  module_rfid.PCD_Init();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("SCAN YOUR TAG");

  if ( ! module_rfid.PICC_IsNewCardPresent())
  {
    return;
  }

  if ( ! module_rfid.PICC_ReadCardSerial())
  {
    return;
  }

  String UID = "";

  for (byte i = 0; i < module_rfid.uid.size; i++) {
    UID.concat(String(module_rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    UID.concat(String(module_rfid.uid.uidByte[i], HEX));
  }

  UID.toUpperCase();

  if (UID.substring(1) == "03 B0 0B ED")
  {
    lcd.clear();
    lcd.print("OPENING...");
    digitalWrite(relay, HIGH);
    digitalWrite(green_led, HIGH);
    delay(2000);
    digitalWrite(green_led, LOW);
    digitalWrite(relay, LOW);
  }

  else {
    lcd.clear();
    lcd.print("WRONG TAG");
    digitalWrite(red_led, HIGH);
    tone(buzzer, 200, 750);
    delay(1000);
    digitalWrite(red_led, LOW);
  }
}

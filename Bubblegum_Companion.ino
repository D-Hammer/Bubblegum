/*
   Control software for the Bubblegum Companion PID programmer

                                             By D.Hammer
*/
#include <NeoSWSerial.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#define debug 0 // Serial debug mode, 0 for off, 1 for on
//#include <SoftwareSerial.h>

////////// Create instances //////////
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
NeoSWSerial mserial(15, 18); // RX, TX

////////// Vars //////////
uint8_t mode = 6; // What mode are we in? 0-P, 1-I, 2-D, 3-S, 4-Read, 5-Reset
uint16_t Pi, Ii, Di; // Scaled values, max is 65535
double P, I, D; // Unscaled values, Xi/1000.0
uint8_t S; // Throttle scaling factor
uint8_t longPressVal;
uint16_t delayInterval = 500;

void setup() {
  Serial.begin(9600);
  mserial.begin(9600); // Pour a bowl of Serial. Begins serial com

  analogWrite(10, 100);
  lcd.begin(16, 2); // Number of columns, rows
  lcd.setCursor(0, 0);
  lcd.write("     Hammer");
  lcd.setCursor(0, 1);
  lcd.write("   Industries");

  initialize();

  pinMode(6, INPUT_PULLUP); // Enter/send
  pinMode(7, INPUT_PULLUP); // Left
  pinMode(8, INPUT_PULLUP); // Mode
  pinMode(9, INPUT_PULLUP); // Right
  pinMode(15, INPUT_PULLUP); // Prevent floating
  pinMode(16, INPUT_PULLUP); // Prevent floating
  pinMode(17, OUTPUT); // Reset line
  digitalWrite(17, HIGH);

  delay(2000);
  lcd.clear();
}

void loop() {
  Serial.println(digitalRead(8));
  if (digitalRead(6) == 0) { // Enter/send
    if (mode < 4) {
      save();
      transmitData();
    }
    else if (mode == 4) {
      requestData();
      save();
    }
    else {
      EEPROM.write(0, 0); // Reset the initialized token
      initialize();
      mode = 6;
    }
    while (digitalRead(6) == 0);
  }
  else if (digitalRead(8) == 0 || mode > 5) { // Mode
    mode ++;
    if (mode > 5) {
      mode = 0;
      lcd.clear();
      lcd.blink();
      lcd.setCursor(0, 0);
      lcd.print("P:       I:     ");
      lcd.setCursor(2, 0);
      lcd.print(P, 3);
      lcd.setCursor(11, 0);
      lcd.print(I, 3);
      lcd.setCursor(0, 1);
      lcd.print("D:       S:");
      lcd.setCursor(2, 1);
      lcd.print(D, 3);
      lcd.setCursor(11, 1);
      lcd.print(S);
      lcd.setCursor(0, 0);
    }
    if (mode == 1) lcd.setCursor(9, 0);
    if (mode == 2) lcd.setCursor(0, 1);
    if (mode == 3) lcd.setCursor(9, 1);
    if (mode == 4) {
      lcd.clear();
      lcd.print("   Read from");
      lcd.setCursor(0, 1);
      lcd.print("   target?");
      lcd.noBlink();
    }
    if (mode == 5) {
      lcd.clear();
      lcd.print("  Reset vars?");
      lcd.noBlink();
    }
    delay(50);
    while (digitalRead(8) == 0);
  }
  else if (digitalRead(9) == 0) { // Left
    if (longPressVal < 10) longPressVal++;
    if (mode == 0) {
      Pi--;
      if (Pi < 0) Pi = 0;
      P = Pi / 1000.0;
      lcd.setCursor(2, 0);
      lcd.print(P, 3);
      lcd.setCursor(0, 0);
    }
    if (mode == 1) {
      Ii--;
      if (Ii < 0) Ii = 0;
      I = Ii / 1000.0;
      lcd.setCursor(11, 0);
      lcd.print(I, 3);
      lcd.setCursor(9, 0);
    }
    if (mode == 2) {
      Di--;
      if (Di < 0) Di = 0;
      D = Di / 1000.0;
      lcd.setCursor(2, 1);
      lcd.print(D, 3);
      lcd.setCursor(0, 1);
    }
    if (mode == 3) {
      if (S > 0) S--;
      lcd.setCursor(11, 1);
      lcd.print(S);
      lcd.setCursor(9, 1);
    }
    delayInterval = 500 - 50 * longPressVal + 1;
    delay(delayInterval);
  }
  else if (digitalRead(7) == 0) { // Right
    if (longPressVal < 10) longPressVal++;
    if (mode == 0) {
      Pi++;
      if (Pi > 50000) Pi = 50000;
      P = Pi / 1000.0;
      lcd.setCursor(2, 0);
      lcd.print(P, 3);
      lcd.setCursor(0, 0);
    }
    if (mode == 1) {
      Ii++;
      if (Ii > 10000) Ii = 10000;
      I = Ii / 1000.0;
      lcd.setCursor(11, 0);
      lcd.print(I, 3);
      lcd.setCursor(9, 0);
    }
    if (mode == 2) {
      Di++;
      if (Di > 50000) Di = 50000;
      D = Di / 1000.0;
      lcd.setCursor(2, 1);
      lcd.print(D, 3);
      lcd.setCursor(0, 1);
    }
    if (mode == 3) {
      S++;
      if (S > 100) S = 100;
      lcd.setCursor(11, 1);
      lcd.print(S);
      lcd.setCursor(9, 1);
    }
    delayInterval = 500 - 50 * longPressVal + 1;
    delay(delayInterval);
  }
  else longPressVal = 1;
  delayInterval = 500 - 50 * longPressVal + 1;
  delay(1);
}

void transmitData() {
  digitalWrite(17, LOW); // Reset target
  lcd.clear();
  lcd.write("Transmitting...");
  delay(1);
  digitalWrite(17, HIGH); // Start target
  delay(90);
  mserial.write(4);

  delay(1);
  mserial.write(highByte(Pi));
  mserial.write(lowByte(Pi));
  mserial.write(highByte(Ii));
  mserial.write(lowByte(Ii));
  mserial.write(highByte(Di));
  mserial.write(lowByte(Di));
  mserial.write(S);
 
  delay(100);
  lcd.setCursor(0, 1);
  lcd.write("Done.");
  delay(1000);
  
  mserial.read(); // Target transmits even on send, reading to flush serial buffer
  mserial.read();
  mserial.read();
  mserial.read();
  mserial.read();
  mserial.read();
  mserial.read();
  
  mode = 6; // Return
}

void requestData() {
  digitalWrite(17, LOW); // Reset target
  lcd.clear();
  lcd.write("Requesting...");
  delay(1);
  digitalWrite(17, HIGH); // Start target
  delay(90);
  mserial.write(11);

  uint16_t i = 0;
  while (mserial.available() < 7 && i < 5000) {
    i++;
    delay(1);
  }
  lcd.setCursor(0, 1);
  if (i < 5000) {
    Pi = mserial.read() << 8 | mserial.read();
    Ii = mserial.read() << 8 | mserial.read();
    Di = mserial.read() << 8 | mserial.read();
    S = mserial.read();
    P = Pi / 1000.0; // Scale values for use
    I = Ii / 1000.0;
    D = Di / 1000.0;
    lcd.write("Done.");
  }
  else {
    lcd.write("Failed.");
    delay(2000);
  }
  delay(1000);
  mode = 6;
}

void initialize() {
  uint8_t memIsInitialized = EEPROM.read(0);
  if (memIsInitialized == 0) { // If the EEPROM is empty
    EEPROM.write(1, 0x07); // High and low bits for Pi, Ii, and Di in hex
    EEPROM.write(2, 0x08);
    EEPROM.write(3, 0x00);
    EEPROM.write(4, 0x14);
    EEPROM.write(5, 0x00);
    EEPROM.write(6, 0x05);
    EEPROM.write(7, 30);
    EEPROM.write(0, 1); // Set as initialized
  }
  Pi = EEPROM.read(1) << 8 | EEPROM.read(2); // Read values from EEPROM
  Ii = EEPROM.read(3) << 8 | EEPROM.read(4);
  Di = EEPROM.read(5) << 8 | EEPROM.read(6);
  S = EEPROM.read(7);
  P = Pi / 1000.0;
  I = Ii / 1000.0;
  D = Di / 1000.0;
}

void save() {
  EEPROM.write(1, highByte(Pi)); // High and low bits for Pi, Ii, and Di in hex
  EEPROM.write(2, lowByte(Pi));
  EEPROM.write(3, highByte(Ii));
  EEPROM.write(4, lowByte(Ii));
  EEPROM.write(5, highByte(Di));
  EEPROM.write(6, lowByte(Di));
  EEPROM.write(7, S);
}


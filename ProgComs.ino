void initialize() {
  uint8_t memIsInitialized = EEPROM.read(0);
  if (memIsInitialized == 0) { // If the EEPROM is empty
    EEPROM.write(1, 0x07); // High and low bits for Pi, Ii, and Di in hex
    EEPROM.write(2, 0x0A);
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

void sendData() {
  pserial.write(highByte(Pi));
  pserial.write(lowByte(Pi));
  pserial.write(highByte(Ii));
  pserial.write(lowByte(Ii));
  pserial.write(highByte(Di));
  pserial.write(lowByte(Di));
  pserial.write(S);
}

void receiveData() {
  while (pserial.available() < 7); // Wait for values to be transmitted
  Pi = (pserial.read() << 8) | pserial.read(); // Read values
  Ii = (pserial.read() << 8) | pserial.read();
  Di = (pserial.read() << 8) | pserial.read();
  S = pserial.read();
  EEPROM.write(1, highByte(Pi)); // Save values
  EEPROM.write(2, lowByte(Pi));
  EEPROM.write(3, highByte(Ii));
  EEPROM.write(4, lowByte(Ii));
  EEPROM.write(5, highByte(Di));
  EEPROM.write(6, lowByte(Di));
  EEPROM.write(7, S);
  P = Pi / 1000.0; // Scale values for use
  I = Ii / 1000.0;
  D = Di / 1000.0;
}

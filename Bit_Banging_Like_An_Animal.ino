int16_t dShotVal = 0;
uint8_t pin;

/* Valid pin values and their corresponding pins:
    IDE   Port Pin   Value
    0     PD0       (Don't do it, you're gonna be sad!!!)
    1     PD1        2
    2     PD2        4
    3     PD3        8
    4     PD4        16
    5     PD5        32
    6     PD6        64
    7     PD7        128
*/

void setup() {
  pin = 4;
  for (uint16_t i = 0; i < 500; i++) {
    transmit_dShot(dShotVal, pin);
    delay(10);
  }
  dShotVal = 1024 + 24;
}

void loop() {
  //pin = 4;
  transmit_dShot(dShotVal, pin);
  delay(1);
  // At least 2 uS must pass after end of previous frame to allow ESC check and re-sync
}

void transmit_dShot(int16_t dShotVal, uint8_t pin) {
  // Take in the value from 48 to 2047 (1-47 are reserved) and also the pin location as a binary port map
  DDRD |= pin; // Set the pin as output (just in case...)
  uint8_t checksum = ((dShotVal >> 7)&B1111) ^ ((dShotVal >> 3)&B1111) ^ ((dShotVal << 1)&B1110); // Create the checksum
  dShotVal = (dShotVal << 5) | checksum; // Create the shot frame
  uint8_t notpin = ~pin; // Save clock cycles by doing this now

  // Hundreds of lines of sad ugly bit banging.  It works.  I'm not proud of this
  if ((dShotVal >> 15) & 0x01) { // Check if 1 or 0
    PORTD |= pin;  // Turn the pin on
    PORTD |= pin;  // Waste time
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin; //Turn off, then waste more time
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 14) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 13) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 12) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 11) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 10) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 9) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 8) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 7) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 6) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 5) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 4) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 3) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 2) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if ((dShotVal >> 1) & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  if (dShotVal & 0x01) { // Check if 1 or 0
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
  else {
    PORTD |= pin;
    PORTD |= pin;
    //PORTD |= pin;
    //PORTD |= pin;
    //PORTD &= notpin;
    //PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
    PORTD &= notpin;
  }
}


#define BAUD_RATE 1200000
#define BAUD_RATE_DIVISOR (F_CPU / 8 / BAUD_RATE - 1)
int16_t dShotVal;
uint8_t checksum;

void setup() {
  UCSR0A = 1;
  UCSR0B = 1 << TXEN0;
  UCSR0C = 0 << UCSZ01 | 0 << UCSZ00;
  UBRR0 = BAUD_RATE_DIVISOR;

  dShotVal = 1000;
}

void loop() {
  transmit_dShot(dShotVal);

  delayMicroseconds(100);
}

void transmit_dShot(int16_t dShotVal) { // Take in the value from 48 to 2047 (1-47 are reserved)
  checksum = ((dShotVal >> 7)&B1111) ^ ((dShotVal >> 3)&B1111) ^ ((dShotVal << 1)&B1111); // Create the checksum
  dShotVal = (dShotVal << 5) | checksum; // Create the shot frame

  for (uint8_t i = 0; i < 16; i++) { // For each of the 16 bits in the frame
    if ((dShotVal >> (15 - i)) & 0x01) { // Check if 1 or 0
      loop_until_bit_is_set(UCSR0A, UDRE0); // Wait until previous bit finishes sending
      UDR0 = B10000; // Send new bit over USART
    } else {
      loop_until_bit_is_set(UCSR0A, UDRE0);
      UDR0 = B11100;
    }
  }
}

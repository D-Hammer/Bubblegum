#include <Wire.h>
#include <Servo.h>
#include <FUTABA_SBUS.h> // Source: http://www.ernstc.dk/arduino/sbus.html
#include <EEPROM.h>
#define debug 0 // Serial debug mode, 0 for off, 1 for on
#include <NeoSWSerial.h> // For debugging and programming

////////// Create instances //////////
#if debug
NeoSWSerial mserial(10, 11); // RX, TX (serial for debug)
#endif
NeoSWSerial pserial(11, 12); // RX, TX (serial for programmer)
FUTABA_SBUS sBus;                 // sBus over serial
Servo WeaponOUT;                  // Weapon output

////////// IMU Data //////////
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;
uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data
double gyroBuffer[5]; // Buffer for gyro low pass filter
double fGyro, gyroOffset;  // Filtered gyro value and gyro calibration offset
uint8_t i;

////////// Control Vars //////////
double expovalThrottle = 0.25;
uint8_t deadband = 15;
uint8_t inversionCounter;

////////// Pid Vars //////////
uint16_t Pi, Ii, Di; // Scaled values, max is 65535
double Kp = 1.81, Ki = 0.02, Kd = 0.005;
double P0, P, I, D;
int16_t maxSpin = 500; // Degrees per second, Remember to set gyro scale register accordingly
uint8_t S, progInstruct; // Throttle scaling factor

////////// DShot Vars //////////
int16_t dShotVal = 0; // Initialize in disarm status, valid vals: 48-2047
uint8_t pin = 32; // Right wheel is 32, Left wheel is 64

void setup() {
#if debug
  mserial.begin(9600); // Pour a bowl of Serial. Begins debug serial com
#endif


  ///// PID Programming /////
  pserial.begin(9600);
  pinMode(12, OUTPUT);
  initialize();
  while (pserial.available() == 0 && i < 100) {
    i++;
    delay(1);
  }
  i = 0;
  if (pserial.available() > 0) progInstruct = pserial.read();
  if (progInstruct == 4) {
    receiveData();
  }
  sendData();


  ///// Normal Startup /////
  sBus.begin();
  Wire.begin();          // Begins I2C for IMU
  Wire.setClock(400000UL); // Set I2C frequency to 400kHz


  pinMode(13, OUTPUT);  // Indicator LED
  digitalWrite(13, 1);
  delay(1000);
  digitalWrite(13, 0); // Indicator LED off


#if debug
  mserial.println("POST");
#endif


  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x18; // Set Gyro Full Scale Range to ±500deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while (i2cWrite(0x19, i2cData, 4, false)); // Write to all four registers at once
  while (i2cWrite(0x1B, 0x10, true));
  while (i2cWrite(0x6B, 0x01, true)); // PLL with X axis gyroscope reference and disable sleep mode
  while (i2cRead(0x75, i2cData, 1));


  delay(100); // Wait for sensor to stabilize
  while (i < 150) { // Calibrate gyro
    while (i2cRead(0x47, i2cData, 2)); // Read value
    gyroZ = (i2cData[0] << 8) | i2cData[1];
    if (gyroZ < 2500 && gyroZ > -2500) { // Save value if not moving too much
      i += 1;
      gyroOffset += gyroZ;
    }
    transmit_dShot(0, 32);
    transmit_dShot(0, 64);
    delay(30);
  }
  i = 0;
  gyroOffset = gyroOffset / 150.0 / 131.0; // Average over 100 values and scale to deg/s


  digitalWrite(13, 1);

  WeaponOUT.attach(15);
  for (uint16_t i = 0; i < 500; i++) { // Initialize the drive in unarmed state
    transmit_dShot(dShotVal, 32);
    transmit_dShot(dShotVal, 64);
    delay(10);
  }

  timer = micros();
}

void loop() {

  // SBUS in
  sBus.FeedLine();
  if (sBus.toChannels == 1) {
    sBus.UpdateServos();
    sBus.UpdateChannels();
    sBus.toChannels = 0;
  }


  // Expo
  double expo;
  expo = (double)(sBus.channels[4] - 172) / 1640.0; // Ch5 sets expo %
  double ch1 = (sBus.channels[1] - 991.5) / 820.0; // Apply expo
  ch1 = expo * ch1 * ch1 * ch1 + (1.0 - expo) * ch1;
  double ch2 = (sBus.channels[2] - 991.5) / 820.0;
  ch2 = expovalThrottle * ch2 * ch2 * ch2 + (1.0 - expovalThrottle) * ch2; // Added separate expo val for forward/reverse 12/24/17 per Matt & Brit feedback



  // Mixing
  // Determine desired movement
  double spd, clockwise_spin;
  if (sBus.channels[5] > 1600) { // Ch6 toggles between straight and diagonal drive
    spd = (ch2 - ch1) / 2.0; // Diagonal mix
    clockwise_spin = (ch2 + ch1) / 2.0;
  } else { // Straight
    spd = ch2;
    clockwise_spin = ch1;
  }


  // Upside down reversing
  if (accZ < -11000) { // If sensor says 3/4ish upside down or more
    if (inversionCounter < 200) inversionCounter++; // if buffer isn't saturated then increment
  }
  else {
    if (inversionCounter > 0) inversionCounter--; // if not upside down and buffer isn't zero'd then decrement
  }
  if (inversionCounter > 100) { // if buffer shows we're upside down a lot recently then invert controls
    spd = -spd;
  }


  /* Update all the values */
  while (i2cRead(0x3B, i2cData, 14));
  //accX = ((i2cData[0] << 8) | i2cData[1]); // We don't care about these commented out datas presently
  //accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  //tempRaw = (i2cData[6] << 8) | i2cData[7];
  //gyroX = (i2cData[8] << 8) | i2cData[9];
  //gyroY = (i2cData[10] << 8) | i2cData[11];
  gyroZ = (i2cData[12] << 8) | i2cData[13];

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  // Lowpass filter the gyro
  i += 1;
  if (i > 4) i = 0;
  gyroBuffer[i] = gyroZ;
  fGyro = (gyroBuffer[0] + gyroBuffer[1] + gyroBuffer[2] + gyroBuffer[3] + gyroBuffer[4]) / 5;


  // PID
  if (sBus.channels[6] > 1600) { // Ch7 toggles PID on/off
    clockwise_spin = clockwise_spin * maxSpin; // Rescale from % to degrees/sec
    if (abs(clockwise_spin) * 2 < 1) clockwise_spin = 0; // Deadband
    P0 = P;
    if (inversionCounter > 100) { // No death spirals!
      P = clockwise_spin - (fGyro / 131.0 - gyroOffset);
    } else {
      P = clockwise_spin + (fGyro / 131.0 - gyroOffset); // P is error, 131 scales to deg/s for sensitivity of 250, add calibration
    }
    if (abs(P) < 10 && abs(spd) < 0.02) P = 0; // PID deadband to prevent zero throttle jittering
    I += P * dt;
    D = (P - P0) / dt;
    clockwise_spin = (clockwise_spin + (S / 100.0 + abs(spd)) * (Kp * P + Ki * I + Kd * D)) / maxSpin; // Apply PID and rescale back to %
  }


  // Convert from spd and cws to wheel outputs
  if (spd - clockwise_spin < 0) {
    ch1 = (spd - clockwise_spin + 1.0) * -999.0 + 1047;
    if (ch1 > 1047) ch1 = 1047;
    if (ch1 < 48 + deadband) ch1 = 1048;
  }
  else {
    ch1 = (spd - clockwise_spin) * 999.0 + 1048;
    if (ch1 > 2047) ch1 = 2047;
    if (ch1 < 1048 + deadband) ch1 = 1048;
  }
  if (spd + clockwise_spin < 0) {
    ch2 = (spd + clockwise_spin + 1.0) * -999.0 + 1047;
    if (ch2 > 1047) ch2 = 1047;
    if (ch2 < 48 + deadband) ch2 = 1048;
  }
  else {
    ch2 = (spd + clockwise_spin) * 999.0 + 1048;
    if (ch2 > 2047) ch2 = 2047;
    if (ch2 < 1048 + deadband) ch2 = 1048;
  }


  // Update outputs
  dShotVal = ch1; pin = 32; // Right wheel, ch1
  transmit_dShot(dShotVal, pin);
  dShotVal = ch2; pin = 64; // Left wheel, ch2
  transmit_dShot(dShotVal, pin);
  WeaponOUT.writeMicroseconds(map(sBus.channels[0], 172, 1811, 1000, 2000)); // Output weapon motor signal
}

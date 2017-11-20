# Bubblegum
## Integrated control board

### Scope: 
- Interface with an RC receiver
  - Sbus is priority
  - Other variants will be created later
- Control drive motors
  - 2 motors priority
  - 4 motors later to allow use for Mecanum-Master-Race
- Control weapon motor
- IMU/PID drive to make up for my lack of driving skill
- Remote adjustable expo, user selectable mixing, sense and remix for inverted driving, remote switch for PID

### To Do
- Test and tune on various drive topologies
- Branch HW and FW to interface w/ standard pwm receivers


# Changelog
## Hardware

### V1.20
- IMU integrated into control board.  
- 3.3v power system added
- Decreased current capacity on 5V supply to free up board space
- 0.8mm FR4, 2oz Cu

### V1.10
- Increased power input hole diameters

### V1.01
- Rerouted weapon pwm signal to different GPIO
- Added versioning and logo on back silk
- 1.5mm FR4, 1oz Cu

### V1.00
- Prototype
- 0.8mm FR4, 2oz Cu

## Firmware

### 11/20/17
- Added weapon output to DS branch (servo lib implementation)
- Update PWM branch with DS branch improvements
- New PID gains: P1.8 I0.02 D0.005

### 11/17/17
- Filtered the inversion sensing
- added deadband to PID to stop zero throttle flutter
- PID correction now proportional to throttle input [0.5 + abs(spd)]
- Tuned gains. Settled on P1.2 I0.02 D0.05

### 11/15/17
- Added deadband to drive inputs
- Increased data inclusivity on gyro calibration
- Fixed expo scaling issue
- Fixed sign of gyroOffset in PID
- DShot center is 1048 in 3D mode, NOT 1047 :/

### 11/14/17 
- Added alternate FW for DShot implementation
- dshot fiddlings file uses UART and requires inverter (untested) (DShot150)
- bit banging like an animal is addressable on port D and functional (DShot600)
- New FW syncs ESC update with PID loop and increases frequency from 140 Hz to ~1KHz
- All the standard anti-jitter, digital, checksum goodness you've come to expect from DShot!

# Code Design

## ESP32

### Functionalities
1. Draw screen
2. Update LEDs
3. Retrieve song info
4. Settings
5. Storage

#### Screen
1. Init
2. Setup screen
- Maybe not needed
- Input bluetooth config
3. Main screen
- Background
- Settings button
- Song info (updates)
4. Settings screen
- Leds 
  - on/off
  - style
  - sensitivity
  - tests
  - Mic sample size
- calibration overlay
- bluetooth config
- Metrics overlay
5. Calibration overlay
- Left/right/up/down
6. Statititics Screen
- Songs listened to
- Minutes since last reset

#### LEDs
1. Init LEDS
2. Init audio
2. Loading animation
3. Load settings
5. Start 
 - Every loop, take x seconds of sample
 - Simple: Update LEDs
 - Complex: Transform into waveform, update LEDs
6. Update LEDs
 - Assign each band a waveform


### Loop
#### Init
#### CPU 1
#### CPU 2

## Mobile app
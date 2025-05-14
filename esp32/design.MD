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

### App Controller
#### State
- Active screen
 - subsription: lvgl.updateScreen
- Screen offset
  Subscription: storage.save
#### Actions
- updateOffset(hor, ver):
    offsetObservable.update(hor, ver)
    lvgl_screen_offset(lvgl_active_screen())
#### init
 - Load offset
 - Draw screen

### Calibration Screen
#### State
- previous offset
#### Components
- Calibration lines
- 4 arrow buttons
- Confirm
- Cancel
- Title
- Offset
  - subscribe: screenController.offset

#### Controller
- onDirectionPressed
 - appController.updateOffset
- onDonePressed(bool confirm)
 - if (!confirm):
    appController.updateOffset(previousOffset)

### Main Screen
#### State
- Song info: artist, song, time
- Bluetooth connection
- Performance metrics
  - Sample rate
  - Audio threshold
  - LED style
- Statistics
#### Componenets
- Song title
- Artist
- Song position
- Settings button
- Performance overlay
- Background
- LEDs?
- Connection status

### Loop
#### Init
#### CPU 1
#### CPU 2

## Mobile app
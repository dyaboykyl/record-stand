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
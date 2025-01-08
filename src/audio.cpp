#include "audio.h"

#include <Arduino.h>
#include <driver/adc.h>

void initAudio() {
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc2_config_channel_atten(ADC2_CHANNEL_5, ADC_ATTEN_DB_11);
  analogReadResolution(16);
}

void readAudio(int* value) { adc2_get_raw(ADC2_CHANNEL_5, ADC_WIDTH_BIT_12, value); }
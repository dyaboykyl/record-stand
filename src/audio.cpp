#include "audio.h"

#include <Arduino.h>
#include <EasyLogger.h>
#include <driver/adc.h>
#include <driver/i2s.h>

#define LOG_LEVEL LOG_LEVEL_NOTICE
#define TAG "Audio"
#define SOC_I2S_SUPPORTS_ADC true

#define SAMPLE_BUFFER_SIZE 1024
#define NOISE_SAMPLE_SIZE 2024
#define SAMPLE_RATE 44100
#define BYTE_TO_BINARY_PATTERN "%s%s%s%s%s%s%s%s"
#define BYTE_TO_BINARY(b)                                                           \
  ((b) & 0x80 ? "1" : "0"), ((b) & 0x40 ? "1" : "0"), ((b) & 0x20 ? "1" : "0"),     \
      ((b) & 0x10 ? "1" : "0"), ((b) & 0x08 ? "1" : "0"), ((b) & 0x04 ? "1" : "0"), \
      ((b) & 0x02 ? "1" : "0"), ((b) & 0x01 ? "1" : "0")

int noiseOffset = 0;

const float notchFreq = 100.0;  // Notch frequency in Hz
const float Q = 30.0;           // Quality factor

short a0, a1, a2, b1, b2;
float alpha;
short xp, xpp, yp, ypp;

i2s_config_t i2s_config = {.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
                           .sample_rate = 44000,
                           .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
                           .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
                           .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
                           //  |             I2S_COMM_FORMAT_STAND_MSB),
                           .intr_alloc_flags = 0,  // ESP_INTR_FLAG_LEVEL1,
                           .dma_buf_count = 8,
                           .dma_buf_len = SAMPLE_BUFFER_SIZE,
                           .use_apll = true,
                           .tx_desc_auto_clear = true,
                           .fixed_mclk = 0};
// i2s pins
i2s_pin_config_t i2sPins = {
    .bck_io_num = A1, .ws_io_num = SS, .data_out_num = I2S_PIN_NO_CHANGE, .data_in_num = MOSI};

AudioData i2sData;
int32_t* rawData;

void initAudio() {
  LOG_INFO(TAG, "Initializing audio");
  // adc1_config_width(ADC_WIDTH_BIT_12);
  // adc2_config_channel_atten(ADC2_CHANNEL_5, ADC_ATTEN_DB_11);
  // analogReadResolution(16);
  auto err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  LOG_INFO(TAG, (String) "Driver result: " + err + ". Setting I2S pins");
  i2s_set_pin(I2S_NUM_0, &i2sPins);
  i2sData = {.samples = (int16_t*)ps_malloc(sizeof(int16_t) * SAMPLE_BUFFER_SIZE), .size = 0};
  rawData = (int32_t*)ps_malloc(4 * SAMPLE_BUFFER_SIZE);
  LOG_INFO(TAG, "Audio initialized");
}

// start a task to read samples from I2S
TaskHandle_t readerTaskHandle;

void calculateNotchCoefficients() {
  float w0 = 2.0 * M_PI * notchFreq / SAMPLE_RATE;
  alpha = sin(w0) / (2.0 * Q);

  a0 = 1.0;
  a1 = -2.0 * cos(w0);
  a2 = 1.0;
  b1 = -2.0 * cos(w0);
  b2 = 1.0 - alpha;
}

short notchFilter(short sample) {
  short result = (a0 * sample + a1 * xp + a2 * xpp - b1 * yp - b2 * ypp) / (1.0 + alpha);
  xpp = xp;
  xp = sample;
  ypp = yp;
  yp = result;
  return result;
}

int16_t scale(int value) {
  return map(value, 0, noiseOffset * 2, -noiseOffset * 16, noiseOffset * 16);
  // -(noiseOffset * 16), noiseOffset * 16);
}

int audioValue = 0;
int16_t readAudio(bool shouldScale, bool notchFilter) {
  adc2_get_raw(ADC2_CHANNEL_5, ADC_WIDTH_BIT_12, &audioValue);
  // return audioValue;
  return shouldScale ? scale(audioValue) : audioValue;
}

AudioData* readI2sAudio() {
  size_t bytes_read = 0;
  i2s_read(I2S_NUM_0, rawData, 4 * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
  int samples_read = bytes_read / 4;
  if (samples_read != SAMPLE_BUFFER_SIZE) {
    LOG_ERROR(TAG, (String) "Read " + samples_read + " samples instead of " + SAMPLE_BUFFER_SIZE);
  }
  for (int i = 0; i < samples_read; i++) {
    i2sData.samples[i] = ((float)(rawData[i] >> 8) / 0x7fffff) * INT16_MAX;
  }
  i2sData.size = samples_read;

  return &i2sData;
}

void calculateNoiseOffset() {
  LOG_INFO(TAG, "Calculating noise offset");
  int sum = 0;
  for (int i = 0; i < NOISE_SAMPLE_SIZE; i++) {
    sum += readAudio(false, false);
  }
  noiseOffset = sum / NOISE_SAMPLE_SIZE;
  LOG_INFO(TAG, (String) "Noise offset: " + noiseOffset);
}

int getNoiseOffset() { return noiseOffset; }
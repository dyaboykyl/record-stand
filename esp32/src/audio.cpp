#include "audio.h"

#include <Arduino.h>
#include <arduinoFFT.h>
#include <driver/adc.h>
#include <driver/i2s.h>

#define LOG_LEVEL LOG_LEVEL_NOTICE
#define LABEL "Audio"
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
                           .sample_rate = SAMPLE_RATE,
                           .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
                           .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
                           .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
                           //  |             I2S_COMM_FORMAT_STAND_MSB),
                           .intr_alloc_flags = 0,  // ESP_INTR_FLAG_LEVEL1,
                           .dma_buf_count = 8,
                           .dma_buf_len = SAMPLE_BUFFER_SIZE,
                           .use_apll = false,
                           .tx_desc_auto_clear = true,
                           .fixed_mclk = 0};

i2s_pin_config_t i2sPins = {
    .bck_io_num = A0,
    .ws_io_num = A1,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = A1  //
};

AudioData audioData;
int32_t* rawData;

void initAudio() {
  ESP_LOGI(LABEL, "Initializing audio");
  audioData = {.samples = (int16_t*)ps_malloc(sizeof(int16_t) * SAMPLE_BUFFER_SIZE), .size = 0};
  ESP_LOGI(LABEL, "Audio initialized");
}

void initI2s() {
  ESP_LOGI(LABEL, "Initializing I2S");
  auto err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  ESP_LOGI(LABEL, "Driver result: %d. Setting I2S pins", err);
  err= i2s_set_pin(I2S_NUM_0, &i2sPins);
  ESP_LOGI(LABEL, "Pins result: %d", err);
  rawData = (int32_t*)ps_malloc(4 * SAMPLE_BUFFER_SIZE);
  ESP_LOGI(LABEL, "I2S initialized");
}

void stopI2s() {
  i2s_stop(I2S_NUM_0);
  i2s_driver_uninstall(I2S_NUM_0);
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

AudioData *readAnalogAudio() {
  audioData.size = 1;
  audioData.samples[0] = abs(analogRead(A0) - 2000);
  return &audioData;
}

AudioData* readI2sAudio() {
  size_t bytes_read = 0;
  auto now = millis();
  // ESP_LOGD(LABEL, "[readI2sAudio] start");
  auto err = i2s_read(I2S_NUM_0, rawData, 4 * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
  if (err != ESP_OK) {
    ESP_LOGE(LABEL, "Error reading I2S: %d", err);
    return NULL;
  }
  int samples_read = bytes_read / 4;
  if (samples_read != SAMPLE_BUFFER_SIZE) {
    // LOG_ERROR(LABEL, "Read " << samples_read << " samples instead of " << SAMPLE_BUFFER_SIZE);
  }
  for (int i = 0; i < samples_read; i++) {
    audioData.samples[i] = ((float)(rawData[i] >> 8) / 0x7fffff) * INT16_MAX;
  }
  audioData.size = samples_read;

  // ESP_LOGD(LABEL, "[readI2sAudio] end. millis=%d", millis() - now);
  return &audioData;
}

void calculateAudioBandIntensities() {}

void calculateNoiseOffset() {
  ESP_LOGI(LABEL, "Calculating noise offset");
  int sum = 0;
  for (int i = 0; i < NOISE_SAMPLE_SIZE; i++) {
    sum += readAudio(false, false);
  }
  noiseOffset = sum / NOISE_SAMPLE_SIZE;
  ESP_LOGI(LABEL, "Noise offset: %d", noiseOffset);
}

void plotAudio() {
  Serial.print(">A0:");
  Serial.println(analogRead(A0));
  Serial.print(">Scaled:");
  Serial.println(readAnalogAudio()->samples[0]);
    // auto audioData = readI2sAudio();
    // for (int i = 0; i < audioData->size; i++) {
    //   Serial.print(">Raw:");
    //   Serial.println(audioData->samples[i]);
    //     Serial.print(">Size:");
    //     Serial.println(audioData->size);
    // }
}

int getNoiseOffset() { return noiseOffset; }

#include <Arduino.h>
#include <EasyLogger.h>
#include <wav.h>

#include "audio.h"

#define LABEL "WAV"

#define MAX_WAV_SAMPLES (44100 * 10)
#define WAV_HEADER_SIZE 44
#define WAV_RIFF_OFFSET 8

#define LOG_LEVEL LOG_LEVEL_NOTICE

static int maxWavFileSize = WAV_HEADER_SIZE + sizeof(int16_t) * MAX_WAV_SAMPLES;
static int16_t *wavData;
static int16_t *wavSampleData;

wav_header_t WAV_HEADER_TEMPLATE;
unsigned int currentSampleIndex;

struct Filter {
  float alpha = 0;
  short prev = 0;
  short avg = 0;
  Filter(float a) : alpha(a), avg(0) {}
};

short runLowPass(Filter filter, short value) {
  filter.prev = filter.alpha * value + (1 - filter.alpha) * filter.prev;
  return filter.prev;
}

short runHighPass(Filter filter, short value) {
  filter.prev = value - filter.alpha * value + filter.alpha * filter.prev;
  return filter.prev;
}

bool allocateWavSpace() {
  LOG_INFO("WAV", "Allocating space");
  wavData = (int16_t *)ps_malloc(maxWavFileSize);
  wavSampleData = (int16_t *)((char *)wavData + WAV_HEADER_SIZE);
  LOG_INFO("WAV", "Max sample index: " << (maxWavFileSize / sizeof(int16_t)));
  return true;
}

void recordWavAtRate(int rate) {
  startNewWav();

  unsigned long start = millis();
  unsigned long last = micros();
  unsigned long current = micros();
  float microsPerSample = 1000000 / rate;
  int value;

  do {
    current = micros();
    if ((current - last) >= microsPerSample) {
      addWavSample(readAudio());
      last = current;
    }
  } while (!wavFilled());
  int totalMs = (last / 1000) - start;

  for (int i = 0; i < currentSampleIndex; i++) {
    wavSampleData[i] = map(wavSampleData[i], 0, 4096, 0, 65536);
  }

  LOG_INFO("WAV", "Finished writing WAV file");
  finishWav(totalMs);
}

void recordWavFromI2S() {
  startNewWav();

  int value = 0;
  auto low = Filter(.2);
  auto high = Filter(.9);

  // flush initial samples
  LOG_INFO(LABEL, "Flushing initial samples");
  for (int i = 0; i < 100; i++) {
    readI2sAudio();
  }

  LOG_INFO(LABEL, "Recording");
  unsigned long start = millis();
  while (!wavFilled()) {
    auto audioData = readI2sAudio();
    for (int i = 0; i < audioData->size && !wavFilled(); i++) {
      addWavSample(audioData->samples[i]);
      // addWavSample(runHighPass(high, runLowPass(low, audioData->samples[i])) * 32);
    }
  }
  int totalMs = millis() - start;

  LOG_INFO("WAV", "Finished writing WAV file");
  finishWav(totalMs);
}

void recordWavMaxRate() {
  startNewWav();

  unsigned long start = millis();
  unsigned long currentMs = start;
  int value = 0;
  while (!wavFilled()) {
    addWavSample(readAudio());
  }
  int totalMs = millis() - start;

  LOG_INFO("WAV", "Finished writing WAV file");
  finishWav(totalMs);
}

void startNewWav() {
  LOG_INFO("WAV", "Starting new WAV");
  memcpy(wavData, &WAV_HEADER_TEMPLATE, WAV_HEADER_SIZE);
  wavSampleData = (int16_t *)((char *)wavData + WAV_HEADER_SIZE);
  currentSampleIndex = 0;
}

void addWavSample(int16_t sample) {
  if (!wavFilled()) {
    // //LOG_INFO("WAV", "Current sample index: " << currentSampleIndex);
    wavSampleData[currentSampleIndex++] = sample;
  }
}

inline bool wavFilled() { return !(currentSampleIndex < MAX_WAV_SAMPLES); }

void finishWav(int totalMs) {
  currentSampleIndex--;
  wav_header_t *currentHeader = (wav_header_t *)wavData;
  currentHeader->data_bytes = sizeof(int16_t) * currentSampleIndex;
  currentHeader->wav_size = WAV_HEADER_SIZE + currentHeader->data_bytes - WAV_RIFF_OFFSET;
  currentHeader->sample_rate = (currentSampleIndex + 1) * 1000 / totalMs;
  currentHeader->byte_rate = currentHeader->sample_rate * 2;
  LOG_INFO(LABEL, "totalMs: " << totalMs << " sampleCount: " << currentSampleIndex
                              << " dataBytes: " << currentHeader->data_bytes
                              << " wavSize: " << currentHeader->wav_size
                              << " sampleRate: " << currentHeader->sample_rate
                              << " byteRate: " << currentHeader->byte_rate);

  uint64_t sum = 0;
  for (int i = 0; i <= currentSampleIndex; i++) {
    sum += wavData[currentSampleIndex];
  }
  uint16_t average = sum / currentSampleIndex;
  LOG_INFO(LABEL, "Sample statistics: sum=" << sum << " avg=" << average);
  //  writeWavFileToSerial(WAV_HEADER_SIZE);
  //  Serial.println();
}

void writeWavFileToSerial(int size) {
  const uint8_t *wavBytes = (const uint8_t *)wavData;
  wav_header_t *currentHeader = (wav_header_t *)wavData;
  int sizeToWrite = size < 0 ? currentHeader->wav_size + WAV_RIFF_OFFSET : size;
  Serial.println("WAV_FILE");
  delay(500);
  for (int i = 0; i < sizeToWrite; i++) {
    if (wavBytes[i] <= 15) Serial.print("0");
    Serial.print(wavBytes[i], HEX);
    Serial.print(" ");
  }
  delay(500);
  Serial.println("WAV_FILE");
}

uint8_t *getWavData() { return (uint8_t *)wavData; }

int getWavFileSize() { return ((wav_header_t *)wavData)->wav_size + WAV_RIFF_OFFSET; }

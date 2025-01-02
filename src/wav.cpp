
#include <EasyLogger.h>
#include <wav.h>

#define MAX_WAV_SAMPLES (25000 * 10)
#define WAV_HEADER_SIZE 44
#define WAV_RIFF_OFFSET 8

#define LOG_LEVEL LOG_LEVEL_NOTICE

static int maxWavFileSize = WAV_HEADER_SIZE + sizeof(int16_t) * MAX_WAV_SAMPLES;
static int16_t *wavData;
static int16_t *wavSampleData;

wav_header_t WAV_HEADER_TEMPLATE;
unsigned int currentSampleIndex;

bool allocateWavSpace() {
    LOG_INFO("WAV", "Allocating space");
    wavData = (int16_t *)ps_malloc(maxWavFileSize);
    wavSampleData = (int16_t *)((char *)wavData + WAV_HEADER_SIZE);
    LOG_INFO("WAV", (String) "Max sample index: " + (maxWavFileSize / sizeof(int16_t)));
    return true;
}

void startNewWav() {
    LOG_INFO("WAV", "Starting new WAV");
    memcpy(wavData, &WAV_HEADER_TEMPLATE, WAV_HEADER_SIZE);
    wavSampleData = (int16_t *)((char *)wavData + WAV_HEADER_SIZE);
    currentSampleIndex = 0;
}

void addWavSample(int16_t sample) {
    if (!wavFilled()) {
        // LOG_INFO("WAV", (String) "Current sample index: " + currentSampleIndex);
        wavSampleData[currentSampleIndex++] = sample;
    }
}

bool wavFilled() { return !(currentSampleIndex < MAX_WAV_SAMPLES); }

void finishWav(int totalMs) {
    currentSampleIndex--;
    wav_header_t *currentHeader = (wav_header_t *)wavData;
    currentHeader->data_bytes = sizeof(int16_t) * currentSampleIndex;
    currentHeader->wav_size = WAV_HEADER_SIZE + currentHeader->data_bytes - WAV_RIFF_OFFSET;
    currentHeader->sample_rate = currentSampleIndex * 1000 / totalMs;
    currentHeader->byte_rate = currentHeader->sample_rate * 2;
    LOG_INFO("WAV", (String) "totalMs: " + totalMs + " sampleCount: " + currentSampleIndex +
                        " dataBytes: " + currentHeader->data_bytes + " wavSize: " +
                        currentHeader->wav_size + " sampleRate: " + currentHeader->sample_rate +
                        " byteRate: " + currentHeader->byte_rate);

    uint64_t sum = 0;
    for (int i = 0; i <= currentSampleIndex; i++) {
        sum += wavData[currentSampleIndex];
    }
    uint16_t average = sum / currentSampleIndex;
    LOG_INFO("WAV", (String) "Sample statistics: sum=" + sum + " avg=" + average);
    // writeWavFileToSerial(WAV_HEADER_SIZE);
    // Serial.println();
}

void writeWavFileToSerial(int size) {
    const uint8_t *wavBytes = (const uint8_t *)wavData;
    wav_header_t *currentHeader = (wav_header_t *)wavData;
    int sizeToWrite = size < 0 ? currentHeader->wav_size + WAV_RIFF_OFFSET : size;
    Serial.println("WAV_FILE");
    delay(500);
    for (int i = 0; i < sizeToWrite; i++) {
        if (wavBytes[i] <= 15) Serial << "0";
        Serial.print(wavBytes[i], HEX);
        Serial.print(" ");
    }
    delay(500);
    Serial.println("WAV_FILE");
}

uint8_t *getWavData() { return (uint8_t *)wavData; }

int getWavFileSize() { return ((wav_header_t *)wavData)->wav_size + WAV_RIFF_OFFSET; }

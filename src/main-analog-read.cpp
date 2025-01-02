
#include <Arduino.h>
#include <driver/adc.h>

void setup() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc2_config_channel_atten(ADC2_CHANNEL_5, ADC_ATTEN_DB_11);
    // analogReadResolution(16);
}

struct Filter {
    float alpha = 0;
    int avg = 0;
    Filter(float a) : alpha(a), avg(0) {}
};

void runFilter(Filter filter, int value) {
    filter.avg = filter.avg * filter.alpha + (1 - filter.alpha) * value;
    Serial.print((String) ">" + filter.alpha + ":");
    Serial.println(value - filter.avg);
}

Filter filters[] = {
    Filter(.5), Filter(.99), Filter(.75), Filter(.25), Filter(.05),
};

void loop() {
    int offset = 2048 * .92;  // 30300;
    int value = 0;
    unsigned long sum = 0;
    // for (int i = 0; i < 100; i++) {
    //     sum += analogRead(A1);
    //     delay(1);
    // }
    adc2_get_raw(ADC2_CHANNEL_5, ADC_WIDTH_BIT_12, &value);
    value -= offset;
    for (int i = 0; i < 5; i++) {
        runFilter(filters[i], value);
    }
    Serial.print(">Raw:");
    Serial.println(value);
    // Serial.print(">0:");
    // Serial.println(20000 - offset);
    // Serial.print(">Max:");
    // Serial.println(40000 - offset);
    // delay(5);
}

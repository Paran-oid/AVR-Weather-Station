#ifndef DHT22_H
#define DHT22_H

#include <avr/io.h>

struct dht22_temp {
    uint8_t hum_high, hum_low, temp_high, temp_low, checksum;
};

void dht22_read(struct dht22_temp* d);

float dht22_get_temp(struct dht22_temp* d);
float dht22_get_humidity(struct dht22_temp* d);

void dht22_temperature_extract(char buf[], struct dht22_temp* d);
void dht22_humidity_extract(char buf[], struct dht22_temp* d);

#endif

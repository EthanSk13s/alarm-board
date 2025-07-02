// Note: Implementation is based off the C++ implementation
// by Adafruit: https://github.com/adafruit/Adafruit_VEML7700
#ifndef VEML7700_PIGPIO_H
#include <stdint.h>
#include <pigpio.h>

#define VEML7700_PIGPIO_H

#define VEML7700_I2C_ADDR     0x10
#define VEML7700_CMD_BIT      0x80

#define VEML7700_ALS_CONFIG   0x00
#define VEML7700_ALS_POWER    0x03
#define VEML7700_ALS_DATA     0x04
#define VEML7700_WHITE_DATA   0x05

#define VEML7700_GAIN_1X      0x00
#define VEML7700_GAIN_2X      0x01
#define VEML7700_GAIN_1_8X    0x02
#define VEML7700_GAIN_1_4X    0x03

#define VEML7700_INT_TIME_25  0x0C
#define VEML7700_INT_TIME_50  0x08
#define VEML7700_INT_TIME_100 0x00
#define VEML7700_INT_TIME_200 0x01
#define VEML7700_INT_TIME_400 0x02
#define VEML7700_INT_TIME_800 0x03

#define VEML7700_PERS_1       0x00
#define VEML7700_PERS_2       0x01
#define VEML7700_PERS_4       0x02
#define VEML7700_PERS_8       0x03

#define VEML7700_PSM_1        0x00
#define VEML7700_PSM_2        0x01
#define VEML7700_PSM_3        0x02
#define VEML7700_PSM_4        0x03

int VEML7700_setup();

void VEML7700_set_persistence(uint8_t mode);
void VEML7700_set_gain(uint8_t gain);
void VEML7700_set_integ_time(uint8_t integ_time);
void VEML7700_enable_interrupt(uint8_t toggle);
void VEML7700_set_power_save(uint8_t mode);

float VEML7700_get_gain(uint8_t gain);
int VEML7700_get_integ_time(uint16_t integ_time);

void VEML7700_close();

float VEML7700_read_lux(int correction);

#endif
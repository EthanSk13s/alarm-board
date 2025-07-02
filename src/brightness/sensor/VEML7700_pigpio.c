#include "VEML7700_pigpio.h"
#include <pigpio.h>
#include <stdint.h>

typedef struct
{
    int fd;
    uint8_t gain;
    uint16_t integ_time;
} SensorInfo;

static SensorInfo sensor_info = { };

static uint8_t mask_bits(int* val, uint8_t data, int num_of_bits, int shifts)
{
    int mask = (1 << (num_of_bits)) - 1;
    data &= mask;

    mask <<= shifts;
    *val &= ~mask;
    *val |= data << shifts;

    return *val;
}

static int i2c_write(uint8_t reg, int value)
{
    // VEML7700 takes writes in reversed order as per:
    // https://www.vishay.com/docs/84286/veml7700.pdf

    value = ((value * 0x0802LU & 0x22110LU) |
            (value * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
    return i2cWriteWordData(sensor_info.fd, reg, value);
}

static int i2c_read(uint8_t reg)
{
    return i2cReadWordData(sensor_info.fd, reg);
}

static int i2c_read_word(uint8_t reg)
{
    return i2cReadWordData(sensor_info.fd, reg);
}

static void toggle_power(uint8_t value)
{
    int val = i2c_read_word(VEML7700_ALS_CONFIG);
    val = mask_bits(&val, value, 1, 0);

    i2c_write(VEML7700_ALS_CONFIG, val);
}

int VEML7700_setup()
{
    int fd = i2cOpen(1, VEML7700_I2C_ADDR, 0);
    if (fd < 0)
    {
        return fd;
    }

    sensor_info.fd = fd;
    toggle_power(0);
    VEML7700_set_persistence(VEML7700_PERS_1);
    VEML7700_set_gain(VEML7700_GAIN_1_8X);
    VEML7700_set_integ_time(VEML7700_INT_TIME_100);
    VEML7700_enable_interrupt(0);
    toggle_power(1);

    return 0;
}

void VEML7700_close()
{
    toggle_power(0);

    i2cClose(sensor_info.fd);
}

void VEML7700_set_persistence(uint8_t mode)
{
    int val = i2c_read_word(VEML7700_ALS_CONFIG);
    val = mask_bits(&val, mode, 2, 4);

    i2c_write(VEML7700_ALS_CONFIG, val);
}

void VEML7700_set_gain(uint8_t gain)
{
    sensor_info.gain = gain;
    int val = i2c_read_word(VEML7700_ALS_CONFIG);
    val = mask_bits(&val, gain, 2, 11);

    i2c_write(VEML7700_ALS_CONFIG, val);
}

void VEML7700_set_integ_time(uint8_t integ_time)
{
    sensor_info.integ_time = integ_time;
    int val = i2c_read_word(VEML7700_ALS_CONFIG);
    val = mask_bits(&val, integ_time, 4, 6);

    i2c_write(VEML7700_ALS_CONFIG, val);
}

void VEML7700_enable_interrupt(uint8_t toggle)
{
    int val = i2c_read_word(VEML7700_ALS_CONFIG);
    val = mask_bits(&val,toggle, 1, 1);

    i2c_write(VEML7700_ALS_CONFIG, val);
}

int VEML7700_get_integ_time(uint16_t integ_time)
{
    switch(integ_time)
    {
        case VEML7700_INT_TIME_25:
            return 25;
        case VEML7700_INT_TIME_50:
            return 50;
        case VEML7700_INT_TIME_100:
            return 100;
        case VEML7700_INT_TIME_200:
            return 200;
        case VEML7700_INT_TIME_400:
            return 400;
        case VEML7700_INT_TIME_800:
            return 800;
        default:
            return -1;
    }
}

float VEML7700_get_gain(uint8_t gain)
{
    switch(gain)
    {
        case VEML7700_GAIN_1_8X:
            return 0.125;
        case VEML7700_GAIN_1_4X:
            return 0.25;
        case VEML7700_GAIN_1X:
            return 1;
        case VEML7700_GAIN_2X:
            return 2;
        default:
            return -1;
    }
}

float VEML7700_read_lux(int correction)
{
    int integ_time = VEML7700_get_integ_time(sensor_info.integ_time);
    float gain = VEML7700_get_gain(sensor_info.gain);
    if (integ_time == -1 || gain == -1)
    {
        return -1;
    }

    float resolution = 0.0036 * (800 / (float) integ_time) * (2 / gain);
    int lux = i2c_read_word(VEML7700_ALS_DATA) * resolution;

    if (correction)
    {
        // Correction Equation from: https://www.vishay.com/docs/84323/designingveml7700.pdf
        // a * (lux)^4 + b * (lux)^3 + c * (lux)^2 + d * lux
        float a = 6.0135e-13;
        float b = -9.3924e-9;
        float c = 8.1488e-5;
        float d = 1.0023;

        // Horner's method applied.
        return (((a * lux + b) * lux + c) * lux + d) * lux; 
    }

    return lux;
}
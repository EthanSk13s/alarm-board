#include "VEML7700_pigpio.h"
#include <pigpio.h>
#include <stdint.h>
#include <stdio.h>

static int fd;

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
    return i2cWriteWordData(fd, reg, value);
}

static int i2c_read(uint8_t reg)
{
    return i2cReadWordData(fd, reg);
}

static int i2c_read_word(uint8_t reg)
{
    return i2cReadWordData(fd, reg);
}

static void toggle_power(uint8_t value)
{
    int val = i2c_read_word(VEML7700_ALS_CONFIG);
    val = mask_bits(&val, value, 1, 0);

    i2c_write(VEML7700_ALS_CONFIG, val);
}

int VEML7700_setup()
{
    fd = i2cOpen(1, VEML7700_I2C_ADDR, 0);
    if (fd < 0)
    {
        return fd;
    }

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

    i2cClose(fd);
}

void VEML7700_set_persistence(uint8_t mode)
{
    int val = i2c_read_word(VEML7700_ALS_CONFIG);
    val = mask_bits(&val, mode, 2, 4);

    i2c_write(VEML7700_ALS_CONFIG, val);
}

void VEML7700_set_gain(uint8_t gain)
{
    int val = i2c_read_word(VEML7700_ALS_CONFIG);
    val = mask_bits(&val, gain, 2, 11);

    i2c_write(VEML7700_ALS_CONFIG, val);
}

void VEML7700_set_integ_time(uint8_t integ_time)
{
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

float VEML7700_read_lux()
{
    float resolution = 0.0036 * (800 / 100) * (2 / 0.125);
    int als = i2c_read_word(VEML7700_ALS_DATA);
    gpioDelay(100000);
    printf("ALS: %d\n", als);

    return resolution * als;
}
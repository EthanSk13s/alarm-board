#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "brightness_ctrl.h"
#include "sensor/VEML7700_pigpio.h"

// NOTE: This assumes you're using the Raspberry Pi Touch Display 2.
// TODO(?): Make this more portable among monitors. 
#define BRIGHTNESS_CTRL_FILE "/sys/class/backlight/10-0045/brightness"
#define MAX_BRIGHTNESS_FILE  "/sys/class/backlight/10-0045/max_brightness"

static int MAX_BRIGHTNESS;
static int LOWER_THRESHOLD = 5;
static int current_brightness;

static volatile int kill_thread = 0;

static int init_brightness_ctrl()
{
    FILE* max_brightness_file = fopen(MAX_BRIGHTNESS_FILE, "r");
    if (max_brightness_file == NULL)
    {
        return -1;
    }

    char buffer[10];
    fgets(buffer, 3, max_brightness_file);
    int result = atoi(buffer);

    if (result == 0)
    {
        return -1;
    }

    MAX_BRIGHTNESS = result;
    fclose(max_brightness_file);

    FILE* brightness_file = fopen(BRIGHTNESS_CTRL_FILE, "r");
    if (brightness_file == NULL)
    {
        return -1;
    }

    fgets(buffer + 3, 3, brightness_file);
    result = atoi(buffer + 3);

    if (result == 0)
    {
        return -1;
    }

    current_brightness = result;

    return result;
}

static void transition_brightness(int target, FILE* brightness_file)
{
    int op = 1;
    if (target < current_brightness)
    {
        op = -1;
    }

    char write_buffer[5];
    while (target != current_brightness)
    {
        current_brightness += op;
        sprintf(write_buffer, "%d", current_brightness);
        fputs(write_buffer, brightness_file);
        fseek(brightness_file, 0, SEEK_SET);

        gpioSleep(PI_TIME_RELATIVE, 0, 50000);
    }
}

void brightness_kill_thread()
{
    kill_thread = 1;
}

// Let's assume the sensor has already been setup at this point.
void* brightness_ctrl_thread()
{
    int result = init_brightness_ctrl();

    if (result == -1)
    {
        printf("Error: Could not open backlight control files.\n");
        return NULL;
    }

    FILE* brightness_file = fopen(BRIGHTNESS_CTRL_FILE, "r+");
    if (brightness_file == NULL)
    {
        printf("Error: You do not have permission to modify the backlight.\n");
        return NULL;
    }

    float value = VEML7700_read_lux(1);
    while (!kill_thread)
    {
        value = VEML7700_read_lux(1);
        if (value > 1300)
        {
            int target = MAX_BRIGHTNESS;
            transition_brightness(target, brightness_file);
        } else if (value < 600)
        {
            int target = LOWER_THRESHOLD;
            transition_brightness(target, brightness_file);
        }

        gpioSleep(PI_TIME_RELATIVE, 0, 250000);
    }

    fclose(brightness_file);
    return NULL;
}
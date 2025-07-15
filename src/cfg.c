#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cfg.h"
#include "ds/hash_table.h"
#include "info_storage.h"

#define MAX_KEY_VAL_SIZE 100
#define DELIMITER "="

int cfg_parse(ht* cfg_map, char* file_ptr)
{
    if (cfg_map == NULL || file_ptr == NULL)
    {
        return -1;
    }

    FILE* cfg_file = fopen(file_ptr, "r");
    if (cfg_file == NULL)
    {
        cfg_file = fopen(file_ptr, "w");
        if (cfg_file == NULL)
        {
            return -1;
        }

        fputs("ALARM=00:00", cfg_file);
        fclose(cfg_file);
        cfg_file = fopen(file_ptr, "r");
        if (cfg_file == NULL)
        {
            return -1;
        }
    }

    char val[MAX_KEY_VAL_SIZE];
    char* line = fgets(val, MAX_KEY_VAL_SIZE, cfg_file);
    while(line)
    {
        char* rest = val;
        char* key = strtok_r(rest, DELIMITER, &rest);
        char* value = strtok_r(rest, DELIMITER, &rest);
        if (key == NULL || value == NULL)
        {
            continue;
        }

        char* val_dup = strndup(value, strlen(value));
        ht_set(cfg_map, key, val_dup);

        line = fgets(val, MAX_KEY_VAL_SIZE, cfg_file);
    }

    return 0;
}

int cfg_load(ht* cfg_map)
{
    char* alarm_str = (char*) ht_get(cfg_map, "ALARM");
    if (alarm_str == NULL)
    {
        return -1;
    }
    char buf[MAX_KEY_VAL_SIZE];

    char* rest = alarm_str;
    char* hour_str = strtok_r(rest, ":", &rest);
    char* min_str = strtok_r(rest, ":", &rest);

    if (hour_str == NULL || min_str == NULL)
    {
        return -1;
    }

    set_alarm_hour(atoi(hour_str));
    set_alarm_min(atoi(min_str));

    return 0;
}

int cfg_save(char* file_ptr)
{
    FILE* cfg_file = fopen(file_ptr, "w");
    if (cfg_file == NULL)
    {
        return -1;
    }

    // TODO: Instead of using info storage, use the hash table.
    char buf[MAX_KEY_VAL_SIZE];

    struct tm alarm = get_alarm();
    sprintf(buf, "ALARM=%d:%d", alarm.tm_hour, alarm.tm_min);
    fputs(buf, cfg_file);
    fclose(cfg_file);
}
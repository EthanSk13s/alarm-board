#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cfg.h"
#include "ds/hash_table.h"
#include "info_storage.h"

#define MAX_KEY_VAL_SIZE 100
#define MAX_CONFIG_BUFFER 1000
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

        fputs("ALARM=00:00\n", cfg_file);
        fputs("API_KEY=<INSERT_KEY_HERE>\n", cfg_file);
        fputs("LATITUTDE=34.05\n", cfg_file);
        fputs("LONGITUDE=118.24", cfg_file);
        fclose(cfg_file);
        cfg_file = fopen(file_ptr, "r");
        if (cfg_file == NULL)
        {
            return -1;
        }
    }

    char val[MAX_KEY_VAL_SIZE];
    //char* line = fgets(val, MAX_KEY_VAL_SIZE, cfg_file);
    while(fgets(val, MAX_KEY_VAL_SIZE, cfg_file))
    {
        if (strlen(val) == 1 || strlen(val) == 0)
        {
            continue;
        }

        char* rest = val;
        char* key = strtok_r(rest, DELIMITER, &rest);
        char* value = strtok_r(rest, DELIMITER, &rest);
        if (key == NULL || value == NULL)
        {
            continue;
        }

        char* val_dup = strndup(value, strlen(value));
        ht_set(cfg_map, key, val_dup);

        //line = fgets(val, MAX_KEY_VAL_SIZE, cfg_file);
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

int cfg_save(ht* cfg_map, char* file_ptr)
{
    FILE* cfg_file = fopen(file_ptr, "w");
    if (cfg_file == NULL)
    {
        return -1;
    }

    // TODO: Instead of using info storage, use the hash table.
    char buf[MAX_CONFIG_BUFFER];
    int buf_pos = 0;

    struct tm alarm = get_alarm();
    char* api_key = ht_get(cfg_map, "API_KEY");
    char* long_str = ht_get(cfg_map, "LONGITUDE");
    char* lat_str = ht_get(cfg_map, "LATITUDE");

    buf_pos += sprintf(buf + buf_pos, "ALARM=%d:%d\n", alarm.tm_hour, alarm.tm_min);
    buf_pos += sprintf(buf + buf_pos, "API_KEY=%s", api_key);
    buf_pos += sprintf(buf + buf_pos, "LONGITUDE=%s", long_str);
    buf_pos += sprintf(buf + buf_pos, "LATITUDE=%s", lat_str);

    fputs(buf, cfg_file);
    fclose(cfg_file);

    return 0;
}
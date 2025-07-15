#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "ds/hash_table.h"

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
        // TODO: Create config file and load default values
    }

    char val[MAX_KEY_VAL_SIZE];
    char* line = fgets(val, MAX_KEY_VAL_SIZE, cfg_file);
    while(line)
    {
        char* token;
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
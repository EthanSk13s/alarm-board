#ifndef CFG_H
#define CFG_H
/*
 * Note: The config structure is very simple.
 * It is a key value pair delimited by an equal sign
 * (i.e. foo=bar). Every value will be parsed as a string, and it is up
 * to the user to parse the value.
 */
#include "ds/hash_table.h"

int cfg_parse(ht* cfg_map, char* file_path);
int cfg_load(ht* cfg_map);
int cfg_save(char* file_ptr);
#endif
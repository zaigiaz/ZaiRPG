#ifndef DATA_H
#define DATA_H

// defines for different data that we can parse with our JSON system
#define MAX_UNIT_SIZE 8
#include "declares.h"

// Class and Subclass Data
typedef struct {
  char *name;  
} JSON_Class;

// Unit consists of multiple characters
typedef struct {
  char* name;
  int player_unit[MAX_UNIT_SIZE];
} JSON_Unit;

// read files and spit out contents as a string
// have this done as mmap in the future
s8 *slurp(const char* name);
void parse_json_file(const s8* json);

#endif 

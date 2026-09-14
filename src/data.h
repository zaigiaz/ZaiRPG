#ifndef DATA_H
#define DATA_H

// defines for different data that we can parse with our JSON system
#define MAX_UNIT_SIZE 8
#include "declares.h"

// read files and spit out contents as a string
// have this done as mmap in the future
s8 *slurp(const char* name);

// parse move into move struct from json
void parse_json_move(const s8* json);

#endif 

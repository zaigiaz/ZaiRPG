#include <stdio.h>
#include "data.h"
#include "json.h"
#include "declares.h"

// TODO :: create Object ID system for game

typedef enum {
  ENTITY,
  MAP,
  OBJECT,
  CLASS
} data_type;

// slurp up file contents
// TODO :: change this to s8 so I can retain length of buffer when I pass back?
u8* slurp(char *name) 
{
  FILE *fp;
  fp = fopen(name, "r");

  fseek(fp, 0L, SEEK_END);
  i64 res = ftell(fp);
  rewind(fp);
  u8 *buffer = malloc(res);

  int c, count=0;
  while((c = fgetc(fp)) != EOF) {
    buffer[count++] = c;
  }
  fclose(fp);
  return buffer;
}

// slurp up the json file and read it and parse to specific part
// TODO :: get basic json format for parsing save characters / entities
void read_json_file(const char* json) {
  struct json_value_s* root = json_parse(json, strlen(json));
  return;
}

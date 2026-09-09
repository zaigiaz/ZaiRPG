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
s8* slurp(const char *name) 
{
  FILE *fp;
  fp = fopen(name, "r");

  fseek(fp, 0L, SEEK_END);
  i64 buf_size = ftell(fp);
  rewind(fp);

  s8 *string = malloc(sizeof(s8));
  string->data = malloc(buf_size);
  string->len = buf_size;

  int c, count=0;
  while((c = fgetc(fp)) != EOF) {
    string->data[count++] = c;
  }
  fclose(fp);
  return string;
}

// slurp up the json file and read it and parse to specific part
// TODO :: get basic json format for parsing save characters / entities
void parse_json_file(const s8* json) {
  struct json_value_s* root = json_parse(json->data, json->len);

  return;
}

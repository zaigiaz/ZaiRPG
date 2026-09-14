#include <stdio.h>
#include "data.h"
#include "json.h"
#include <assert.h>
#include "declares.h"

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

// parse json for move objects
// EXAMPLE :: { "name": "Fire Breath", "Damage": 25 }
void parse_json_move(const s8* json) {

  struct json_value_s* root = json_parse(json->data, json->len);
  assert(root->type == json_type_object);
  
  struct json_object_s* object = (struct json_object_s*)root->payload;
  struct json_object_element_s* a = object->start;

  struct json_string_s* a_name = a->name;
  assert(0 == strcmp(a_name->string, "items"));
 
  struct json_value_s* a_value = a->value;
  assert(a_value->type == json_type_array);
 
  struct json_array_s* array = (struct json_array_s*)a_value->payload;
  assert(array->length == 2);
  
  printf("\n the length of json object is %zu \n", object->length);
  free(root);
}

#include <stdio.h>
#include <assert.h>
#include "includes/json.h"
#include "primary.h"
#include "declares.h"
#include "data.h"

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
// EXAMPLE :: { "name": "Fire Breath", "move_type": "MOVE_FLAME", "Damage": 25 }
void parse_json_move(const s8* json) {
    struct json_value_s* root = json_parse(json->data, json->len);
    assert(root != NULL);
    assert(root->type == json_type_object);

    struct json_object_s* object =
        (struct json_object_s*)root->payload;

    struct json_object_element_s* a = object->start;
    assert(a != NULL);

    assert(strcmp(a->name->string, "items") == 0);
    assert(a->value->type == json_type_array);

    struct json_array_s* array =
        (struct json_array_s*)a->value->payload;

    for (struct json_array_element_s* elem = array->start;
         elem != NULL;
         elem = elem->next) {

        assert(elem->value->type == json_type_object);

        // Use the current array element, not the outer object.
        struct json_object_s* item =
            (struct json_object_s*)elem->value->payload;

        struct json_object_element_s* field = item->start;

        const char* move_name = NULL;
        const char* move_type = NULL;
        int damage = 0;

        for (; field != NULL; field = field->next) {
            const char* key = field->name->string;

            if (strcmp(key, "name") == 0) {
                assert(field->value->type == json_type_string);

                struct json_string_s* value =
                    (struct json_string_s*)field->value->payload;

                move_name = value->string;
            }
            else if (strcmp(key, "Move_Type") == 0) {
                assert(field->value->type == json_type_string);

                struct json_string_s* value =
                    (struct json_string_s*)field->value->payload;

                move_type = value->string;
            }
            else if (strcmp(key, "damage") == 0) {
                assert(field->value->type == json_type_number);

                struct json_number_s* value =
                    (struct json_number_s*)field->value->payload;

                damage = atoi(value->number);
            }
        }
        printf("Move: %s, Type: %s, Damage: %d\n",
               move_name, move_type, damage);
    }
    free(root);
}


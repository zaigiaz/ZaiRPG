#include <stdio.h>
#include "data.h"
#include "json.h"
#include "declares.h"

// slurp up file contents
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


void data_entry() {
  printf("hello");
}

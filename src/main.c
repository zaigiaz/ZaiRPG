#include <stdio.h>
#include <stdlib.h>

#define MAX_MOVES 5

// for defining moves
typedef struct {
  char *name;
  int damage;
} Move;

// stats for all entities in the game
typedef struct {
  int HP;
  int AP;
  int MP;
} stats;

// character sheet for a character
typedef struct {
  char *name;
  stats charstats;
  Move moves[MAX_MOVES];
  size_t current_moves;
} charsheet;

// print the player information
void prt_char(charsheet* sheet) {
  printf("==============================\n");
  printf("entity name is: '%s'\n", sheet->name);
  printf("HP:%d ", sheet->charstats.HP);
  printf("MP:%d ", sheet->charstats.MP);
  printf("AP:%d\n", sheet->charstats.AP);
  for(size_t i=0; i < sheet->current_moves; i++) {
    printf("Move: %s - dmg: %d\n", sheet->moves[i].name, sheet->moves[i].damage);
  }
  printf("==============================\n");
}

// generate random character
void proc_char(charsheet* sheet, char* ent_name) {
  // TODO :: add procgen stuff here
  // TODO :: figure out a way to add moves without having to figure out count, etc.
  sheet->name = ent_name;
  sheet->charstats = (stats) { .HP=40, .AP=40, .MP=40 };
  sheet->moves[0] =  (Move)  { .name="Tackle", .damage=10 };
  sheet->moves[1] =  (Move)  { .name="Scratch", .damage=20 };
  sheet->current_moves = 2;
}

// function to get two entities to interact through move system
void attack(charsheet* ent, Move attack) {
  ent->charstats.HP -= attack.damage;  
}

// main
int main() {

  printf("entering game\n");
  
  charsheet *protag, *enem;
  protag = malloc(sizeof(charsheet));
  enem = malloc(sizeof(charsheet));

  proc_char(protag, "protag");
  proc_char(enem, "enem");
  
  attack(protag, (Move){"hi", 20});

  prt_char(protag);
  prt_char(enem);

  free(protag);
  free(enem);
  return 0;
}




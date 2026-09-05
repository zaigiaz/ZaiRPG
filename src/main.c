#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define MAX_MOVES 5
#define MAX_TYPE_COUNT 5

typedef enum {
  MOVE_PHYSICAL,
  MOVE_FLAME,
  MOVE_STORM,
  MOVE_EARTH,
  MOVE_WHITE,
  MOVE_BLACK
} move_type;


// first element is elem being affected by second element
const float type_interaction_table[6][6] = {
    [MOVE_PHYSICAL] = {0.0f, 2.0f, 1.5f, 1.0f, 1.5f, 1.0f},
    [MOVE_FLAME]    = {3.0f, 0.0f, 2.0f, 1.5f, 1.1f, 1.0f},
    [MOVE_STORM]    = {1.0f, 1.0f, 0.0f, 1.0f, 1.4f, 1.5f},
    [MOVE_EARTH]    = {1.0f, 1.8f, 1.0f, 0.0f, 1.2f, 1.0f},
    [MOVE_WHITE]    = {2.0f, 1.0f, 2.0f, 1.0f, 0.0f, 1.5f},
    [MOVE_BLACK]    = {1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 0.0f}
};

// change move_type enum to char pointer
char* mv_type_to_string(move_type type) {
  char *mv_name = "";
  switch(type) {
  case MOVE_PHYSICAL: mv_name = "Physical"; break;
  case MOVE_FLAME: mv_name = "Flame"; break;
  case MOVE_STORM: mv_name = "Storm"; break;
  case MOVE_WHITE: mv_name = "White"; break;
  case MOVE_BLACK: mv_name = "Black"; break;
  case MOVE_EARTH: mv_name = "Earth"; break;
  default: fprintf(stderr, "move doesn't exist"); exit(1);
  }
  return mv_name;
}

// for defining moves
typedef struct {
  char *name;
  int damage;
  move_type type;
} Move;

// stats for all entities in the game
typedef struct {
  int HP;
  int AP;
  int MP;
  int Strength;
  int Intelligence;
  int Endurance;
  
} stats;


// character sheet for a player or entity
typedef struct {
  char *name;  
  move_type player_type;
  stats charstats;
  Move moves[MAX_MOVES];
  size_t current_moves;
} player;


// print the player information
void prt_char_info(player* sheet) {
  printf("==============================\n");
  printf("entity name is: '%s'\n", sheet->name);
  printf("Player Type: %s\n", mv_type_to_string(sheet->player_type));
  printf("HP:%d ", sheet->charstats.HP);
  printf("MP:%d ", sheet->charstats.MP);
  printf("AP:%d\n", sheet->charstats.AP);
  printf("Strength:%-7d ", sheet->charstats.Strength);
  printf("Intelligence:%-7d ", sheet->charstats.Intelligence);
  printf("Wisdom:%-7d\n", sheet->charstats.Endurance);

  for(size_t i=0; i < sheet->current_moves; i++) {
    printf("Move: %-10s Move_Type: %-10s dmg: %d\n", sheet->moves[i].name, 
	   mv_type_to_string(sheet->moves[i].type), sheet->moves[i].damage);
  }
  printf("==============================\n");
}

// generate random character
void proc_char(player* sheet, char* ent_name) {
  // TODO :: figure out a way to add moves without having to figure out count, etc. (store in file)
  // TODO :: make it procgen for generating a character
  sheet->name = ent_name;
  
  /* int random = GetRandomValue(5, 15); */
  /* printf("%d", random); */
  
  sheet->charstats = (stats) { .HP=40,       .AP=40,           .MP=40,
			       .Strength=10, .Intelligence=10, .Endurance=10, };
  sheet->moves[0] =  (Move)  { .name="Tackle", .damage=10 };
  sheet->moves[1] =  (Move)  { .name="Scratch", .damage=20 };
  sheet->current_moves = 2;
}

// function to get two entities to interact through move system
void attack(player* ent, Move attack) {
  float modifier = type_interaction_table[ent->player_type][attack.type];
  ent->charstats.HP -= (attack.damage * modifier);
}

// main
// TODO :: make main game loop of single battle with data I currently have
int main() {

  printf("entering game\n");
  SetRandomSeed((unsigned int)time(NULL));
  
  player *protag, *enem;
  protag = malloc(sizeof(player));
  enem = malloc(sizeof(player));

  proc_char(protag, "protag");
  proc_char(enem, "enem");
  
  attack(protag, (Move){"hi", 10, MOVE_FLAME});

  prt_char_info(protag);
  prt_char_info(enem);

  free(protag);
  free(enem);

  printf("exiting game\n");
  return 0;
}

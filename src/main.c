#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "raylib.h"

#define ARENA_IMPLEMENTATION
#include "arena.h"
#include "data.h"
#include "declares.h"

#define TEST_GAME 0
#define MAX_MOVES 5
#define MAX_TYPE_COUNT 5

typedef enum {
  WIN,
  RUNNING,
  LOSE,
  ERROR
} GAME_STATE;

typedef enum {
  MOVE_PHYSICAL,
  MOVE_FLAME,
  MOVE_STORM,
  MOVE_EARTH,
  MOVE_WHITE,
  MOVE_BLACK
} move_type;

// enum to specify type of class (for tagged union on class)
typedef enum {
  KNIGHT,
  ARCHER,
  MAGE,
  HEALER,
  ROGUE
} class_type;

// first element is elem being affected by second element
const float type_interaction_table[6][6] = {
  [MOVE_PHYSICAL] = {0.0f, 2.0f, 1.5f, 1.0f, 1.5f, 1.0f},
  [MOVE_FLAME]    = {3.0f, 0.0f, 2.0f, 1.5f, 1.1f, 1.0f},
  [MOVE_STORM]    = {1.0f, 1.0f, 0.0f, 1.0f, 1.4f, 1.5f},
  [MOVE_EARTH]    = {1.0f, 1.8f, 1.0f, 0.0f, 1.2f, 1.0f},
  [MOVE_WHITE]    = {2.0f, 1.0f, 2.0f, 1.0f, 0.0f, 1.5f},
  [MOVE_BLACK]    = {1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 0.0f}
};

char* mv_type_to_string(move_type type) {
  char *mv_name = "";
  switch(type) {
  case MOVE_PHYSICAL: mv_name = "Physical"; break;
  case MOVE_FLAME: mv_name = "Flame"; break;
  case MOVE_STORM: mv_name = "Storm"; break;
  case MOVE_WHITE: mv_name = "White"; break;
  case MOVE_BLACK: mv_name = "Black"; break;
  case MOVE_EARTH: mv_name = "Earth"; break;
  default: fprintf(stderr, "move doesn't exist"); exit(1); // TODO :: have better error defaulting
  }
  return mv_name;
}

typedef struct {
  char *name;
  u64 damage;
  move_type type;
} Move;

typedef struct {
  i32 HP;
  i32 AP;
  i32 MP;
  
  u32 Strength;
  u32 Intelligence;
  u32 Endurance;
} stats;

typedef struct {
  char *name;  
  move_type player_type;
  stats charstats;
  Move moves[MAX_MOVES];
  u8 current_moves;
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
  printf("Endurance:%-7d\n", sheet->charstats.Endurance);

  for(size_t i=0; i < sheet->current_moves; i++) {
    printf("Move: %-10s Move_Type: %-10s dmg: %lu\n", sheet->moves[i].name, 
	   mv_type_to_string(sheet->moves[i].type), sheet->moves[i].damage);
  }
  printf("==============================\n");
}

// generate random character
void proc_char(player* sheet, char* ent_name) {
  // TODO :: figure out a way to add moves without having to figure out count, etc. (store in file)
  sheet->name = ent_name;  
  sheet->player_type = MOVE_PHYSICAL;
  u8 rand[6];

  for(u32 i=0; i<countof(rand); i++) {
    rand[i] = GetRandomValue(1, 50);
  }
  sheet->charstats = (stats) { .HP=rand[0],       .AP=rand[1],           .MP=rand[2],
			       .Strength=rand[3], .Intelligence=rand[4], .Endurance=rand[5], };

  // TODO :: function to read json and store moves and the data there
  // and automatically count moves
  sheet->moves[0] =  (Move)  { .name="Tackle",  .damage=2, .type=MOVE_FLAME };
  sheet->moves[1] =  (Move)  { .name="Scratch", .damage=5, .type=MOVE_BLACK };

  sheet->current_moves = 2;
}

// choose attack (random for now)
u8 choose_attack(u8 current_moves) {
  u8 att_move = GetRandomValue(0, current_moves-1);
  return att_move;
}

// function to get two entities to interact through move system
void attack(player* ent, player* enem) {
  u8 index = choose_attack(enem->current_moves);
  Move Chosen = enem->moves[index];
  float modifier = type_interaction_table[ent->player_type][Chosen.type];
  u32 damage = (Chosen.damage * modifier);
  ent->charstats.HP -= damage;
  printf("%s was attacked by %s for %d damage\n", ent->name, Chosen.name, damage);
}

// calculate the player speed
i32 calc_speed(player* ent) {
  return (i32) (ent->charstats.Endurance * 2.5);
}

// Calculate turn action and who can go first
bool calc_turn_action(player* ent, player* opp) {
  if(calc_speed(ent) > calc_speed(opp)) {
    return true;
  }
  return false;
}

void test_window() {
  InitWindow(1200, 1200, "ZaiRPG");
  SetTargetFPS(60);
  while (!WindowShouldClose())    // Detect window close button or ESC key
    {
      BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
      EndDrawing();
    }
  CloseWindow();
}

void test_game() {
  player *protag, *enem;
  protag = malloc(sizeof(player));
  enem = malloc(sizeof(player));

  proc_char(protag, "protag");
  proc_char(enem, "enem");

  prt_char_info(protag);
  prt_char_info(enem);

  u8 turn_action = calc_turn_action(protag,enem);

  if(!turn_action) {
    attack(protag, enem);    
  }

  test_window();

  free(protag);
  free(enem);
}

// TODO :: make main game loop of single battle with data I currently have
// TODO :: what happens when protag dies, code in turn-based thing, flesh-out move-array and move generation and choice
int main() {

  SetRandomSeed((unsigned int)time(NULL));
  Arena game_arena = {0};

  printf("entering game\n");

  if(TEST_GAME == 1) {
    test_game();
    exit(0);
  }

  player *protag, *enem;
  protag = arena_alloc(&game_arena, sizeof(player));
  enem = arena_alloc(&game_arena, sizeof(player));
  proc_char(protag, "protag");    
  proc_char(enem, "enem");
  
  GAME_STATE state = RUNNING;
  while(state == RUNNING) { 

    u8 turn_action = calc_turn_action(protag,enem);
    if(!turn_action) {
      attack(protag, enem);    
    } else {
      attack(enem, protag);
    }

    if(protag->charstats.HP <= 0) {
      state = LOSE;
      printf("player has DIED!\n");
      break;
    }

    if(enem->charstats.HP <= 0) {
      state = WIN;
      printf("player has WON!\n");
      break;
    }
  }

  arena_free(&game_arena);
  printf("exiting game\n");
  return 0;
}

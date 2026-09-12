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

// TODO :: Make basic inventory system

typedef enum : u8 { WIN, RUNNING, LOSE, ERROR } GAME_STATE;

typedef enum : u8 {
  MOVE_PHYSICAL,
  MOVE_FLAME,
  MOVE_STORM,
  MOVE_EARTH,
  MOVE_WHITE,
  MOVE_BLACK,
  MAX_TYPE
} move_type;

typedef enum : u8 {
  KNIGHT,
  ARCHER,
  MAGE
} class_type;

// first element is elem being affected by second element
const f32 type_interaction_table[6][6] = {
  [MOVE_PHYSICAL] = {1.0f, 2.0f, 1.5f, 1.0f, 1.5f, 1.0f},
  [MOVE_FLAME]    = {3.0f, 1.0f, 2.0f, 1.5f, 1.1f, 1.0f},
  [MOVE_STORM]    = {1.0f, 1.0f, 1.0f, 1.0f, 1.4f, 1.5f},
  [MOVE_EARTH]    = {1.0f, 1.8f, 1.0f, 1.0f, 1.2f, 1.0f},
  [MOVE_WHITE]    = {2.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.5f},
  [MOVE_BLACK]    = {1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f}
};

const char* mv_type_to_string(move_type type) {
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
  move_type type;
  u64 damage;
  u8 range;
  u16 * effects_targets; 
  // TODO :: add effect type
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
  const char *name;  
  move_type player_type;
  stats charstats;
  Move moves[6];
  u8 current_moves;
} player_ent;

void prt_char_info(const player_ent* sheet) {
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

void proc_rand_char(player_ent* sheet, char* ent_name) {
  sheet->name = ent_name;  
  sheet->player_type = GetRandomValue(0, MAX_TYPE-1);
  u32 rand[6];

  for(u8 i=0; i<countof(rand); i++) {
    rand[i] = GetRandomValue(1, 50);
  }
  sheet->charstats = (stats) { .HP=rand[0],       .AP=rand[1],           .MP=rand[2],
			       .Strength=rand[3], .Intelligence=rand[4], .Endurance=rand[5], };

  // TODO :: integrate with JSON system to pick random moves according to ent type
  sheet->moves[0] =  (Move)  { .name="Tackle",  .damage=2, .type=MOVE_FLAME };
  sheet->moves[1] =  (Move)  { .name="Scratch", .damage=5, .type=MOVE_BLACK };
  sheet->current_moves = 2;
}

u8 choose_attack(u8 current_moves) {
  u8 att_move = GetRandomValue(1, current_moves-1);
  return att_move;
}

void attack(player_ent* ent, const player_ent* enem) {
  u8 index = choose_attack(enem->current_moves);
  Move Chosen = enem->moves[index];
  f32 modifier = type_interaction_table[ent->player_type][Chosen.type];
  u32 damage = (Chosen.damage * modifier);
  ent->charstats.HP -= damage;
  printf("%s was attacked by %s with %s for %d damage\n", ent->name, enem->name, Chosen.name, damage);
}

i32 calc_speed(const player_ent* ent) {
  return (i32) (ent->charstats.Endurance * 2.5);
}

i32 calc_power(const player_ent* ent) {
  i32 base = (ent->charstats.HP + ent-> charstats.MP + ent->charstats.AP);
  f32 modifiers = (ent->charstats.Strength + ent->charstats.Intelligence + ent->charstats.Endurance);
  return (i32) (base * modifiers);
}

bool calc_turn_action(const player_ent* ent, const player_ent* opp) {
  if(calc_speed(ent) > calc_speed(opp)) {
    return true;
  }
  return false;
}

void test_window() {
  InitWindow(1200, 1200, "ZaiRPG");
  SetTargetFPS(60);
  while (!WindowShouldClose())
    {
      BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
      EndDrawing();
    }
  CloseWindow();
}

void test_game() {
  player_ent *protag, *enem;
  protag = malloc(sizeof(player_ent));
  enem = malloc(sizeof(player_ent));

  proc_rand_char(protag, "protag");
  proc_rand_char(enem, "enem");

  prt_char_info(protag);
  prt_char_info(enem);

  u8 turn_action = calc_turn_action(protag,enem);

  if(!turn_action) {
    attack(protag, enem);    
  }

  /* s8* st = slurp("../TODO_LONG"); */
  /* printf("\n%s\n", st->data); */
  /* test_window(); */

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

  player_ent *protag, *enem;
  protag = arena_alloc(&game_arena, sizeof(player_ent));
  enem = arena_alloc(&game_arena, sizeof(player_ent));
  proc_rand_char(protag, "protag");    
  proc_rand_char(enem, "enem");
  
  GAME_STATE state = RUNNING;
  while(state == RUNNING) { 

    u8 turn_action = calc_turn_action(protag,enem);
    if(!turn_action) {
      attack(protag, enem);    
      attack(enem, protag);
    } else {
      attack(enem, protag);
      attack(protag, enem);    
    }

    // TODO :: wrap this in a state machine handling battle/game progress
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
}

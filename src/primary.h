#ifndef PRIMARY_H
#define PRIMARY_H

#include "declares.h"

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

typedef struct {
  char *name;
  move_type type;
  u64 damage;
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


#endif

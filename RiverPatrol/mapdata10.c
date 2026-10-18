/*
 *      ZX81 Map Data Module (Round 10)
 */

#include "mapdata.h"
#include "objects.h"

// river data
unsigned char rock_tbl10[250] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0, 21, 13,  0,  0,  0,  2,
  2,  0,  0,  0,  0, 13,  0,  0,  0,  0,
  0,  0,  0, 23,  0,  0, 52,  0,  2,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0, 65,
  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,
  2,  0,  2, 65,  0,  0,  0,  0, 25,  0,
  0,  0,  1,  0,  0,  0, 52,  0,  0,  0,
  0, 45, 65,  0,  0,  1,  0,  0,  0,  0,
  0, 52, 32,  2,  2,  2,  0,  0,  7, 25,
 34, 34,  2, 32,  0, 32, 32,  0,  0,  0,
  0,  0,  0,  0,  2, 70,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  2,  0,  1, 13, 25,
  0,  0,  0,  0,  0, 17,  0,  0, 25,  0,
  0,  0, 32,  0,  0,  0,  1,  0, 13,  0,
 65,  0,  0,  0,  0,  2,  0,  0,  0, 23,
  0,  0,  0,  0,  0, 25, 16,  0,  0,  0,
  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,
  2,  0,  0,  0,  0,  0,131,  0,  0,132,
  0,  0, 13, 32,  0,  0, 13,  0, 16, 34,
  0,  1,  0,133,  0,  0, 52,  0,  0, 65,
  0,  1, 13,  0, 23,  0,  0,  2,  0,  0,
  0,  1,  0, 32, 45, 16, 13,  0,  2,  0,
  0,  0, 21, 13,  0, 72,  2,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

struct SPAWN_TBL spawn_tbl10[]  = {
{19,SPAWN_ATTR(7,OBJ_DRIFTER)},
{25,SPAWN_ATTR(9,OBJ_WHIRL)},
{30,SPAWN_ATTR(6,OBJ_CROC)},
{31,SPAWN_ATTR(12,OBJ_DRIFTER)},
{40,SPAWN_ATTR(5,OBJ_DRIFTER)},
{43,SPAWN_ATTR(12,OBJ_RAFT)},
{47,SPAWN_ATTR(7,OBJ_ENEMY)},
{52,SPAWN_ATTR(13,OBJ_DRIFTER)},
{57,SPAWN_ATTR(6,OBJ_DRIFTER)},
{60,SPAWN_ATTR(12,OBJ_CROC)},
{64,SPAWN_ATTR(13,OBJ_ENEMY)},
{70,SPAWN_ATTR(13,OBJ_BARREL)},
{72,SPAWN_ATTR(4,OBJ_CROC)},
{77,SPAWN_ATTR(9,OBJ_BARREL)},
{83,SPAWN_ATTR(12,OBJ_RAFT)},
{91,SPAWN_ATTR(5,OBJ_WHIRL)},
{100,SPAWN_ATTR(13,OBJ_ENEMY)},
{113,SPAWN_ATTR(9,OBJ_RAFT)},
{119,SPAWN_ATTR(13,OBJ_DRIFTER)},
{123,SPAWN_ATTR(5,OBJ_DRIFTER)},
{128,SPAWN_ATTR(9,OBJ_DRIFTER)},
{133,SPAWN_ATTR(8,OBJ_WHIRL)},
{137,SPAWN_ATTR(10,OBJ_DRIFTER)},
{143,SPAWN_ATTR(5,OBJ_DRIFTER)},
{145,SPAWN_ATTR(11,OBJ_DRIFTER)},
{149,SPAWN_ATTR(6,OBJ_DRIFTER)},
{153,SPAWN_ATTR(12,OBJ_DRIFTER)},
{158,SPAWN_ATTR(5,OBJ_DRIFTER)},
{163,SPAWN_ATTR(12,OBJ_DRIFTER)},
{170,SPAWN_ATTR(9,OBJ_DRIFTER)},
{176,SPAWN_ATTR(4,OBJ_WHIRL)},
{195,SPAWN_ATTR(12,OBJ_CROC)},
{205,SPAWN_ATTR(7,OBJ_DRIFTER)},
{214,SPAWN_ATTR(7,OBJ_CROC)},
{0,SPAWN_ATTR(0, OBJ_NONE)}
};

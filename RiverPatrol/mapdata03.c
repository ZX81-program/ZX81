/*
 *      ZX81 Map Data Module (Round 3)
 */

#include "mapdata.h"
#include "objects.h"

// river data
unsigned char rock_tbl03[250] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0, 74,  0,  0,  0,  0,  0,
 33,  0, 32,  0, 29,  0,  0,  0, 75,  0,
 23,  0,  0, 76, 33, 48,  0, 13,  0, 77,
  0,  0, 65,  0,  0,  0, 78,  0,  0,  0,
 79,  0,  0,  0,  0, 78,  0,  0,  0, 79,
  0,  0,  0,  0,  0, 40, 80, 59, 25,  0,
  0,  0,  0, 73, 81, 82, 81, 15,  0,  0,
  0,  0,  0, 71, 73, 73, 33, 64, 64,  0,
  0,  9,  0, 19, 10, 10,  0,  0,  0,  0,
 33, 33, 83, 84, 85, 84, 86, 87, 88, 88,
 84, 83,  0,  0,  0,  0, 89, 89, 89,  0,
  0,  0,  0, 30, 59, 59, 34,  0,  0,  0,
  0,  0,  0,  0,  0,  0, 33, 73, 33,  0,
 40, 40, 90, 85, 84, 83,  0,  0,  0, 10,
  0,  0, 15, 15, 33, 91, 73,  0,  0, 13,
 29,  0,  0,  0, 92, 47, 64,  0,  0,  0,
  0,  0, 32,  2,  2, 10,  0,  0, 84,  0,
  0, 32, 10, 34, 33, 73, 91, 91, 71, 30,
 13,  0,  0,  0, 34, 42, 10,  0,  0,  0,
  0, 93, 94, 33, 73, 91, 73,  0,  0,  0,
  2, 70, 70,  2,  0,  0,  0,  0,  0,  0,
  0,  9, 84,  9,  0,  0, 10, 10,  0,  0,
  0,  0, 44, 45,  0, 24, 46,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

struct SPAWN_TBL spawn_tbl03[]  = {
{17,SPAWN_ATTR(10,OBJ_WHIRL)},
{20,SPAWN_ATTR(14,OBJ_DRIFTER)},
{28,SPAWN_ATTR(4,OBJ_RAFT)},
{30,SPAWN_ATTR(4,OBJ_DRIFTER)},
{41,SPAWN_ATTR(3,OBJ_WHIRL)},
{46,SPAWN_ATTR(3,OBJ_DRIFTER)},
{50,SPAWN_ATTR(14,OBJ_DRIFTER)},
{55,SPAWN_ATTR(3,OBJ_DRIFTER)},
{59,SPAWN_ATTR(14,OBJ_DRIFTER)},
{65,SPAWN_ATTR(10,OBJ_ENEMY)},
{70,SPAWN_ATTR(6,OBJ_DRIFTER)},
{72,SPAWN_ATTR(13,OBJ_WHIRL)},
{74,SPAWN_ATTR(7,OBJ_RAFT)},
{84,SPAWN_ATTR(10,OBJ_DRIFTER)},
{92,SPAWN_ATTR(2,OBJ_CROC)},
{98,SPAWN_ATTR(16,OBJ_DRIFTER)},
{100,SPAWN_ATTR(9,OBJ_WHIRL)},
{104,SPAWN_ATTR(10,OBJ_ENEMY)},
{112,SPAWN_ATTR(8,OBJ_DRIFTER)},
{118,SPAWN_ATTR(5,OBJ_RAFT)},
{123,SPAWN_ATTR(1,OBJ_CROC)},
{125,SPAWN_ATTR(12,OBJ_WHIRL)},
{128,SPAWN_ATTR(16,OBJ_DRIFTER)},
{133,SPAWN_ATTR(15,OBJ_DRIFTER)},
{143,SPAWN_ATTR(3,OBJ_DRIFTER)},
{148,SPAWN_ATTR(5,OBJ_BARREL)},
{154,SPAWN_ATTR(16,OBJ_DRIFTER)},
{160,SPAWN_ATTR(7,OBJ_DRIFTER)},
{163,SPAWN_ATTR(1,OBJ_CROC)},
{166,SPAWN_ATTR(15,OBJ_RAFT)},
{171,SPAWN_ATTR(1,OBJ_WHIRL)},
{171,SPAWN_ATTR(14,OBJ_DRIFTER)},
{185,SPAWN_ATTR(16,OBJ_DRIFTER)},
{188,SPAWN_ATTR(16,OBJ_DRIFTER)},
{193,SPAWN_ATTR(15,OBJ_BARREL)},
{210,SPAWN_ATTR(2,OBJ_ENEMY)},
{213,SPAWN_ATTR(2,OBJ_DRIFTER)},
{218,SPAWN_ATTR(10,OBJ_DRIFTER)},
{223,SPAWN_ATTR(12,OBJ_CROC)},
{0,SPAWN_ATTR(0, OBJ_NONE)}
};

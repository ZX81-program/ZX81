/*
 *      ZX81 Map Data Module (Round 5)
 */

#include "mapdata.h"
#include "objects.h"

// river data
unsigned char rock_tbl05[250] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,111,  0,  0,  0,  0,  0,  0,
112,  0,  0,  2,  0,  0,  0,  1, 30,  0,
 25,  0,  2,  0,  0,  0,  0,113,  0,  0,
  2,  0,  0, 16,  0,  0,  0, 71,  0,  9,
  0,  0, 23, 13,  0,  0,  0,  0,  0,  0,
 23,  0,  0,  0,114,  0,  0, 37,115,106,
 25, 59, 43, 42, 10, 42,  0,  0,  9,  2,
 10, 70, 54, 54, 54, 54, 54, 54, 70, 10,
  2,  0,  0,  0,  0,  0,  0,  0,  0, 15,
  0,116, 65, 25, 45, 45,  0,  0, 72, 64,
 64, 32, 32, 32, 32,  0,  0,  0,  0, 15,
114,114,117,  8,  8,  0,  0,  0,  0, 10,
 19, 43, 43, 43, 43,116, 40, 40, 40, 40,
 40, 40,  9,  0,  0,  0,  0,  0,  0,  0,
 92, 10,118,114, 15, 15,  0,  0,  0,  0,
  0,  0, 45, 25, 65,  0, 15, 15, 15,  0,
  2, 34, 65, 25,  0,  0,  0, 70,103,  0,
  0,  0,  2, 32, 52,  0,  9, 40,  0, 93,
 93, 93, 40,  9,  0,  0,  0, 59, 59, 57,
 42,  0,  0, 15, 15, 15, 64, 47, 47, 16,
  0,  0,  0,  0,  0, 15, 15,  0, 52, 92,
 70,119,119, 70, 92, 52,  0,  0,  0,  0,
  0,  0, 44, 45,  0, 24,120,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

struct SPAWN_TBL spawn_tbl05[]  = {
{17,SPAWN_ATTR(11,OBJ_WHIRL)},
{20,SPAWN_ATTR(11,OBJ_DRIFTER)},
{22,SPAWN_ATTR(7,OBJ_CROC)},
{27,SPAWN_ATTR(9,OBJ_RAFT)},
{32,SPAWN_ATTR(4,OBJ_DRIFTER)},
{36,SPAWN_ATTR(8,OBJ_WHIRL)},
{40,SPAWN_ATTR(14,OBJ_DRIFTER)},
{42,SPAWN_ATTR(6,OBJ_CROC)},
{47,SPAWN_ATTR(14,OBJ_ENEMY)},
{57,SPAWN_ATTR(14,OBJ_CROC)},
{59,SPAWN_ATTR(4,OBJ_WHIRL)},
{66,SPAWN_ATTR(9,OBJ_DRIFTER)},
{74,SPAWN_ATTR(15,OBJ_ENEMY)},
{75,SPAWN_ATTR(3,OBJ_DRIFTER)},
{90,SPAWN_ATTR(3,OBJ_DRIFTER)},
{106,SPAWN_ATTR(9,OBJ_RAFT)},
{112,SPAWN_ATTR(3,OBJ_DRIFTER)},
{117,SPAWN_ATTR(14,OBJ_ENEMY)},
{119,SPAWN_ATTR(13,OBJ_WHIRL)},
{122,SPAWN_ATTR(11,OBJ_CROC)},
{141,SPAWN_ATTR(9,OBJ_RAFT)},
{148,SPAWN_ATTR(15,OBJ_DRIFTER)},
{158,SPAWN_ATTR(15,OBJ_DRIFTER)},
{167,SPAWN_ATTR(4,OBJ_DRIFTER)},
{176,SPAWN_ATTR(3,OBJ_ENEMY)},
{178,SPAWN_ATTR(15,OBJ_DRIFTER)},
{181,SPAWN_ATTR(15,OBJ_DRIFTER)},
{186,SPAWN_ATTR(2,OBJ_WHIRL)},
{196,SPAWN_ATTR(14,OBJ_DRIFTER)},
{200,SPAWN_ATTR(13,OBJ_CROC)},
{215,SPAWN_ATTR(13,OBJ_WHIRL)},
{219,SPAWN_ATTR(2,OBJ_DRIFTER)},
{220,SPAWN_ATTR(15,OBJ_BARREL)},
{0,SPAWN_ATTR(0, OBJ_NONE)}
};

/*
 *      ZX81 Map Data Module (Round 4)
 */

#include "mapdata.h"
#include "objects.h"

// river data
unsigned char rock_tbl04[250] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 16,  0,
 13,  0,  0,  0,  0,  0,  0, 95,  0,  0,
 83,  0, 13,  0,  0,  0,  0,  0,  1,  0,
 33,  0, 32,  0,  0,  0, 45,  0,  0,  0,
 23,  0,  0,  0,  0, 25,  0,  0, 16,  0,
 30,  0,  0,  0,  0,  0, 84, 85, 85, 96,
 97, 83, 83, 98, 99,  0,  0, 33, 73,100,
 90, 90, 93, 17,  0, 73, 33,  0,  0,  0,
  0,  0,  0,  0, 33,  0,  0,101, 17, 23,
  1,  0,  0,  0,  0,  0,  0,  0,  0, 83,
102, 44, 45,  0,  0,  0,  0,  0, 16, 52,
 32,103, 64, 64, 64, 52, 95,  0,  0,  0,
  0,  0,  0,  0, 45, 43, 42, 10,  2, 32,
 32,  0,  0,  0,  0,  0, 83,  0,  0, 23,
  0,  0, 79, 91,104, 33, 52,  0,  0,  0,
 85, 84, 84,  0,  0, 30, 73,  0,  0,  0,
  0,105, 32,  0, 40, 84,106,107,  0,  0,
 33,  0,  0,  0,  0, 65, 45,  0, 64,103,
 72,  0,  0, 33, 73,108, 92, 92,  0,  0,
  0, 59, 43,  0,  0,  0,  0,  0,109, 73,
 73, 33,  0,  0,  0,  0,  0,  0,110, 86,
 84, 84, 83,  0,  0,  0,  0, 32, 16, 23,
  0,  0, 44, 45,  0, 24, 46,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

struct SPAWN_TBL spawn_tbl04[]  = {
{19,SPAWN_ATTR(8,OBJ_CROC)},
{22,SPAWN_ATTR(6,OBJ_DRIFTER)},
{26,SPAWN_ATTR(13,OBJ_BARREL)},
{31,SPAWN_ATTR(8,OBJ_BARREL)},
{36,SPAWN_ATTR(8,OBJ_DRIFTER)},
{37,SPAWN_ATTR(3,OBJ_BARREL)},
{45,SPAWN_ATTR(12,OBJ_RAFT)},
{48,SPAWN_ATTR(2,OBJ_CROC)},
{51,SPAWN_ATTR(6,OBJ_ENEMY)},
{56,SPAWN_ATTR(15,OBJ_BARREL)},
{66,SPAWN_ATTR(5,OBJ_DRIFTER)},
{68,SPAWN_ATTR(10,OBJ_BARREL)},
{81,SPAWN_ATTR(6,OBJ_DRIFTER)},
{85,SPAWN_ATTR(15,OBJ_DRIFTER)},
{87,SPAWN_ATTR(10,OBJ_WHIRL)},
{93,SPAWN_ATTR(8,OBJ_RAFT)},
{100,SPAWN_ATTR(16,OBJ_DRIFTER)},
{115,SPAWN_ATTR(7,OBJ_DRIFTER)},
{122,SPAWN_ATTR(5,OBJ_CROC)},
{126,SPAWN_ATTR(15,OBJ_DRIFTER)},
{137,SPAWN_ATTR(15,OBJ_WHIRL)},
{140,SPAWN_ATTR(14,OBJ_CROC)},
{143,SPAWN_ATTR(10,OBJ_DRIFTER)},
{147,SPAWN_ATTR(7,OBJ_RAFT)},
{160,SPAWN_ATTR(6,OBJ_DRIFTER)},
{165,SPAWN_ATTR(7,OBJ_WHIRL)},
{165,SPAWN_ATTR(15,OBJ_RAFT)},
{168,SPAWN_ATTR(6,OBJ_DRIFTER)},
{175,SPAWN_ATTR(2,OBJ_BARREL)},
{180,SPAWN_ATTR(15,OBJ_DRIFTER)},
{191,SPAWN_ATTR(7,OBJ_DRIFTER)},
{194,SPAWN_ATTR(13,OBJ_CROC)},
{200,SPAWN_ATTR(10,OBJ_RAFT)},
{203,SPAWN_ATTR(14,OBJ_WHIRL)},
{208,SPAWN_ATTR(16,OBJ_DRIFTER)},
{214,SPAWN_ATTR(5,OBJ_DRIFTER)},
{217,SPAWN_ATTR(13,OBJ_BARREL)},
{222,SPAWN_ATTR(10,OBJ_DRIFTER)},
{0,SPAWN_ATTR(0, OBJ_NONE)}
};

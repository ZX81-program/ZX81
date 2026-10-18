/*
 *      ZX81 Map Data Module (Round 2)
 */

#include "mapdata.h"
#include "objects.h"

// river data
unsigned char rock_tbl02[250] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0, 41, 47,  0, 48,  0,
 10, 49,  0,  0,  0,  9,  9,  9,  9,  9,
 50, 48, 48, 51, 23, 23, 23, 23, 52,  2,
  2,  0,  0,  0,  0, 53,  0,  0,  0,  0,
 21,  0,  0,  0, 12,  0,  0, 54, 45, 13,
 30, 19, 34,  0,  0,  0,  0, 13, 55,  7,
 56,  0,  0,  0, 33, 12,  0,  0, 57, 20,
 44,  0,  0,  0,  0, 58, 32,  2, 34,  0,
  0,  0,  0, 59, 44, 60,  0, 13,  0, 23,
 17, 23,  0,  0,  0,  0,  2, 10, 47, 16,
  0, 12,  0,  0, 45, 61, 62, 61, 45,  0,
  0,  0,  0, 45, 11, 63, 63, 11,  5,  0,
  0, 45, 60, 60, 60, 13,  0,  0,  0,  0,
 64, 32,  0, 12,  0, 65, 66, 44, 67, 29,
 30,  0,  0,  0, 68,  0,  5, 25,  0, 16,
  0, 69, 35,  0, 15,  1, 34,  0,  0,  0,
  0,  0,  0,  0,  2, 70, 70,  2,  0,  0,
 15, 71, 60, 71, 15,  0,  0,  0,  0,  0,
  0,  0,  0,  0, 16, 17,  1,  0,  0, 59,
 59, 42,  0,  0,  0, 41, 41,  0, 52, 72,
 23, 23,  0,  0,  0, 33, 73, 73, 33,  0,
  1, 17, 34,  0,  0,  0,  0,  0,  0,  0,
  0,  0, 44, 45,  0, 24, 46,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

struct SPAWN_TBL spawn_tbl02[]  = {
{ 19, SPAWN_ATTR( 4, OBJ_BARREL)},
{ 25, SPAWN_ATTR( 5, OBJ_WHIRL)},
{ 30, SPAWN_ATTR( 4, OBJ_CROC)},
{ 37, SPAWN_ATTR(17, OBJ_DRIFTER)},
{ 45, SPAWN_ATTR(14, OBJ_CROC)},
{ 52, SPAWN_ATTR(14, OBJ_WHIRL)},
{ 61, SPAWN_ATTR(16, OBJ_DRIFTER)},
{ 68, SPAWN_ATTR( 8, OBJ_DRIFTER)},
{ 78, SPAWN_ATTR(14, OBJ_RAFT)},
{ 82, SPAWN_ATTR( 2, OBJ_DRIFTER)},
{ 91, SPAWN_ATTR(12, OBJ_BARREL)},
{ 95, SPAWN_ATTR(17, OBJ_DRIFTER)},
{ 97, SPAWN_ATTR(16, OBJ_CROC)},
{104, SPAWN_ATTR(17, OBJ_DRIFTER)},
{107, SPAWN_ATTR( 1, OBJ_WHIRL)},
{115, SPAWN_ATTR( 0, OBJ_CROC)},
{116, SPAWN_ATTR(10, OBJ_DRIFTER)},
{127, SPAWN_ATTR( 8, OBJ_DRIFTER)},
{136, SPAWN_ATTR(15, OBJ_DRIFTER)},
{139, SPAWN_ATTR(16, OBJ_BARREL)},
{162, SPAWN_ATTR(11, OBJ_DRIFTER)},
{169, SPAWN_ATTR(13, OBJ_CROC)},
{172, SPAWN_ATTR(15, OBJ_WHIRL)},
{174, SPAWN_ATTR(17, OBJ_DRIFTER)},
{183, SPAWN_ATTR(11, OBJ_BARREL)},
{190, SPAWN_ATTR( 1, OBJ_DRIFTER)},
{195, SPAWN_ATTR( 3, OBJ_WHIRL)},
{205, SPAWN_ATTR( 6, OBJ_DRIFTER)},
{209, SPAWN_ATTR(17, OBJ_RAFT)},
{219, SPAWN_ATTR( 5, OBJ_DRIFTER)},
{222, SPAWN_ATTR( 3, OBJ_BARREL)},
{  0, SPAWN_ATTR( 0, OBJ_NONE)}
};

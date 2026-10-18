/*
 *      ZX81 Map Data Module (Round 1)
 */

#include "mapdata.h"
#include "objects.h"

// rock data
unsigned char rock_tbl01[250] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  2,  3,  4,  0,
  0,  5,  0,  0,  6,  7,  8,  9,  0,  0,
  0, 10,  0,  0,  0,  0, 11, 12,  0,  0,
  0, 13,  0,  0,  0,  0, 14,  0,  0,  0,
 15,  0, 16, 17, 18, 12,  0,  0, 10, 19,
 20, 21, 22,  0,  0,  0, 23, 24, 24, 23,
  0, 12,  0,  0, 25, 26, 26, 25, 25,  0,
  0,  0,  0,  0,  0,  1, 16, 13, 13,  0,
  5, 27,  0,  0,  0, 25,  0,  0,  0,  0,
 23, 28,  0,  0, 29, 29, 30,  0,  0,  0,
  0, 30,  0,  0,  0,  0,  5, 31, 32,  0,
  0, 33,  0,  0,  0,  0,  0,  0,  0, 34,
 25, 13,  0, 35,  0,  0,  0,  0,  0,  0,
 23, 24, 36, 24,  0,  0, 15, 15,  0,  0,
 34,  0,  0,  0,  0, 37, 38, 12,  0,  0,
  0, 39,  0,  0,  0,  0, 13,  0,  5, 35,
  0,  0,  0, 16,  0, 33,  0,  0,  5,  9,
 40, 40,  0,  0,  0,  0, 15,  0,  0,  0,
  0, 41, 41, 33,  0,  0, 42,  0,  0,  0,
  0,  0,  9, 11, 35,  0,  0,  0, 43, 25,
  0,  0,  0,  0,  0, 30,  0,  0,  0,  0,
  0,  0, 44, 45,  0, 24, 46,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

struct SPAWN_TBL spawn_tbl01[]  = {
 { 15, SPAWN_ATTR( 9, OBJ_ENEMY)},
 { 16, SPAWN_ATTR( 4, OBJ_DRIFTER)},
 { 21, SPAWN_ATTR( 4, OBJ_WHIRL)},
 { 24, SPAWN_ATTR(15, OBJ_DRIFTER)},
 { 32, SPAWN_ATTR( 7, OBJ_DRIFTER)},
 { 39, SPAWN_ATTR(16, OBJ_DRIFTER)},
 { 46, SPAWN_ATTR( 6, OBJ_RAFT)},
 { 53, SPAWN_ATTR(10, OBJ_WHIRL)},
 { 56, SPAWN_ATTR(13, OBJ_BARREL)},
 { 62, SPAWN_ATTR( 6, OBJ_DRIFTER)},
 { 68, SPAWN_ATTR(14, OBJ_CROC)},
 { 77, SPAWN_ATTR( 3, OBJ_WHIRL)},
 { 85, SPAWN_ATTR(17, OBJ_ENEMY)},
 { 88, SPAWN_ATTR(17, OBJ_DRIFTER)},
 {101, SPAWN_ATTR( 9, OBJ_DRIFTER)},
 {110, SPAWN_ATTR(10, OBJ_CROC)},
 {117, SPAWN_ATTR(16, OBJ_DRIFTER)},
 {127, SPAWN_ATTR( 5, OBJ_WHIRL)},
 {131, SPAWN_ATTR(15, OBJ_RAFT)},
 {139, SPAWN_ATTR( 1, OBJ_CROC)},
 {142, SPAWN_ATTR( 9, OBJ_DRIFTER)},
 {156, SPAWN_ATTR(13, OBJ_DRIFTER)},
 {162, SPAWN_ATTR( 4, OBJ_BARREL)},
 {168, SPAWN_ATTR(17, OBJ_DRIFTER)},
 {174, SPAWN_ATTR(11, OBJ_RAFT)},
 {176, SPAWN_ATTR( 9, OBJ_DRIFTER)},
 {191, SPAWN_ATTR( 7, OBJ_RAFT)},
 {191, SPAWN_ATTR(10, OBJ_DRIFTER)},
 {201, SPAWN_ATTR(12, OBJ_WHIRL)},
 {201, SPAWN_ATTR(17, OBJ_DRIFTER)},
 {208, SPAWN_ATTR( 2, OBJ_BARREL)},
 {218, SPAWN_ATTR(15, OBJ_RAFT)},
 {219, SPAWN_ATTR( 2, OBJ_DRIFTER)},
 {223, SPAWN_ATTR(11, OBJ_DRIFTER)},
 {  0, SPAWN_ATTR( 0, OBJ_NONE)}
};

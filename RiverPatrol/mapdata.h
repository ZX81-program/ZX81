/*
 *      ZX81 Map Data Handling Module header
 */

#ifndef _ZX81_MAPDATA_H
#define _ZX81_MAPDATA_H


// Spawn Table
#define SPAWN_ATTR(x, type) (((x) << 3) | ((type) & 0x07))
#define GET_TYPE(attr)      ((attr) & 0x07)
#define GET_X(attr)         ((attr) >> 3)

struct SPAWN_TBL {
    unsigned char y;
    unsigned char attr;
};

// Global Map Data Pointers (Current Round)
extern struct SPAWN_TBL *p_spawn_tbl;
extern unsigned char *p_rock_tbl;
extern unsigned char (*p_rock_ptn)[3];


// Function Declarations

void set_mapdata(int round);


#endif  // _ZX81_MAPDATA_H

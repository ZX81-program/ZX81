/*
 *      ZX81 Map Handling Module header
 */

#ifndef _ZX81_MAP_H
#define _ZX81_MAP_H


#define RENDER_DX	22
#define RENDER_DY	24

// map valiavles
extern int  map_scroll_y;
extern char map_min_x;
extern char map_max_x;


// screen functions
unsigned char* getrenderaddr(int posy, int posx);
#asm
EXTERN  asm_getrenderaddr
#endasm

// map functions
void map_init();
int  map_scroll(int dy);
void map_render(void);
void map_draw(void);

#endif  // _ZX81_MAP_H


/*
 *      ZX81 Screen Handling Module header
 */

#ifndef _ZX81_SCRN_H
#define _ZX81_SCRN_H

// screen address
#define D_FILE          0x400c
#define D_DX            33      // 32+1(CR)
#define D_DY            24

extern unsigned int  score;
extern unsigned int  hiscore;
extern unsigned int  round;
extern unsigned int  boat_lives;
extern unsigned int  boat_extend;
extern unsigned int  game_speed;

// screen functions
void scrn_init();
unsigned char* getscrnaddr(int posy, int posx);
void clear_game_screen();

// zx81 write
void zx81_write_str(unsigned char *dstp, const char *asciip, int step);
void zx81_write_num(unsigned char *dstp, unsigned int num, int ndigit, int fill_zero);

// score
void score_draw();

// round
void rounds_draw();

// boat lives
void boat_lives_draw();

// game speed
void game_speed_draw();
void game_speed_set(unsigned char key);

// goal meter
void goal_meter_init(void);
void goal_meter_draw(int dy);

extern int water_level;

// water_gauge
void water_gauge_init(void);
void water_gauge_draw(int water_leak_level);

#endif  // _ZX81_SCRN_H


#ifndef __SCRN
#define __SCRN

#include "map.h"

#define OFF     0
#define ON      1
#define ALLON   3


// screen memory

#define D_FILE          0x400c
#define D_DX            (32 + 1)
#define D_DY            24


// back buffer

#define BAK_DX      9*CHR_DX
#define BAK_DY      9*CHR_DY

#define VIEW_DX     7
#define VIEW_DY     7

extern unsigned char bakbuf[BAK_DY][BAK_DX];

extern unsigned int bakbuf_sx;
extern unsigned int bakbuf_sy;


// radar

#define RADAR_SX    24
#define RADAR_SY    7
#define RADAR_DX    8
#define RADAR_DY    14

extern unsigned char radarbuf[RADAR_DY][RADAR_DX];


// fuel

#define FUEL_SX     24
#define FUEL_SY     5
#define FUEL_DX     8


// car table

#define CAR_NUM    8
#define CAR_UP     0
#define CAR_RT     2
#define CAR_DN     4
#define CAR_LT     6

extern unsigned char mycartbl[CAR_NUM][CHR_DY][CHR_DX];
extern unsigned char encartbl[CAR_NUM][CHR_DY][CHR_DX];


// live

#define LIVE_SY 21
#define LIVE_SX 24
extern int live_num;


unsigned char *getscrnaddr(int posy, int posx);

void clr_main();

void displive();

void makebak(unsigned char *bakbufp, int fldy, int fldx,
             int bakbuf_dy, int bakbuf_dx, int bakubuf_nextline);

void initcourse();
void dispcourse(int cary, int carx, int stepy, int stepx);

void initradar();
unsigned char getdotval(unsigned char dotval, int y, int x, int onoff);
void setradar(int y, int x, int onoff);
void dispradar();

void dispfuel(int num);

void dispchr(unsigned char *chrbufp, int posy, int posx);

void copyascii2zx81chr(unsigned char *dstp, char *asciip);
void copynum2zx81chr(unsigned char *dstp, int num, int ndigit);

#endif
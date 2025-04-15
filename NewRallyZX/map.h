#ifndef __MAP
#define __MAP

// map

#define MAP_NUM     4

#define MAP_DX      4
#define MAP_DY      56

extern unsigned char maptbl[MAP_NUM][MAP_DY][MAP_DX];


// position

struct POS {
    unsigned char x;
    unsigned char y;
};


// flag

#define FLAG_NUM    5

struct FLAGTBL {
    unsigned char num;
    struct POS pos[17];
};

extern struct FLAGTBL flagtbl[MAP_NUM][FLAG_NUM];


// rock

#define ROCK_NUM    4

struct ROCKTBL {
    unsigned char num;
    struct POS pos[4];
};

extern struct ROCKTBL rocktbl[MAP_NUM][ROCK_NUM];


// field

#define FLD_DX      MAP_DX * 8
#define FLD_DY      MAP_DY

extern unsigned char fldtbl[FLD_DY][FLD_DX];


// mapping

#define FLAG_MAX    10
#define ROCK_MAX    10


// character table

#define CHR_SPC     0
#define CHR_NFLAG   1   // normal flag
#define CHR_SFLAG   2   // special flag
#define CHR_LFLAG   3   // lucky flag
#define CHR_ROCK    4
#define CHR_SMOKE   5
#define CHR_BLK     6
#define CHRBIT_UP   0x01
#define CHRBIT_DN   0x02
#define CHRBIT_LT   0x04
#define CHRBIT_RT   0x08

#define CHR_NUM 22
#define CHR_DX  3
#define CHR_DY  3

extern unsigned char chrtbl[CHR_NUM][CHR_DY][CHR_DX];


// directon

#define DIR_NONE    0x00
#define DIR_UP      0x01
#define DIR_RT      0x02
#define DIR_DN      0x04
#define DIR_LT      0x08


void makefld(unsigned char *maptblp);

unsigned char randum();

void makemap(int mapno, int rocknum);

unsigned char getfldchr(int fldy, int fldx);
unsigned char getflddir(int fldy, int fldx);

#endif

#ifndef __CAR
#define __CAR


// car infomation

struct CARINF {
    unsigned char fld_y;
    unsigned char fld_x;
    unsigned char step_y;
    unsigned char step_x;
    unsigned char y;
    unsigned char x;
    unsigned char dirno;
    unsigned char chrno;
    unsigned char mode;
    unsigned char free;
};

#define CARINF_MAX  8
extern struct CARINF carinf[CARINF_MAX];
#define CARINF_SIZE 10   // sizeof(struct CARINF)の値を定義。asm用

extern unsigned char car_num;
extern unsigned char challenging_stage;


// car mode

#define CARM_NONE   0
#define CARM_WAIT   1
#define CARM_MOVE   CARM_WAIT+30
#define CARM_ROCK   CARM_MOVE+4
#define CARM_SMOKE  CARM_MOVE+8*3
#define CARM_EN2EN  CARM_MOVE+4


// smoke

#define SMOKENUM    12
#define SMOKETIME   30

extern unsigned char smokecnt;

struct SMOKEINF {
    unsigned char fld_y;
    unsigned char fld_x;
    unsigned char time;
};
extern struct SMOKEINF smokeinf[SMOKENUM];


// key

#define KBIT_UP  0x01
#define KBIT_DN  0x02
#define KBIT_LT  0x04
#define KBIT_RT  0x08
#define KBIT_ENT 0x10
#define KBIT_SPC 0x20


void initcar();
void dispcar();

unsigned char keycheck();
void movecar(int mycar_stp, int encar_stp);
int  collision();

void initsmoke();
int  getsmokeidx();
void setsmoke();
void clrsmoke();
void smokeproc();

#endif
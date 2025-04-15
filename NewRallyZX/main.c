#include <stdio.h>
#include <string.h>
#include <input.h>      // in_Wait

#include "map.h"
#include "scrn.h"
#include "car.h"


// phase

#define PHASE_TITLE     0
#define PHASE_START     1
#define PHASE_RESTART   2
#define PHASE_MAIN      3
#define PHASE_CRUSH     4
#define PHASE_JUDGE     5


// NEW logo

unsigned char logo1str[3][8] = 
{
  {0x82, 0x85,  0x85, 0x03, 0x01,  0x05, 0x05, 0x05},
  {0x05, 0x84,  0x85, 0x03, 0x01,  0x86, 0x86, 0x01},
  {0x01, 0x02,  0x02, 0x03, 0x01,  0x02, 0x02, 0x00}
};


// RALLY-ZX logo

unsigned char logo2str[3][18] =
{
  {0x07, 0x86,  0x87, 0x03, 0x04,  0x05, 0x00,  0x05, 0x02,  0x04, 0x06,  0x00, 0x02,  0x03, 0x07, 0x02,  0x04, 0x06},
  {0x07, 0x86,  0x85, 0x03, 0x05,  0x05, 0x00,  0x05, 0x00,  0x85, 0x00,  0x03, 0x01,  0x06, 0x00, 0x00,  0x06, 0x04},
  {0x01, 0x02,  0x02, 0x00, 0x01,  0x03, 0x01,  0x03, 0x01,  0x02, 0x00,  0x00, 0x02,  0x03, 0x03, 0x02,  0x00, 0x02}
};


// DSPINF

struct DSPINF {
  unsigned char chry;
  unsigned char chrx;
  unsigned char *chrp;
  unsigned char stry;
  unsigned char strx;
  char *strp;
};


// start title

struct DSPINF cast_chr[6] = {
  { 0, 0, NULL,                      7, 10, "CAST"},
  { 8, 3, &mycartbl[0][0][0],        9,  7, "MY CAR"},
  {11, 3, &encartbl[0][0][0],       12,  7, "RED CAR"},
  {14, 3, &chrtbl[CHR_NFLAG][0][0], 15,  7, "CHECK POINT"},
  {17, 3, &chrtbl[CHR_ROCK ][0][0], 18,  7, "ROCK (DANGER)"},
  {20, 3, &chrtbl[CHR_SMOKE][0][0], 21,  7, "SMOKE SCREEN"}
};


// stage title

struct DSPINF stage_chr[5] = {
  { 0, 0, NULL,                      4,  3, "CHALLENGING STAGE"},
  { 8, 7, &encartbl[0][0][0],        9, 11, "="},
  {12, 7, &chrtbl[CHR_ROCK ][0][0], 13, 11, "="},
  { 0, 0, NULL,                     17,  3, "RED CARS DONT MOVE"},
  { 0, 0, NULL,                     19,  2, "UNTIL FUEL RUNS OUT."}
};


// fuel definition

unsigned char fuelstr[] = {0x04,0x04,0x2b,0x3a,0x2a,0x31,0x87,0x87};


// round

struct ROUNDINFO {
    unsigned char map_num;
    unsigned char encar_num;
    unsigned char rock_num;
    unsigned char wait_num;
    unsigned char challenging_stage;
} roundinfo[15] = {
    {1,1,0, 4, 0}, {1,2,1, 2, 0},                {1,7, 5, 0, 1},
    {2,3,2, 1, 0}, {2,3,3, 1, 0}, {2,4,4, 0, 0}, {2,7, 7, 0, 1},
    {3,4,5, 0, 0}, {3,5,6, 0, 0}, {3,5,6, 0, 0}, {3,7,10, 0, 1},
    {4,4,6, 0, 0}, {4,5,6, 0, 0}, {4,5,6, 0, 0}, {4,7,10, 0, 1}
};


// game valiavles

unsigned int  score;
unsigned int  hscore;
int round;
int extend;
int rock_num;
int fuel_num;
int map_num;
int flag_num;
int wait_num;


void phase_title()
{
    int  n;

    // celar main
    clr_main();

    // draw logo
    for (n = 0; n < 3; n++) {
        memcpy(getscrnaddr(n, 1), &logo1str[n][0], sizeof(logo1str[0]));
        memcpy(getscrnaddr(n+3, 4), &logo2str[n][0], sizeof(logo2str[0]));
    }

    // draw cast
    for (n = 0; n < 6; n++) {
        if (cast_chr[n].chrp != NULL) {
            dispchr(cast_chr[n].chrp, cast_chr[n].chry, cast_chr[n].chrx);
        }
        copyascii2zx81chr(getscrnaddr(cast_chr[n].stry, cast_chr[n].strx), cast_chr[n].strp);
    }

    n = 0;
    while (!(keycheck() & (KBIT_ENT|KBIT_SPC))) {     // press enter or space to return
        if (n == 0) {
            copyascii2zx81chr(getscrnaddr(23, 5), "HIT SPACE KEY");
        }
        else if (n == 10) {
            copyascii2zx81chr(getscrnaddr(23, 5), "HIT       KEY");
        }
        else if (n == 20) {
            n = 0;
            continue;
        }
        randum();
        in_Wait(10);
        n++;
    }
}

void disp_stage()
{
    int  n;

    // celar main
    clr_main();

    // draw stage
    for (n = 0; n < 5; n++) {
        if (stage_chr[n].chrp != NULL) {
            dispchr(stage_chr[n].chrp, stage_chr[n].chry, stage_chr[n].chrx);
        }
        copyascii2zx81chr(getscrnaddr(stage_chr[n].stry, stage_chr[n].strx), stage_chr[n].strp);
    }

    copynum2zx81chr(getscrnaddr( 9, 13), car_num-1, 2);
    copynum2zx81chr(getscrnaddr(13, 13), rock_num, 2);
}

void dispscore()
{
    // hi-score
    if (hscore < score) {
        hscore = score;
    }
    if (hscore == 0) {
        copyascii2zx81chr(getscrnaddr(1, 25), "      0");
    }
    else {
        copyascii2zx81chr(getscrnaddr(1, 30), "00");
        copynum2zx81chr(getscrnaddr(1, 25), hscore, 5);
    }

    // score
    if (score == 0) {
        copyascii2zx81chr(getscrnaddr(3, 25), "      0");
    }
    else {
        copyascii2zx81chr(getscrnaddr(3, 30), "00");
        copynum2zx81chr(getscrnaddr(3, 25), score, 5);
    }

    // 1UP
    if (((extend == 0) && (score >=  200)) ||   //  20,000“_‚Å‚Ì1UP”»’è
        ((extend == 1) && (score >= 1000))) {   // 100,000“_‚Å‚Ì1UP”»’è
        extend++;
        live_num++;
        displive();
    }
}

void phase_start()
{
    struct ROUNDINFO *roundinfop;

    roundinfop = &roundinfo[(round - 1) % 15];
    map_num  = roundinfop->map_num;
    car_num  = roundinfop->encar_num+1; //Ž©ŽÔ•ª‚ð‰ÁŽZ
    rock_num = roundinfop->rock_num;
    wait_num = roundinfop->wait_num;
    if (round >= 15) {
        car_num = CAR_NUM;
        rock_num = 10;
        wait_num = 0;
    }

    challenging_stage = roundinfop->challenging_stage;
    if (challenging_stage != 0) {
        disp_stage();
        in_Wait(1000);
    }

    clr_main();
    initradar();
    makemap(map_num, rock_num);
    flag_num = 10;

    copyascii2zx81chr(getscrnaddr( 0, 24), "HI-SCORE");
    copyascii2zx81chr(getscrnaddr( 2, 24), "1UP");
    copyascii2zx81chr(getscrnaddr(23, 24), "ROUND");

    memcpy(getscrnaddr(4, 24), fuelstr, sizeof(fuelstr));
    copynum2zx81chr(getscrnaddr(23, 29), round, 3);
    dispscore();
}

void anim_fuelbonus()
{
    int  n;

    for (n = fuel_num; n >= 0; n--) {
        dispfuel(n);
        score++;
        dispscore();
        in_Wait(20);
    }
}

int main()
{
    unsigned char key;
    int  phase;
    int  fuel_cnt;
    int  doubleup, flag_pts;
    int  move_cnt;
    int  ret;

    hscore = 0;
    phase = PHASE_TITLE;

    while (1) {     // press enter to exit
        key = keycheck();
        if (key & KBIT_ENT) {
            break;
        }
        if (phase != PHASE_MAIN) {
            switch (phase) {
                case PHASE_TITLE:
                    phase_title();
                    phase = PHASE_START;
                    round = 1;
                    live_num = 2;
                    score = 0;
                    extend = 0;
                    continue;

                case PHASE_START:
                    phase_start();
                    // no break;
                case PHASE_RESTART:
                    displive();
                    fuel_num=32;
                    fuel_cnt=0;
                    dispfuel(fuel_num);
                    initcar();
                    initsmoke();
                    initcourse();
                    dispradar(ALLON);
                    in_Wait(500);
                    phase = PHASE_MAIN;
                    move_cnt = 0;
                    doubleup = OFF;
                    flag_pts = 0;
                    break;

                default:
                    break;
            }
            continue;
        }

        if ((fuel_num != 0) && (move_cnt == 0)) {
            movecar(0, 1);
        }
        else {
            if ((fuel_num == 0) && ((move_cnt % 2)== 0)) {
                challenging_stage = 0;
                movecar(0, 1);
            }
            else {
                movecar(1, 1);
            }
            dispradar(ON);

            fuel_cnt++;
            if (fuel_cnt > 40) {
                fuel_cnt = 0;
                if (fuel_num != 0) {
                    fuel_num--;
                }
                dispfuel(fuel_num);
            }
            dispcourse(carinf[0].fld_y, carinf[0].fld_x, carinf[0].step_y, carinf[0].step_x);
            dispcar();
            smokeproc();

            dispradar(OFF);
        }

        move_cnt++;
        if (move_cnt >= 8) {  
            move_cnt = 0;
        }

        ret = collision();
        if (ret == 0x04) {
            if (smokecnt == 3) {
                if (fuel_num >= 2) {
                    fuel_num -= 2;
                    dispfuel(fuel_num);
                    setsmoke();
                }
                else {
                    smokecnt = 0;
                }
            }
            else {
                setsmoke();
            }
        }
        if (ret & 0x02) {
            if (ret == 0x02) {      // Sflag
                doubleup = ON;
            }
            // radar off
            setradar(carinf[0].fld_y, carinf[0].fld_x, OFF);
            // flag clear
            fldtbl[carinf[0].fld_y][carinf[0].fld_x] = CHR_SPC;
            // flag point
            flag_num--;
            flag_pts++;
            copynum2zx81chr(&bakbuf[4*CHR_DY][4*CHR_DX-1], flag_pts*100, 4);
            copyascii2zx81chr(&bakbuf[4*CHR_DY+1][4*CHR_DX], (doubleup == ON) ? " X2" : "   ");
            copyascii2zx81chr(&bakbuf[4*CHR_DY+2][4*CHR_DX], "   ");
            score += flag_pts;
            if (doubleup== ON) {
                score += flag_pts;
            }
            dispscore();
            dispcourse(carinf[0].fld_y, carinf[0].fld_x, carinf[0].step_y, carinf[0].step_x);
            if (flag_num <= 0) {
                anim_fuelbonus();
                dispcourse(carinf[0].fld_y, carinf[0].fld_x, carinf[0].step_y, carinf[0].step_x);
                in_Wait(500);
                round++;
                phase = PHASE_START;
            }
            else if (ret == 0x0a) {      // Lflag
                anim_fuelbonus();
                in_Wait(100);
                dispfuel(fuel_num);
            }
        }

        if (ret & 0x01) {
            copyascii2zx81chr(getscrnaddr(3*CHR_DY+2, 3*CHR_DX), "BANG");
            clrsmoke();
            in_Wait(500);
            live_num--;
            if (live_num >= 0) {
                if (roundinfo[(round-1) % 15].challenging_stage == 0) {
                    phase = PHASE_RESTART;
                }
                else {
                    round++;
                    phase = PHASE_START;
                }
            }
            else {
                copyascii2zx81chr(getscrnaddr(3*CHR_DY+2, 3*CHR_DX), "GAME");
                copyascii2zx81chr(getscrnaddr(3*CHR_DY+3, 3*CHR_DX), "OVER");
                in_Wait(1000);
                phase = PHASE_TITLE;
            }
        }

        if (wait_num != 0) {
            in_Wait(wait_num);
        }
    }

    return 0;
}
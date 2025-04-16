#include <input.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <zx81.h>

#define _countof(array)    (sizeof(array) / sizeof(array[0]))

// video memory

#define D_FILE          0x400c
#define D_DX            (32 + 1)
#define D_DY            24

unsigned char *scrnlinep[D_DY];

// character table

#define CHR_DX          7
#define CHR_DY          7

#define CHR_HOLE        0
#define CHR_HAMMER_DSP  1
#define CHR_HAMMER_CLR  2
#define CHR_HEAD1       3
#define CHR_HEAD2       4
#define CHR_HEAD3       5
#define CHR_HEAD4       6
#define CHR_HEAD_CLR    7
#define CHR_HIP1        8
#define CHR_HIP2        9
#define CHR_HIP3        10
#define CHR_HIP4        11
#define CHR_HIP_CLR     12
#define CHR_HEAD1_HIT   13
#define CHR_HEAD2_HIT   14
#define CHR_HEAD3_HIT   15
#define CHR_HEAD4_HIT   16
#define CHR_HIP1_HIT    17
#define CHR_HIP2_HIT    18
#define CHR_HIP3_HIT    19
#define CHR_HIP4_HIT    20

static unsigned char chrtbl[][CHR_DX*CHR_DY] = {
//0: hole
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x08, 0x08, 0x09, 0x09, 0x09, 0x08, 0x08},
//1: hammer display
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x02, 0x89, 0x03, 0x03, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x08, 0x09, 0x09, 0x09, 0x08, 0x08},
//2: hummer clear
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x08, 0x09, 0x09, 0x09, 0x08, 0x08},
//3: head1
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x87, 0xB4, 0x80, 0xB4, 0x04, 0x00,
     0x00, 0x85, 0x94, 0xa8, 0x94, 0x05, 0x00,
     0x00, 0x85, 0x8B, 0x80, 0x8B, 0x05, 0x00,
     0x00, 0x85, 0x80, 0x80, 0x80, 0x05, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//4: head2
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x87, 0xB4, 0x80, 0xB4, 0x04, 0x00,
     0x00, 0x85, 0x94, 0xa8, 0x94, 0x05, 0x00,
     0x00, 0x85, 0x8B, 0x80, 0x8B, 0x05, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//5: head3
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x87, 0xB4, 0x80, 0xB4, 0x04, 0x00,
     0x00, 0x85, 0x94, 0xa8, 0x94, 0x05, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//6: head4
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x87, 0xB4, 0x80, 0xB4, 0x04, 0x00,
     0x88, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//7: head clear
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x08, 0x08, 0x09, 0x09, 0x09, 0x08, 0x08},
//8: hip1
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
     0x00, 0x87, 0x80, 0x80, 0x80, 0x04, 0x00,
     0x00, 0x85, 0x80, 0x80, 0x80, 0x05, 0x00,
     0xff, 0x85, 0x80, 0x80, 0x80, 0x05, 0x00,
     0xff, 0x85, 0x80, 0x80, 0x80, 0x05, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//9: hip2
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
     0x00, 0x87, 0x80, 0x80, 0x80, 0x04, 0x00,
     0x00, 0x85, 0x80, 0x80, 0x80, 0x05, 0x00,
     0xff, 0x85, 0x80, 0x80, 0x80, 0x05, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//10: hip3
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
     0x00, 0x87, 0x80, 0x80, 0x80, 0x04, 0x00,
     0x00, 0x85, 0x80, 0x80, 0x80, 0x05, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//11: hip4
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
     0x00, 0x87, 0x80, 0x80, 0x80, 0x04, 0x00,
     0x88, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//12: hip clear
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x08, 0x08, 0x09, 0x09, 0x09, 0x08, 0x08},
//13: head1 hit
    {0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x02, 0x89, 0x03, 0x03, 0x00,
     0x00, 0x87, 0x92, 0x80, 0x93, 0x04, 0x00,
     0x00, 0x80, 0x94, 0xa8, 0x94, 0x80, 0x00,
     0x00, 0x80, 0x80, 0xbd, 0x80, 0x80, 0x00,
     0x00, 0x84, 0x80, 0x80, 0x80, 0x07, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//14: head2 hit
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x02, 0x89, 0x03, 0x03, 0x00,
     0x00, 0x87, 0x92, 0x80, 0x93, 0x04, 0x00,
     0x00, 0x80, 0x94, 0xa8, 0x94, 0x80, 0x00,
     0x00, 0x80, 0x80, 0xbd, 0x80, 0x80, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//15: head3 hit
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x02, 0x89, 0x03, 0x03, 0x00,
     0x00, 0x87, 0x92, 0x80, 0x93, 0x04, 0x00,
     0x00, 0x80, 0x94, 0xa8, 0x94, 0x80, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//16: head4 hit
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x02, 0x89, 0x03, 0x03, 0x00,
     0x00, 0x81, 0x92, 0x80, 0x93, 0x82, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//17: hip1 hit
    {0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x02, 0x89, 0x03, 0x03, 0x00,
     0x00, 0x87, 0x80, 0x80, 0x80, 0x04, 0x00,
     0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
     0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
     0x00, 0x84, 0x80, 0x80, 0x80, 0x07, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//18: hip2 hit
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x02, 0x89, 0x03, 0x03, 0x00,
     0x00, 0x87, 0x80, 0x80, 0x80, 0x04, 0x00,
     0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
     0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//19: hip3 hit
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x02, 0x89, 0x03, 0x03, 0x00,
     0x00, 0x87, 0x80, 0x80, 0x80, 0x04, 0x00,
     0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88},
//20: hip4 hit
    {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x02, 0x89, 0x03, 0x03, 0x00,
     0x00, 0x81, 0x80, 0x80, 0x80, 0x82, 0x00,
     0xff, 0x88, 0x89, 0x89, 0x89, 0x88, 0x88}
};

// motion table

#define PTN_NUM         2
#define PTN_HEAD        0
#define PTN_HIP         1

struct {
    int molechr[PTN_NUM];
    int hitchr[PTN_NUM];
    int point[PTN_NUM];
} mottbl[] = {
    {{CHR_HEAD4,    CHR_HIP4},    {CHR_HEAD4_HIT,  CHR_HIP4_HIT},   {40, -40}},
    {{CHR_HEAD3,    CHR_HIP3},    {CHR_HEAD3_HIT,  CHR_HIP3_HIT},   {30, -30}},
    {{CHR_HEAD2,    CHR_HIP2},    {CHR_HEAD2_HIT,  CHR_HIP2_HIT},   {20, -20}},
    {{CHR_HEAD1,    CHR_HIP1},    {CHR_HEAD1_HIT,  CHR_HIP1_HIT},   {10, -10}},
    {{CHR_HEAD1,    CHR_HIP1},    {CHR_HEAD1_HIT,  CHR_HIP1_HIT},   {10, -10}},
    {{CHR_HEAD2,    CHR_HIP2},    {CHR_HEAD2_HIT,  CHR_HIP2_HIT},   {20, -20}},
    {{CHR_HEAD3,    CHR_HIP3},    {CHR_HEAD3_HIT,  CHR_HIP3_HIT},   {30, -30}},
    {{CHR_HEAD4,    CHR_HIP4},    {CHR_HEAD4_HIT,  CHR_HIP4_HIT},   {40, -40}},
    {{CHR_HEAD_CLR, CHR_HIP_CLR}, {-1, -1}, { 0,   0}}
};

// hole information

#define ACT_NONE        0
#define ACT_HEAD        1
#define ACT_HIP         2
#define ACT_HAMMER_CLR  3
#define ACT_MOLE_CLR    4

struct {
    int x;
    int y;
    int key;
    int press;
    int actno;
    int motcnt;
    int wait;
} holeinfo[] = {
    { 0,  0, 0x08ef, 0, 0, 0, 0}, { 8,  0, 0x04ef, 0, 0, 0, 0}, {16,  0, 0x02ef, 0, 0, 0, 0},
    { 0,  7, 0x08f7, 0, 0, 0, 0}, { 8,  7, 0x10f7, 0, 0, 0, 0}, {16,  7, 0x10ef, 0, 0, 0, 0},
    { 0, 14, 0x01f7, 0, 0, 0, 0}, { 8, 14, 0x02f7, 0, 0, 0, 0}, {16, 14, 0x04f7, 0, 0, 0, 0}
};

// ASCII to ZX81-character

#define ASCII_BGN   32
#define ASCII_END   (ASCII_BGN + sizeof(zx81chr) - 1)
static unsigned char zx81chr[] = {
//   (32),!(33),"(34),#(35),$(36)
    0x00, 0x00, 0x0b, 0x00, 0x0d,
//  %(37),&(38),'(39),((40),)(41)
    0X00, 0X00, 0X00, 0x10, 0x11,
//  *(42),+(43),,(44),-(45),.(46)
    0x17, 0x15, 0x1a, 0x16, 0x1b,
//  /(47),0(48),1(49),2(50),3(51)
    0x18, 0x1c, 0x1d, 0x1e, 0x1f,
//  4(52),5(53),6(54),7(55).8(56)
    0x20, 0x21, 0x22, 0x23, 0x24,
//  9(57),:(58),;(59),<(60),=(61)
    0x25, 0x0e, 0x19, 0x13, 0x14,
//  >(62),?(63),@(64),A(65),B(66)
    0x12, 0x0f, 0x0c, 0x26, 0x27,
//  C(67),D(68),E(69),F(70),G(71)
    0x28, 0x29, 0x2a, 0x2b, 0x2c,
//  H(72),I(73),J(74),K(75),L(76)
    0x2d, 0x2e, 0x2f, 0x30, 0x31,
//  M(77),N(78),O(79),P(80),Q(81)
    0x32, 0x33, 0x34, 0x35, 0x36,
//  R(82),S(83),T(84),U(85),V(86)
    0x37, 0x38, 0x39, 0x3a, 0x3b,
//  W(87),X(88),Y(89),Z(90)
    0x3c, 0x3d, 0x3e, 0x3f
};

// phase

#define PHASE_TITLE     0
#define PHASE_MAIN      1
#define PHASE_JUDGE     2

int hiscore;
int score;
int roundpoint;

struct {
    int passingpoint;
    int molenum;
    int molewait;
    int inwait;
} roundinfo[] = {
    {100, 10, 4, 10},
    {110, 11, 3, 10},
    {120, 12, 2, 10},
    {130, 13, 2, 8},
    {140, 14, 2, 5},
    {150, 15, 2, 4},
    {160, 16, 2, 3},
    {170, 17, 2, 2},
    {180, 18, 2, 1},
    {190, 19, 1, 1},
    {200, 20, 0, 1}
};

void clrfield()
{
    unsigned char *scrnp;
    int  i;

    scrnp = scrnlinep[0];
    for (i = 0; i < 22; i++) {
        memset(scrnp, 0x00, 23);
        scrnp[23] = 0x0e;
        scrnp += D_DX;
    }
}

void dispsubtitle()
{
    struct {
        int  x;
        int  y;
        char *p;
    } strtbl[] = {
        {24,  0, "========"},
        {24,  1, "  mole  "},
        {24,  2, " attack "},
        {24,  3, "========"},
        {24,  5, "hi-score"},
        {24,  7, "score   "},
        {24, 10, "round   "},
        {24, 11, "   point"},
        {24, 13, "passing "},
        {24, 14, "   point"},
        {24, 17, "round   "},
        {24, 19, "mole    "}
    };
    int  i;

    for (i = 0; i < _countof(strtbl); i++) {
        zx_setcursorpos(strtbl[i].y, strtbl[i].x);
        printf(strtbl[i].p);
    }
}

void setchrpoint(int chrno, int point)
{
    unsigned char str[16];
    int  i, len;

    sprintf(str, "%d", point);
    len = strlen(str);
    for (i = 0; i < len; i++) {
        if ((str[i] >= ASCII_BGN) && (str[i] <= ASCII_END)) {
            str[i] = zx81chr[str[i]-ASCII_BGN];
        }
        else {
            str[i] = 0;
        }
    }
    memcpy(&chrtbl[chrno][0], str, len);
}

void printnum(int y, int x, int num)
{
    unsigned char str[16];
    int  i, len;

    sprintf(str, "%7d", num);
    len = strlen(str);
    for (i = 0; i < len; i++) {
        if ((str[i] >= ASCII_BGN) && (str[i] <= ASCII_END)) {
            str[i] = zx81chr[str[i]-ASCII_BGN];
        }
        else {
            str[i] = 0;
        }
    }

    memcpy(scrnlinep[y] + x, str, len);
}

void dispscore(int hisc, int sc, int rdpt, int pspt, int rd, int ml)
{
    if (hisc != -1) {
        printnum(6, 24, hisc);
    }
    if (sc != -1) {
        printnum(8, 24, sc);
    }
    if (rdpt != -1) {
        printnum(12, 24, rdpt);
    }
    if (pspt != -1) {
        printnum(15, 24, pspt);
    }
    if (rd != -1) {
        printnum(18, 24, rd);
    }
    if (ml != -1) {
        printnum(20, 24, ml);
    }
}

void disphole(int holeno, int chrno)
{
    unsigned char *scrnp, *chrp;
    int  i;

    if ((holeno < 0) || (holeno >= _countof(holeinfo))) {
        return;
    }
    if ((chrno < 0) || (chrno >= _countof(chrtbl))) {
        return;
    }

    scrnp = scrnlinep[holeinfo[holeno].y] + holeinfo[holeno].x;
    chrp = &chrtbl[chrno][0];
    for (i = 0; i < CHR_DY; i++) {
        if (chrp[0] != 0xff) {
            memcpy(scrnp, chrp, CHR_DX);
        }
        scrnp += D_DX;
        chrp += CHR_DX;
    }
}

int phase_title()
{
    struct {
        int  x;
        int  y;
        char *p;
    } strtbl[] = {
        {3,  2, "7 8 9"},
        {3,  4, "4 5 6 to hit mole"},
        {3,  6, "1 2 3"},
        {1, 17, "push NEWLINE to start"}
    };
    int  i;
    int  motcnt[3], motptn[3];
    int  wait[3], molewait[3];

    clrfield();
    dispsubtitle();
    dispscore(hiscore, score, -1, -1, -1, -1);

    for (i = 0; i < _countof(strtbl); i++) {
        zx_setcursorpos(strtbl[i].y, strtbl[i].x);
        printf(strtbl[i].p);
    }

    for (i = 0; i < 3; i++) {
        disphole(i + 3, CHR_HOLE);
    }

    i = 0;
    memset(motcnt, 0, sizeof(motcnt));
    memset(motptn, 0, sizeof(motptn));
    memset(wait, 0, sizeof(wait));
    memset(molewait, 0, sizeof(molewait));

    while ((in_KeyPressed(0x01bf) == 0) &&     // press enter to exit
           (in_KeyPressed(0x017f) == 0)) {     // press SPC   to exit
        for (i = 0; i < 3; i++) {
            if (wait[i] > 0) {
                wait[i]--;
                continue;
            }
            else {
                if (motcnt[i] == 0) {
                    if (rand() % 50 >= 1) {
                        continue;
                    }
                    molewait[i] = rand() % 10;
                    motptn[i] = rand() % 2;
                }
                wait[i] = molewait[i];

                disphole(i + 3, mottbl[motcnt[i]].molechr[motptn[i]]);
                motcnt[i]++;
                if (motcnt[i] >= _countof(mottbl)) {
                    motcnt[i] = 0;
                }
            }
        }
        in_Wait(1);    // wait(msec)
    }

    return (PHASE_MAIN);
}

int phase_main(int round)
{
    int  i, molenum;
    int  motptn, point;

    molenum = roundinfo[round-1].molenum;
    dispscore(hiscore, score, roundpoint, roundinfo[round-1].passingpoint, round, molenum);

    clrfield();

    zx_setcursorpos(8, 8);
    printf("round:%d", round);
    in_Wait(500);    // wait(msec)

    clrfield();

    for (i = 0; i < _countof(holeinfo); i++) {
        disphole(i, CHR_HOLE);
    }

    while (in_KeyPressed(0x017f) == 0) {     // press SPC   to exit
        if (molenum <= 0) {
            for (i = 0; i < _countof(holeinfo); i++) {
                if (holeinfo[i].actno != ACT_NONE) {
                    break;
                }
            }
            if (i >= _countof(holeinfo)) {
                break;
            }
        }

        if ((rand() % 10 < 1) && (molenum > 0)) {
            i = rand() % _countof(holeinfo);
            if (holeinfo[i].actno == ACT_HAMMER_CLR) {
                disphole(i, CHR_HAMMER_CLR);
                holeinfo[i].actno = 0;
            }
            if (holeinfo[i].actno == 0) {
                holeinfo[i].actno = ((rand() % 2) == 0) ? ACT_HEAD : ACT_HIP;
                holeinfo[i].motcnt = 0;
                holeinfo[i].wait = 0;
                if (holeinfo[i].actno == ACT_HEAD) {
                    molenum--;
                    dispscore(-1, -1, -1, -1, -1, molenum);
                }
            }
        }

        for (i = 0; i < _countof(holeinfo); i++) {
            if (holeinfo[i].wait > 0) {
                holeinfo[i].wait--;
            }
            else {
                switch (holeinfo[i].actno) {
                    case ACT_NONE:
                        break;

                    case ACT_HEAD:
                    case ACT_HIP:
                        motptn = (holeinfo[i].actno == ACT_HEAD) ? PTN_HEAD : PTN_HIP;
                        disphole(i, mottbl[holeinfo[i].motcnt].molechr[motptn]);
                        holeinfo[i].motcnt++;
                        if (holeinfo[i].motcnt < _countof(mottbl)) {
                            holeinfo[i].wait = roundinfo[round-1].molewait;
                        }
                        else {
                            holeinfo[i].actno = ACT_NONE;
                            holeinfo[i].motcnt = 0;
                            holeinfo[i].wait = 0;
                        }
                        break;

                    case ACT_HAMMER_CLR:
                        disphole(i, CHR_HAMMER_CLR);
                        holeinfo[i].actno = ACT_NONE;
                        holeinfo[i].motcnt = 0;
                        holeinfo[i].wait = 0;
                        break;

                    case ACT_MOLE_CLR:
                        disphole(i, CHR_HOLE);
                        holeinfo[i].actno = ACT_NONE;
                        holeinfo[i].motcnt = 0;
                        holeinfo[i].wait = 0;
                        break;

                    default:
                        break;
                }
            }

            if (in_KeyPressed(holeinfo[i].key) != 0) {
                if (holeinfo[i].press == 0) {
                    switch (holeinfo[i].actno) {
                        case ACT_NONE:
                            disphole(i, CHR_HAMMER_DSP);
                            holeinfo[i].press = 1;
                            holeinfo[i].actno = ACT_HAMMER_CLR;
                            holeinfo[i].motcnt = 0;
                            holeinfo[i].wait = 2;
                            break;

                        case ACT_HEAD:
                        case ACT_HIP:
                            motptn = (holeinfo[i].actno == ACT_HEAD) ? PTN_HEAD : PTN_HIP;
                            point = mottbl[holeinfo[i].motcnt-1].point[motptn];
                            disphole(i, mottbl[holeinfo[i].motcnt-1].hitchr[motptn]);
                            roundpoint = (roundpoint + point < 0) ? 0 : roundpoint + point;
                            dispscore(-1, -1, roundpoint, -1, -1, -1);
                            holeinfo[i].press = 1;
                            holeinfo[i].actno = ACT_MOLE_CLR;
                            holeinfo[i].motcnt = 0;
                            holeinfo[i].wait = 4;
                            break;

                        default:
                            break;
                    }
                }
            }
            else {
                holeinfo[i].press = 0;
            }
        }
        in_Wait(roundinfo[round-1].inwait);    // wait(msec)
    }

    return (PHASE_JUDGE);
}

int phase_judge(int round)
{
    int  bonus, judge;
    int  inwait, i;

    inwait = 200;

    zx_setcursorpos(8, 8);
    printf("  %4d", roundpoint);
    in_Wait(inwait);    // wait(msec)

    zx_setcursorpos(9, 8);
    printf(" -%4d", roundinfo[round-1].passingpoint);
    in_Wait(inwait);    // wait(msec)

    bonus = roundpoint - roundinfo[round-1].passingpoint;
    judge = 0;
    if (bonus < 0) {
        bonus = 0;
        judge = 1;
    }
    zx_setcursorpos(10, 8);
    printf("-------");
    zx_setcursorpos(11, 8);
    printf("  %4d", bonus);
    in_Wait(inwait);

    zx_setcursorpos(15, 4);
    printf("BONUS %4d POINTS", bonus);
    score += bonus + roundpoint;
    if (hiscore < score) {
        hiscore = score;
    }
    dispscore(hiscore, score, -1, -1, -1, -1);
    in_Wait(inwait*2);

    if (judge == 1) {
        zx_setcursorpos(17, 7);
        printf("GAME OVER");
        in_Wait(inwait * 4);
        return (PHASE_TITLE);
    }

    if (round >= _countof(roundinfo)) {
        for (i = 0; i < 5; i++) {
            zx_setcursorpos(17, 5);
            printf("CONGRATULATIONS");
            in_Wait(inwait/2);
            zx_setcursorpos(17, 5);
            printf("congratulations");
            in_Wait(inwait/2);
        }
        return (PHASE_TITLE);
    }

    return (PHASE_MAIN);

}

int main()
{
    int  i, phase;
    int  round;

    srand((unsigned int)clock());

    hiscore = 0;
    score = 0;
    roundpoint = 0;

    for (i = 0; i < _countof(scrnlinep); i++) {
        scrnlinep[i] = (unsigned char *)((*((unsigned int *)D_FILE) + 1 + i * D_DX));
    }

    for (i = 0; i < _countof(mottbl); i++) {
        if (mottbl[i].point[0] != 0) {
            setchrpoint(mottbl[i].hitchr[0], mottbl[i].point[0]);
        }
        if (mottbl[i].point[1] != 0) {
            setchrpoint(mottbl[i].hitchr[1], mottbl[i].point[1]);
        }
    }

    phase = PHASE_TITLE;

    while (in_KeyPressed(0x017f) == 0) {     // press SPC to exit
        switch (phase) {
            case PHASE_TITLE:
                phase = phase_title();
                score = 0;
                round = 0;
                srand((unsigned int)clock());
                break;

            case PHASE_MAIN:
                roundpoint = 0;
                round++;
                phase = phase_main(round);
                break;

            case PHASE_JUDGE:
                phase = phase_judge(round);
                break;
        
            default:
                break;
        }
    }

    return (0);
}


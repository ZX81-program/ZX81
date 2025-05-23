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

int main()
{
    int  i;
    int  motptn, molenum;
    int  sc, ro, point;

    srand((unsigned int)clock());

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

	for (i = 0; i < 100; i++) {
		 zx_setcursorpos(0,0);
        printf("%7d", i);
	}
	for (i = 0; i < 100; i++) {
		printnum(0,0, i);
	}
    clrfield();
    dispsubtitle();
    dispscore(500, 0, 0, 150, 1, 30);
    for (i = 0; i < _countof(holeinfo); i++) {
        disphole(i, CHR_HOLE);
    }

    sc = 0;
    ro = 0;
    molenum = 30;

    while (in_KeyPressed(0x01bf) == 0) {     // press enter to exit
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
                            holeinfo[i].wait = 1;
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
//                            zx_setcursorpos(holeinfo[i].y, holeinfo[i].x);
//                            printf("%d", point);
                            ro = (ro + point < 0) ? 0 : ro + point;
                            dispscore(-1, -1, ro, -1, -1, -1);
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
        in_Wait(10);    // wait(msec)
    }

    return (0);
}


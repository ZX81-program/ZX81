/*
 *      ZX81 Screen Handling Module program
 */

#include <stdio.h>
#include <string.h>

#include "scrn.h"
#include "map.h"
#include "key.h"

// ASCII to ZX81-character

#define ASCII_BGN   32
#define ASCII_END   (ASCII_BGN + sizeof(zx81chr) - 1)
unsigned char zx81chr[] = {
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
#define ZX81CHR_SIZE    59

unsigned int  score = 0;
unsigned int  hiscore = 0;
unsigned int  round = 1;
unsigned int  boat_lives = 3;
unsigned int  boat_extend = 0;
unsigned int  game_speed = 1;
static unsigned char boat_lives_tbl[2][2] =
{
    {0x81, 0x04},
    {0x80, 0x05}
};

// ----------------------------------------------------------
// èâä˙âª
// ----------------------------------------------------------
void scrn_init(void)
{
    struct HUD_STR {
        char y;
        char x;
        char step;
        char *p;
    } hud_strtbl[] =
    {
        { 6,  1, D_DX, "WATER GAUGE"},
        { 6,  3, D_DX, "GOAL  METER"},
        { 1, 27, 1,   "1UP"},
        { 3, 31, 1,       "0"},
        { 6, 27, 1,   "TOP"},
        { 8, 31, 1,       "0"},
        {11, 26, 1,  "ROUNDS"},
        {20, 26, 1,  "SPEED"}
    };
    struct HUD_STR *hud_strp;

    // HUD draw
    for (int i = 0; i < sizeof(hud_strtbl) / sizeof(hud_strtbl[0]); i++)
    {
        hud_strp = &hud_strtbl[i];
        zx81_write_str(getscrnaddr(hud_strp->y, hud_strp->x), hud_strp->p, hud_strp->step);
    }

    // score draw
    score_draw();

    // rounds draw
    rounds_draw();

    // boat lives draw
    memcpy(getscrnaddr(16, 27), &boat_lives_tbl[0][0], sizeof(boat_lives_tbl[0]));
    memcpy(getscrnaddr(17, 27), &boat_lives_tbl[1][0], sizeof(boat_lives_tbl[0]));
    boat_lives_draw();

    // game speed draw
    game_speed_draw();
}

#asm
;----------------------------------------
; HL = HL * 33
;----------------------------------------
_HL_MULT33:
    push    de

    ld      d, h
    ld      e, l
    add     hl, hl      ; *2
    add     hl, hl      ; *4
    add     hl, hl      ; *8
    add     hl, hl      ; *16
    add     hl, hl      ; *32
    add     hl, de      ; *33

    pop     de
    ret

#endasm

unsigned char* getscrnaddr(int posy, int posx)
{
#asm
    ld      ix, 2
    add     ix, sp

    push    de
    ld      d, (ix+2)       ; d=posy
    ld      e, (ix+0)       ; e=posx
    call    getscrnaddr_calc
    pop     de
    ret

getscrnaddr_calc:
; IN:  D=posy, E=posx
; OUT: HL= screen address
    push    bc

    ; HL = posy * 33
    ld      h, 0
    ld      l, d
    call    _HL_MULT33

    ; HL += posx
    ld      b, 0
    ld      c, e            ; c=posx
    add     hl, bc

    ; HL += D_FILE
    ld      bc, (D_FILE)
    inc     bc
    add     hl, bc

    pop     bc
    ret

#endasm
}

void clear_game_screen()
{
    #asm
        ld      c,24
        ld      d,0
        ld      e,4
clear_game_screen_yloop:
        call    getscrnaddr_calc

        ld      b,22
clear_game_screen_xloop:
        ld      (hl),0
        inc     hl
        djnz    clear_game_screen_xloop  

        inc     d
        dec     c
        jp      nz,clear_game_screen_yloop
    #endasm
}

void zx81_write_str(unsigned char *dstp, const char *asciip, int step)
{
    #asm
        ld      ix,2
        add     ix,sp

        ld      e, (ix+0)
        ld      d, (ix+1)       ; step
        ld      c, (ix+2)
        ld      b, (ix+3)       ; asciip
        ld      l, (ix+4)
        ld      h, (ix+5)       ; dstp

zx81_write_str_loop:
        ld      a,(bc)
        cp      0
        ret     z

        push    de

        sub     ASCII_BGN
        jp      c,zx81_write_str_2
        cp      ZX81CHR_SIZE    ;sizeof(zx81chr)
        jp      nc,zx81_write_str_2

        ld      de,_zx81chr
        add     a,e
        jp      nc,zx81_write_str_1
        inc     d
zx81_write_str_1:
        ld      e,a
        ld      a,(de)
        jp      zx81_write_str_3

zx81_write_str_2:
//        ld      a,0
        ld      a,(bc)
zx81_write_str_3:
        ld      (hl),a

        pop     de

        inc     bc
        add     hl,de

        jp      zx81_write_str_loop

    #endasm
}

void zx81_write_num(unsigned char *dstp, unsigned int num, int ndigit, int fill_zero)
{
    unsigned char ascii[6+1];
    int  i, n;

    memset(ascii, 0, sizeof(ascii));

    if (ndigit > 6)
    {
        return;
    }

    i = ndigit-1;
    while (i >= 0) {
        n = num % 10;
        num /= 10;
        ascii[i] = '0' + n;
        i--;
    }

    if (!fill_zero)
    {
        i = 0;
        while (i < ndigit - 1 && ascii[i] == '0')
        {
            ascii[i] = ' ';
            i++;
        }
    }

    zx81_write_str(dstp, ascii, 1);
}

void score_draw()
{
    unsigned char *p;

    // score draw
    p = getscrnaddr(3, 26);
    zx81_write_num(p, score, 5, 1);

    // hi-score draw
    if (hiscore <= score)
    {
        hiscore = score;
        memcpy(getscrnaddr(8, 26), p, 5);
    }

    // 1UP
    if (((boat_extend == 0) && (score >=  2000)) ||     //  20,000ì_Ç≈ÇÃ1UPîªíË
        ((boat_extend == 1) && (score >= 10000))) {     // 100,000ì_Ç≈ÇÃ1UPîªíË
        boat_extend++;
        boat_lives++;
        boat_lives_draw();
    }
}

void rounds_draw()
{
    zx81_write_num(getscrnaddr(13, 28), round, 2, 1);
}

void boat_lives_draw()
{
    zx81_write_num(getscrnaddr(17, 29), boat_lives, 1, 1);
}

void game_speed_draw()
{
    const unsigned char rvs_str[][7] = {
        {0x9d, 0x8e, 0xab, 0xa6, 0xb8, 0xb9, 0x00},
        {0x9e, 0x8e, 0xb2, 0xae, 0xa9, 0x00, 0x00},
        {0x9f, 0x8e, 0xb8, 0xb1, 0xb4, 0xbc, 0x00}
    };
    const struct SPEED_TBL {
        char y;
        char x;
        unsigned char *strp;
    } speed_tbl[] =
    {
        {21, 26, "1:FAST"},
        {22, 26, "2:MID"},
        {23, 26, "3:SLOW"}
    };
    struct SPEED_TBL *tblp;
    unsigned char *strp;
    int  i;

    for (i = 0; i < sizeof(speed_tbl) / sizeof(speed_tbl[0]); i++) {
        tblp = &speed_tbl[i];
        if (i == game_speed) {
            strp = &rvs_str[i][0];
        }
        else {
            strp = tblp->strp;
        }
        zx81_write_str(getscrnaddr(tblp->y, tblp->x), strp, 1);
    }
}

void game_speed_set(unsigned char key)
{
    if (key & KBIT_1) {
        game_speed = 0;
    }
    else if (key & KBIT_2) {
        game_speed = 1;
    }
    else if (key & KBIT_3) {
        game_speed = 2;
    }
    else {
        return;
    }
    game_speed_draw();
}

int goal_level = 0;
int goal_count = 0;

void goal_meter_init(void)
{
    unsigned char *p;
    int  i;

    goal_level = 96;
    goal_count = 2;

    p = getscrnaddr(0, 2);
    for (i = 0; i <= 24; i++)
    {
        *p = 0x80;
        p += D_DX;
    }

}

void goal_meter_draw(int dy)
{
    unsigned char goal_ptn[5] = {0x00, 0x02, 0x03, 0x84, 0x80};
    int  y;
    int  ptnno;
    unsigned char *p, c;
    int  row, step;

    if (dy < 0)
    {
        step = -dy;
        if (goal_level == 0)
        {
            return;
        }

        if (goal_count > step)
        {
            goal_count -= step;
            return;
        }
        goal_count += 10 - step;

        goal_level--;

        row = goal_level >> 2;
        ptnno = goal_level & 0x03;
    }
    else if (dy > 0)
    {
        step = dy;

        goal_count += step;
        if (goal_count < 10)
        {
            return;
        }
        goal_count -= 10;

        goal_level++;

        row = goal_level >> 2;
        if (goal_level & 0x03)
        {
            ptnno = goal_level & 0x03;
        }
        else
        {
            if (row < 1)
            {
                return;
            }
            row --;
            ptnno = 4;
        }
    }
    else
    {
        return;
    }
    p = getscrnaddr(row, 2);
    *p = goal_ptn[ptnno];
}

int water_level = 0;
int water_count = 0;

void water_gauge_init(void)
{
    unsigned char *p;
    int  row;

    water_level = 96;
    water_count = 50;

    p = getscrnaddr(0, 0);
    for (row = 0; row <= 24; row++)
    {
        *p = 0x80;
        p += D_DX;
    }

}

void water_gauge_draw(int water_leak_level)
{
    unsigned char ptn[4] = {0x00, 0x02, 0x03, 0x84};
    int  ptnno;
    unsigned char *p;
    int  row;

    if (water_leak_level != 0) {
        if (water_level == 0) {
            return;
        }
        if (water_count > water_leak_level)
        {
            water_count -= water_leak_level;
            return;
        }
        water_count += 50 - water_leak_level;

        water_level--;
    }

    row = water_level >> 2;
    ptnno = water_level & 0x03;

    p = getscrnaddr(row, 0);
    *p = ptn[ptnno];
}

#include <stdio.h>
#include <string.h>

#include "car.h"
#include "scrn.h"


// back buffer

unsigned char bakbuf[BAK_DY][BAK_DX];

unsigned int bakbuf_sx;
unsigned int bakbuf_sy;


// radar

unsigned char radarbuf[RADAR_DY][RADAR_DX];


// fuel

unsigned char fuel_ptnchr[4] = {
    0x00, 0x87, 0x85, 0x81
};


// car table

unsigned char mycartbl[CAR_NUM][CHR_DY][CHR_DX] = {
{{0x06,0x07,0x04},{0x81,0x84,0x04},{0x01,0x01,0x01}},
{{0x00,0x86,0x04},{0x84,0x84,0x86},{0x02,0x84,0x00}},
{{0x84,0x06,0x04},{0x86,0x07,0x05},{0x03,0x02,0x00}},
{{0x87,0x81,0x00},{0x81,0x81,0x06},{0x00,0x06,0x01}},
{{0x82,0x86,0x05},{0x06,0x07,0x04},{0x02,0x03,0x00}},
{{0x00,0x82,0x04},{0x86,0x82,0x82},{0x02,0x86,0x00}},
{{0x06,0x81,0x01},{0x07,0x82,0x01},{0x02,0x02,0x01}},
{{0x87,0x06,0x00},{0x06,0x07,0x07},{0x00,0x07,0x01}}
};

unsigned char encartbl[CAR_NUM][CHR_DY][CHR_DX] = {
{{0x06,0x07,0x04},{0x85,0x84,0x00},{0x01,0x01,0x01}},
{{0x00,0x86,0x04},{0x86,0x84,0x86},{0x02,0x86,0x00}},
{{0x86,0x06,0x04},{0x86,0x07,0x05},{0x01,0x02,0x00}},
{{0x87,0x06,0x00},{0x06,0x81,0x06},{0x00,0x06,0x01}},
{{0x86,0x86,0x01},{0x06,0x07,0x04},{0x02,0x03,0x00}},
{{0x00,0x86,0x04},{0x86,0x82,0x86},{0x02,0x86,0x00}},
{{0x06,0x83,0x01},{0x07,0x82,0x01},{0x02,0x00,0x01}},
{{0x87,0x06,0x00},{0x06,0x07,0x06},{0x00,0x06,0x01}}
};


// live
int live_num;


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


unsigned char* getscrnaddr(int posy, int posx)
{
    #asm
        ld      ix,2
        add     ix,sp

        push    de
        ld      d, (ix+2)       ; d=posy
        ld      e, (ix+0)       ; e=posx
        call    getscrnaddr
        pop     de
        ret

getscrnaddr:
; IN:  D=y, E=x
; OUT: HL=_getscrnaddr
        push    bc

        ld      b, 0
        ld      c, d            ; c=posy
        ld      h, 0
        ld      l, c
        add     hl, hl          ; *2
        add     hl, hl          ; *4
        add     hl, hl          ; *8
        add     hl, hl          ; *16
        add     hl, hl          ; *32
        add     hl, bc          ; *33

        ld      b, 0
        ld      c, e            ; c=posx
        add     hl, bc

        ld      bc,(D_FILE)
        inc     bc
        add     hl, bc

        pop     bc
        ret

    #endasm
}


void clr_main()
{
    #asm
        ld      c,24
        ld      d,0
        ld      e,0
clr_main_yloop:
        call    getscrnaddr

        ld      b,24
clr_main_xloop:
        ld      (hl),0
        inc     hl
        djnz    clr_main_xloop  

        inc     d
        dec     c
        jp      nz,clr_main_yloop
    #endasm
}


void displive()
{
    #asm
        ld      b,2
        ld      d,LIVE_SY

displive_line:
        push    bc
        ld      e,LIVE_SX
        call    getscrnaddr

        ld      a,(_live_num)
        ld      b,4

displive_loop:
        cp      a,0
        jp      z,displive_spc

displive_mycar:
        ld      (hl),0x81
        inc     hl
        ld      (hl),0x04
        inc     hl
        dec     a
        jp      displive_nxt

displive_spc:
        ld      (hl),0x00
        inc     hl
        ld      (hl),0x00
        inc     hl
displive_nxt:
        djnz    displive_loop

        pop     bc
        ld      d,LIVE_SY+1
        djnz    displive_line

        ret
    #endasm
}

void makebak(unsigned char *bakbufp, int fldy, int fldx,
             int bakbuf_dy, int bakbuf_dx, int bakubuf_nextline)
{
    #asm
        ld      ix,2
        add     ix,sp

        ld      d, (ix+8)       ; d=fldy

        ld      l, (ix+10)
        ld      h, (ix+11)      ; hl=bakbufp

        ld      c,(ix+4)        ; bakbuf dy
makebak_cpyy:
        ld      e, (ix+6)       ; e=fldx
        ld      b,(ix+2)        ; bakbuf dx

makebak_cpyx:

        call    makebak_getchrno
        push    hl
        push    de
        ex      de,hl
        call    makebak_getchradr
        call    makebak_cpychr
        ex      de,hl
        pop     de
        pop     hl

        push   de
        ld     de,CHR_DX
        add    hl,de
        pop    de
        inc    e
        djnz   makebak_cpyx

        push   de
        ld     e, (ix+0)
        ld     d, (ix+1)       ; bakubuf_nextline
        add    hl,de
        pop    de

	    inc    d
        dec    c
        jp     nz,makebak_cpyy

        ret

makebak_getchrno:

; in: fldtblp(ix+10, ix+11)
;     d=fldy, e=fldy
; out: a(fldtbl data)
        ld      a, e
        cp      FLD_DX
        jp      nc,makebak_getchrno_CHR_BLK
        ld      a, d
        cp      FLD_DY
        jp      nc,makebak_getchrno_CHR_BLK

        push    hl
        push    bc

        ld      hl,_fldtbl
        ld      c,e
        ld      b,0
        add     hl,bc

        ld      c,d
        and     0
        sla     c  ; x2
        adc     a,a
        sla     c  ; x4
        adc     a,a
        sla     c  ; x8
        adc     a,a
        sla     c  ; x16
        adc     a,a
        sla     c  ; x32
        adc     a,a
        ld      b,a
        add     hl,bc

        ld      a,(hl)
        pop     bc
        pop     hl
        ret

makebak_getchrno_CHR_BLK:
        ld      a,CHR_BLK
        ret

makebak_getchradr:
; in: chrtbl(ix+12, ix+13)
;     a(chrno)
; out: hl(chradr)
        push    de

        ld      hl,_chrtbl
        ld      e,a
        ld      d,0
        add     hl,de

        and     0
        sla     e  ; x2
        adc     a,a
        sla     e  ; x4
        adc     a,a
        sla     e  ; x8
        adc     a,a
        ld      d,a
        add     hl,de

        pop     de
        ret

makebak_cpychr:
; in: hl chradr
;     de scradr
        push    bc
        push    de

        ld      a,CHR_DY

makebak_cpychr_loop:
        ld      bc, CHR_DX
        ldir

        dec     a
        jp      z,makebak_cpychr_loopout

        ex      de,hl
        ld      bc,BAK_DY-CHR_DX
        add     hl,bc
        ex      de,hl

        jp      makebak_cpychr_loop

makebak_cpychr_loopout:
        pop     de
        pop     bc

    #endasm
}

static int dispbak(unsigned char *scrnlinep, unsigned char *bakbufp, int baksy, int baksx)
{
    #asm
        ld      ix,2
        add     ix,sp

;baksx add
        ld      e, (ix+0)       ; baksx
        ld      d, 0
        ld      l, (ix+4)
        ld      h, (ix+5)       ; hl=bakbufp
        add     hl,de

;baksy add
        ld      de,BAK_DX
        ld      a, (ix+2)       ; baksy
dispbbak_baksy:
        cp      0
        jp      z,dispbak_scrncpy
        add     hl,de
        dec     a
        jp      dispbbak_baksy

dispbak_scrncpy:
        ld      e, (ix+6)
        ld      d, (ix+7)       ; scrnlinep

        ld      a, VIEW_DY*CHR_DX+2

dispbak_scrncpy_x:
        cp     0
        jp      z,dispbak_scrncpy_fin

        ld      bc,VIEW_DX*CHR_DX+2
        ldir

        push    de
        ld      de,BAK_DX-(VIEW_DX*CHR_DX+2)
        add     hl,de
        pop     de

        push    hl
        ex      de,hl
        ld      de,33-(VIEW_DX*CHR_DX+2)
        add     hl,de
        ex      de,hl
        pop     hl

        dec     a
        jp      dispbak_scrncpy_x

dispbak_scrncpy_fin:
        ret

    #endasm
}


void initcourse()
{
    bakbuf_sy = carinf[0].fld_y-4;
    bakbuf_sx = carinf[0].fld_x-4;

    makebak(&bakbuf[0][0], bakbuf_sy, bakbuf_sx, 9, 9, BAK_DY*(CHR_DY-1));
    dispbak(getscrnaddr(0, 0), &bakbuf[0][0], 2, 2);
}

void dispcourse(int cary, int carx, int stepy, int stepx)
{
    #asm
        ld      ix,2
        add     ix,sp

;    dispcourse_newsy = cary - 4;
        ld      d,0
        ld      e,(ix+6)        ; cary
        dec     de
        dec     de
        dec     de
        dec     de

;    sy = bakbuf_sy - newsy;
;    bakbuf_sy = dispcourse_newsy;
        ld      hl,(_bakbuf_sy)
        ld      a,l
        sub     e
        ld      l,a
        ld      a,h
        sbc     d
        ld      h,a
        ld      (_dispcourse_sy),hl
        ld      (_bakbuf_sy),de

;    stepy = (stepy == 0) ? 2 : stepy - 1;
;    by = (stepy == 0) ? 0 : CHR_DY;
        ld      a,(ix+2)        ; stepy
        cp      0
        jp      nz,dispcourse_y1
        ld      a,2
        ld      (ix+2),a
        ld      a,0
        ld      (_dispcourse_by),a
        jp      dispcourse_y2
dispcourse_y1:
        dec     a
        ld      (ix+2),a
        ld      a,CHR_DY
        ld      (_dispcourse_by),a
dispcourse_y2:


;    dispcourse_newsx = carx - 4;
        ld      d,0
        ld      e,(ix+4)        ; carx
        dec     de
        dec     de
        dec     de
        dec     de

;    sx = bakbuf_sx - newsx;
;    bakbuf_sx = dispcourse_newsx;
        ld      hl,(_bakbuf_sx)
        ld      a,l
        sub     e
        ld      l,a
        ld      a,h
        sbc     d
        ld      h,a
        ld      (_dispcourse_sx),hl
        ld      (_bakbuf_sx),de

;    stepx = (stepx == 0) ? 2 : stepx - 1;
;    bx = (stepx == 0) ? 0 : CHR_DX;
        ld      a,(ix+0)        ; stepx
        cp      0
        jp      nz,dispcourse_x1
        ld      a,2
        ld      (ix+0),a
        ld      a,0
        ld      (_dispcourse_bx),a
        jp      dispcourse_x2
dispcourse_x1:
        dec     a
        ld      (ix+0),a
        ld      a,CHR_DX
        ld      (_dispcourse_bx),a
dispcourse_x2:

        ld      hl,(_dispcourse_sy)
        inc     hl                  ; syの-1,0,1チェックを0,1,2チェックに変更

        ld      de,(_dispcourse_sx)
        inc     de                  ; sxの-1,0,1チェックを0,1,2チェックに変更

        ld      a,h
        cp      0                   ; syが00nn以外は全書き換え 
        jp      nz,dispcource_redraw
        ld      a,d
        cp      0                   ; sxが00nn以外は全書き換え 
        jp      nz,dispcource_redraw

        ld      a,e
        cp      1
        jp      nz,dispcource_left_or_right ;sxが0以外

dispcource_up_or_down:
        ld      a,l
        cp      0
        jp      z,dispcource_up_move        ;sx=0 & sy=-1
        cp      2
        jp      z,dispcource_down_move      ;sx=0 & sy=1
        cp      1
        jp      z,dispcource_bak2scrn       ;sx=0 & sy=0
        jp      dispcource_redraw           ;sx=0 & syが上記以外

dispcource_left_or_right:
        ld      a,l
        cp      1                   
        jp      nz,dispcource_redraw        ;sxが0以外 & syが0以外なら全書き換え

        ld      a,e
        cp      0
        jp      z,dispcource_left_move      ;sx=-1 & sy=0
        cp      2
        jp      z,dispcource_right_move     ;sx=1 & sy=0
        cp      1
        jp      z,dispcource_bak2scrn       ;sx=0 & sy=0
        jp      dispcource_redraw           ;sx=0 & syが上記以外

._dispcourse_sx     defw    0
._dispcourse_sy     defw    0
._dispcourse_bx     defb    0
._dispcourse_by     defb    0

dispcource_up_move:
;        memcpy(&bakbuf[0][0], &bakbuf[CHR_DY][0], BAK_DX * (8 * CHR_DY));
        ld      hl,_bakbuf
        ld      de,CHR_DY*BAK_DX
        add     hl,de
        ld      de,_bakbuf
        ld      bc,BAK_DX * (8 * CHR_DY)
        ldir

;        makebak(&bakbuf[8*CHR_DY][0], bakbuf_sy+8, bakbuf_sx, 1, 9, BAK_DY*(CHR_DY-1));
        push    ix

        ld      hl,_bakbuf
        ld      de,8*CHR_DY*BAK_DX
        add     hl,de
        push    hl

        ld      hl,(_bakbuf_sy)
        ld      de,8
        add     hl,de
        push    hl

        ld      hl,(_bakbuf_sx)
        push    hl

        ld      hl,1
        push    hl

        ld      hl,9
        push    hl

        ld      hl,BAK_DY*(CHR_DY-1)
        push    hl

        call    _makebak

        pop     hl
        pop     hl
        pop     hl
        pop     hl
        pop     hl
        pop     hl

        pop     ix
        jp      dispcource_bak2scrn

dispcource_down_move:
;        memmove(&bakbuf[CHR_DY][0], &bakbuf[0][0], BAK_DX * (8 * CHR_DY));
        ld      hl,_bakbuf
        ld      de,CHR_DY*BAK_DX
        add     hl,de
        ld      bc,BAK_DX * (8 * CHR_DY)
        add     hl,bc
        dec     hl
        ld      de,_bakbuf
        ex      de,hl
        add     hl,bc
        dec     hl
        lddr

;        makebak(&bakbuf[0][0], bakbuf_sy, bakbuf_sx, 1, 9, BAK_DY*(CHR_DY-1));
        push    ix

        ld      hl,_bakbuf
        push    hl

        ld      hl,(_bakbuf_sy)
        push    hl

        ld      hl,(_bakbuf_sx)
        push    hl

        ld      hl,1
        push    hl

        ld      hl,9
        push    hl

        ld      hl,BAK_DY*(CHR_DY-1)
        push    hl

        call    _makebak

        pop     hl
        pop     hl
        pop     hl
        pop     hl
        pop     hl
        pop     hl

        pop     ix
        jp      dispcource_bak2scrn

dispcource_left_move:
;        memmove(&bakbuf[0][0], &bakbuf[0][CHR_DX], BAK_DX * BAK_DY - CHR_DX);
        ld      hl,_bakbuf
        ld      de,CHR_DX
        add     hl,de
        ld      de,_bakbuf
        ld      bc,BAK_DX * BAK_DY - CHR_DX
        ldir

;        makebak(&bakbuf[0][8*CHR_DX], bakbuf_sy, bakbuf_sx+8, 9, 1, BAK_DY*CHR_DY-CHR_DX);
        push    ix

        ld      hl,_bakbuf
        ld      de,8*CHR_DX
        add     hl,de
        push    hl

        ld      hl,(_bakbuf_sy)
        push    hl

        ld      hl,(_bakbuf_sx)
        ld      de,8
        add     hl,de
        push    hl

        ld      hl,9
        push    hl

        ld      hl,1
        push    hl

        ld      hl,BAK_DY*CHR_DY-CHR_DX
        push    hl

        call    _makebak

        pop     hl
        pop     hl
        pop     hl
        pop     hl
        pop     hl
        pop     hl

        pop     ix
        jp      dispcource_bak2scrn

dispcource_right_move:
;        memmove(&bakbuf[0][CHR_DX], &bakbuf[0][0], BAK_DX * BAK_DY - CHR_DX);
        ld      hl,_bakbuf
        ld      de,CHR_DX
        add     hl,de
        ld      bc,BAK_DX * BAK_DY - CHR_DX
        add     hl,bc
        dec     hl
        ld      de,_bakbuf
        ex      de,hl
        add     hl,bc
        dec     hl
        lddr

;        makebak(&bakbuf[0][0], bakbuf_sy, bakbuf_sx, 9, 1, BAK_DY*CHR_DY-CHR_DX);
        push    ix

        ld      hl,_bakbuf
        push    hl

        ld      hl,(_bakbuf_sy)
        push    hl

        ld      hl,(_bakbuf_sx)
        push    hl

        ld      hl,9
        push    hl

        ld      hl,1
        push    hl

        ld      hl,BAK_DY*CHR_DY-CHR_DX
        push    hl

        call    _makebak

        pop     hl
        pop     hl
        pop     hl
        pop     hl
        pop     hl
        pop     hl

        pop     ix

        jp      dispcource_bak2scrn


dispcource_redraw:
;       全書き換え処理
;        makebak(&bakbuf[0][0], bakbuf_sy, bakbuf_sx, 9, 9, BAK_DY*(CHR_DY-1));
        push    ix

        ld      hl,_bakbuf
        push    hl

        ld      hl,(_bakbuf_sy)
        push    hl

        ld      hl,(_bakbuf_sx)
        push    hl

        ld      h,0
        ld      l,9
        push    hl
        push    hl

        ld      hl,BAK_DY*(CHR_DY-1)
        push    hl

        call    _makebak

        pop     hl
        pop     hl
        pop     hl
        pop     hl
        pop     hl
        pop     hl

        pop     ix

dispcource_bak2scrn:
;    dispbak(getscrnaddr(0, 0), &bakbuf[dispcourse_by][dispcourse_bx], stepy, stepx);
        push    ix

        ld      hl,(D_FILE)
        inc     hl
        push    hl

        ld      hl,_bakbuf
        ld      d,0
        ld      a,(_dispcourse_bx)
        ld      e,a
        add     hl,de
        ld      a,(_dispcourse_by)
        cp      0
        jp      z,dispcource_bak2scrn_1
        ld      de,BAK_DX*CHR_DY
        add     hl,de
dispcource_bak2scrn_1:
        push    hl

        ld      l,(ix+2)    ;stepy
        push    hl

        ld      h,0
        ld      l,(ix+0)    ;stepx
        push    hl

        call    _dispbak

        pop     hl
        pop     hl
        pop     hl
        pop     hl

        pop     ix

    #endasm
}

void initradar()
{
    int  y;

    memset(&radarbuf[0][0], 0x80, sizeof(radarbuf));

    for (y = 0; y < RADAR_DY; y++) {
        memcpy(getscrnaddr(RADAR_SY+y, RADAR_SX), &radarbuf[y][0], RADAR_DX);
    }
}

unsigned char getdotval(unsigned char dotval, int y, int x, int onoff)
{
    #asm

        ld      ix,2
        add     ix,sp

        ld      a, (ix+6)       ; dotval
        ld      d, (ix+4)       ; y
        ld      e, (ix+2)       ; x
        ld      h, (ix+0)       ; onoff

;0x80以上は0x8fとxorする
        cp      0x80
        jp      m, getdotval_xor1
        xor     0x8f
getdotval_xor1:
        ld      l,a

        ld      a,0x01
        rrc     d
        jp      nc,getdotval_y
        ld      a,0x04
getdotval_y:
        rrc     e
        jp      nc,getdotval_x
        rlca
getdotval_x:

        rrc     h
        jp      c,getdotval_on
getdotval_off:
        or      l
        jp      getdotval_xor2
getdotval_on:
        xor     0xff
        and     a,l


;0x08以上は0x8fとxorする
getdotval_xor2:
        cp      0x08
        jp      m, getdotval_xor3
        xor      0x8f
getdotval_xor3:

        ld      h,0
        ld      l,a

    #endasm
}

void setradar(int y, int x, int onoff)
{
    int  rx, ry;

    // potision check
    ry = y >> 2;
    rx = x >> 2;
    if ((ry < 0) || (ry >= RADAR_DY) ||
        (rx < 0) || (rx >= RADAR_DX)) {
        return;
    }

    // on/off set
    radarbuf[ry][rx] = getdotval(radarbuf[ry][rx], y>>1, x>>1, onoff);

    // refresh
    *(getscrnaddr(RADAR_SY+ry, RADAR_SX+rx)) = radarbuf[ry][rx];
}

void dispradar(int onoff)
{
    #asm
        ld      ix,2
        add     ix,sp

        ld      a, (ix+0)       ; onoff
        ld      (_dispradar_onoff),a
        cp      ALLON
        jp      z,dipradar_normal

        ld      a, (_challenging_stage)
        cp      OFF
        jp      z,dipradar_normal

dipradar_challenging_stage:
        ld      b,1
        jp      dispradar_onoff

dipradar_normal:
        ld      a,(_car_num)
        ld      b,a

        ld      a, (_dispradar_onoff)
        cp      OFF
        jp      nz, dispradar_onoff

dispradar_off:
        ld      de,0
        call    dispradar_getscrnaddr
        ld      de,_radarbuf
        ex      de,hl

        ld      b,RADAR_DY
        ld      c,0xff
dispradar_off_1:
;        ld      bc,RADAR_DX
;        ldir
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi

        ld      a,e
        add     a, -RADAR_DX + D_DX  ; なぜか　D_DX - RADAR_DXはエラーとなる
        jp      nc,dispradar_off_2
        inc     d
dispradar_off_2:
        ld      e,a
        djnz    dispradar_off_1

        ret

dispradar_onoff:
        ld      ix,_carinf

dispradar_loop:
        ld      a,(ix+8)    ; _carinf->mode
        cp      CARM_NONE
        jp      z,dispradar_loop_next

; D = _carinf->fld_y >> 2
; E = _carinf->fld_x >> 2
        ld      d,(ix+0)    ; _carinf->fld_y
        srl     d           ; >>1
        srl     d           ; >>2
        ld      e,(ix+1)    ; _carinf->fld_x
        srl     e           ; >>1
        srl     e           ; >>2

; if ((D < 0) || (D >= RADAR_DY) ||
;     (E < 0) || (E >= RADAR_DX))
;     dispradar_loop_next
        ld      a, d
        cp      RADAR_DY
        jp      nc,dispradar_loop_next
        ld      a, e
        cp      RADAR_DX
        jp      nc,dispradar_loop_next

        call    dispradar_getscrnaddr
        ld      a,(hl)
        call     dispradar_getdotval
        ld      (hl),a

dispradar_loop_next:
        ld      de,CARINF_SIZE  ; add ix,sizeof(struct CARINF)
        add     ix,de
        djnz    dispradar_loop

        ret

dispradar_getscrnaddr:

        ld      a,RADAR_SY
        add     a,d
        ld      d,a

        ld      a,RADAR_SX
        add     a,e
        ld      e,a

        jp      getscrnaddr

dispradar_getdotval:
; IN:  A=dotval, ix+0=y, ix+1=x
; OUT: A=new_dotval
        push    ix
        push    bc
        push    de
        push    hl

        ld      b,0

        ld      c,a         ; dotval
        push    bc

        ld      c,(ix+0)    ; _carinf->y
        srl     c           ; >>1
        push    bc

        ld      c,(ix+1)    ; _carinf->x
        srl     c           ; >>1
        push    bc

        ld      a, (_dispradar_onoff)
        ld      c,a
        push    bc

        call    _getdotval
        ld      a,l

        pop     bc
        pop     bc
        pop     bc
        pop     bc

        pop     hl
        pop     de
        pop     bc
        pop     ix

        ret

._dispradar_onoff   defb    0

    #endasm
}

// n=0<->32
void dispfuel(int num)
{
    #asm
        ld      ix,2
        add     ix,sp

        ld      a,(ix+0)    ; num
        cp      32+1
        ret     nc

        ld      a,(ix+0)    ; num
        and     a,0x03
        ld      hl,_fuel_ptnchr
        ld      d,0
        ld      e,a
        add     hl,de
        ld      c,(hl)

        ld      d,FUEL_SY
        ld      e,FUEL_SX
        call    getscrnaddr

        ld      a,(ix+0)    ; num
        sra     a
        sra     a
        ld      d,a
        ld      e,a
        inc     e

        ld      b,8
dispfuel_loop:
        ld      a,d
        cp      b
        jp      nc,dispfuel_fillchr

        ld      a,e
        cp      b
        jp      nz,dispfuel_spcchr

dispfuel_ptnchr:
        ld      (hl),c
        jp      dispfuel_next

dispfuel_spcchr:
        ld      (hl),0x00
        jp      dispfuel_next

dispfuel_fillchr:
        ld      (hl),0x80

dispfuel_next:
        inc     hl
        djnz    dispfuel_loop

        ret

    #endasm
}

void dispchr(unsigned char *chrbufp, int posy, int posx)
{
        getscrnaddr(posy, posx);

    #asm
        ld      ix,2
        add     ix,sp

        ld      e, (ix+4)
        ld      d, (ix+5)       ; chrbufp

        ld      a,CHR_DY
        ex      de,hl


copychr_loop:
        ld      bc, CHR_DX
        ldir

        dec     a
        ret     z

        ex      de,hl

        ld      bc,33-CHR_DX
        add     hl,bc

        ex      de,hl

        jp      copychr_loop

    #endasm
}

void copyascii2zx81chr(unsigned char *dstp, char *asciip)
{
    #asm
        ld      ix,2
        add     ix,sp

        ld      c, (ix+0)
        ld      b, (ix+1)       ; asciip
        ld      e, (ix+2)
        ld      d, (ix+3)       ; dstp

copyascii2zx81chr_loop:
        ld      a,(bc)
        cp      0
        ret     z

        sub     ASCII_BGN
        jp      c,copyascii2zx81chr_2
        cp      ZX81CHR_SIZE    ;sizeof(zx81chr)
        jp      nc,copyascii2zx81chr_2

        ld      hl,_zx81chr
        add     a,l
        jp      nc,copyascii2zx81chr_1
        inc     h
copyascii2zx81chr_1:
        ld      l,a
        ld      a,(hl)
        jp      copyascii2zx81chr_3

copyascii2zx81chr_2:
        ld      a,0
copyascii2zx81chr_3:
        ld      (de),a

        inc     bc
        inc     de

        jp      copyascii2zx81chr_loop

    #endasm
}

void copynum2zx81chr(unsigned char *dstp, int num, int ndigit)
{
    unsigned char ascii[16+1];
    int  i, n;

    memset(ascii, 0, sizeof(ascii));

    if (ndigit > 16) {
        return;
    }

    i = ndigit-1;
    while (i >= 0) {
        n = num % 10;
        num /= 10;
        if (num != 0) {
            ascii[i] = '0' + n;
        }
        else {
            ascii[i] = ((n != 0) || (i == ndigit-1)) ? '0' + n : ' ';
        }
        i--;
    }
    copyascii2zx81chr(dstp, ascii);
}

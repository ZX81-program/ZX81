#include <stdio.h>
#include <string.h>

#include "map.h"
#include "scrn.h"
#include "car.h"


// car information

struct CARINF carinf[CARINF_MAX];
unsigned char car_num;
unsigned char challenging_stage;


// direction

#define DIR_UPNO    0
#define DIR_RTNO    1
#define DIR_DNNO    2
#define DIR_LTNO    3

struct DIRTBL {
    unsigned char dir;
    unsigned char dirno;
} dirtbl[4][4] = {
    {{DIR_UP, DIR_UPNO}, {DIR_RT, DIR_RTNO}, {DIR_LT, DIR_LTNO}, {DIR_DN, DIR_DNNO}},
    {{DIR_RT, DIR_RTNO}, {DIR_DN, DIR_DNNO}, {DIR_UP, DIR_UPNO}, {DIR_LT, DIR_LTNO}},
    {{DIR_DN, DIR_DNNO}, {DIR_LT, DIR_LTNO}, {DIR_RT, DIR_RTNO}, {DIR_UP, DIR_UPNO}},
    {{DIR_LT, DIR_LTNO}, {DIR_UP, DIR_UPNO}, {DIR_DN, DIR_DNNO}, {DIR_RT, DIR_RTNO}}
};
#define DIRTBL_SIZE 8   //asm用


// smoke

unsigned char smokecnt;
struct SMOKEINF smokeinf[SMOKENUM];


void initcar()
{
    static char start_fldpos[CARINF_MAX][2] =
    {
        {15, 49},
        {15, 51}, {13, 51}, {17, 51}, {11, 51}, {19, 51},
        {15,  1}, {17,  1}
    };
    int n;
    struct CARINF *carinfp;

    memset(carinf, 0, sizeof(carinf));

    for (n = 0; n < CARINF_MAX; n++) {
        carinfp = &carinf[n];

        carinfp->fld_y = start_fldpos[n][1];
        carinfp->fld_x = start_fldpos[n][0];
//        carinfp->step_y = 0;
//        carinfp->step_x = 0;
        carinfp->y = carinfp->fld_y * CHR_DY;
        carinfp->x = carinfp->fld_x * CHR_DX;
        carinfp->dirno = (carinfp->fld_y > 10) ? DIR_UPNO : DIR_DNNO;
        carinfp->chrno = (carinfp->fld_y > 10) ? CAR_UP   : CAR_DN;
        carinfp->mode = (n == 0) ? CARM_MOVE : CARM_WAIT;
//        carinfp->free = 0;
    }
}

void dispcar()
{
    #asm

        ld      ix,_carinf
        ld      a,(_car_num)
        ld      b,a

dispcar_loop:
;        if (carinfp->mode == CARM_NONE) {
;            continue;
;        }
        ld      a,(ix+8)    ; _carinf->mode
        cp      CARM_NONE
        jp      z,dispcar_loop_next

;        if (carinfp->mode > CARM_MOVE) {
;            carinfp->chrno = (carinfp->chrno >= CAR_NUM-1) ? 0 : carinfp->chrno+1;
;        }
;        else {
;            chrno = carinfp->dirno << 1;
;            if (carinfp->chrno != chrno) {
;                // turn
;                i = carinfp->chrno - chrno;
;                if (i >= 4) {
;                    carinfp->chrno = (carinfp->chrno >= CAR_NUM-1) ? 0 : carinfp->chrno+1;
;                }
;                else if (i > 0) {
;                    carinfp->chrno--;
;                }
;                else if (i >= -4) {
;                    carinfp->chrno++;
;                }
;                else {
;                    carinfp->chrno = (carinfp->chrno <= 0) ? 7: carinfp->chrno-1;
;                }
;            }
;        }

        cp      CARM_MOVE+1
        jp      c,dispcar_1
        call    dispcar_turn1
        jp      dispcar_2
dispcar_1:
        jp      nc,dispcar_2
        call    dispcar_turn2
dispcar_2:


;        // my car
;        if (carno == 0) {
;            dispchr(&mycartbl[carinfp->chrno][0][0], 3*CHR_DY+1, 3*CHR_DX+1);
;            continue;
;        }
        ld      a,(_car_num)
        cp      b
        jp      nz,dispcar_encarproc

dispcar_mycarproc:
        call    dispcar_mycar

        ld      a,(ix+4)    ; _carinf->y
        ld      (_dispcar_mycar_y),a
        ld      a,(ix+5)    ; _carinf->x
        ld      (_dispcar_mycar_x),a

        jp      dispcar_loop_next

dispcar_encarproc:
        ld      a,0
        ld      (_dispcar_encar_dispy),a
        ld      (_dispcar_encar_dispx),a
        ld      (_dispcar_encar_offy),a
        ld      (_dispcar_encar_offx),a
        ld      (_dispcar_encar_cpyy),a
        ld      (_dispcar_encar_cpyx),a

        call    dispcar_encar_calc_y
        ld      a,(_dispcar_encar_cpyy)
        cp      0
        jp      z,dispcar_loop_next

        call    dispcar_encar_calc_x

        ld      a,(_dispcar_encar_cpyx)
        cp      0
        jp      z,dispcar_loop_next

        call    dispcar_encar_dispchr

dispcar_loop_next:
        ld      de,CARINF_SIZE  ; add ix,sizeof(struct CARINF)
        add     ix,de
        djnz    dispcar_loop

        ret

._dispcar_mycar_y      defb    0
._dispcar_mycar_x      defb    0
._dispcar_encar_dispy  defb    0
._dispcar_encar_dispx  defb    0
._dispcar_encar_offy   defb    0
._dispcar_encar_offx   defb    0
._dispcar_encar_cpyy   defb    0
._dispcar_encar_cpyx   defb    0

;            carinfp->chrno = (carinfp->chrno >= CAR_NUM-1) ? 0 : carinfp->chrno+1;
dispcar_turn1:
        ld      a,(ix+7)    ; _carinf->chrno
        inc     a
        cp      CAR_NUM
        jp      c,dispcar_turn1_1
        ld      a,0
dispcar_turn1_1:
        ld      (ix+7),a
        ret

;            chrno = carinfp->dirno << 1;
;            if (carinfp->chrno != chrno) {
;                // turn
;                i = carinfp->chrno - chrno;
;                if (i >= 4) {
;                    carinfp->chrno = (carinfp->chrno >= CAR_NUM-1) ? 0 : carinfp->chrno+1;
;                }
;                else if (i > 0) {
;                    carinfp->chrno--;
;                }
;                else if (i >= -4) {
;                    carinfp->chrno++;
;                }
;                else {
;                    carinfp->chrno = (carinfp->chrno <= 0) ? 7: carinfp->chrno-1;
;                }
;            }
dispcar_turn2:
        push    hl

        ld      h,(ix+6)    ; _carinf->dirno << 1
        sla     h
        ld      l,(ix+7)    ; _carinf->chrno

; l = h
        ld      a,l
        cp      h
        jp      z,dispcar_turn2_ret

        jp      c,dispcar_turn2_10
; l > h の場合
        sub     h
        cp      4
        jp      c,dispcar_turn2_1
; l - h >= 4 ならば l++
        inc     l
        ld      a,l
        cp      CAR_NUM
        jp      c,dispcar_turn2_set
; l >= CAR_NUM ならば l = 0
        ld      l,0
        jp      dispcar_turn2_set
dispcar_turn2_1:
; l - h < 4 ならば l--
        dec     l
        jp      dispcar_turn2_set

dispcar_turn2_10:
; l < h の場合
        ld      a,h
        sub     l
        cp      5
        jp      c,dispcar_turn2_12
; h - l >= 4 ならば
        ld      a,l
        cp      0
        jp      z,dispcar_turn2_11
; l が 0以外なら l--
        dec     l
        jp      dispcar_turn2_set
dispcar_turn2_11:
; l が0なら l = 7
        ld      l,7
        jp      dispcar_turn2_set
dispcar_turn2_12:
; h - l < 4 ならば l++
        inc     l
        jp      dispcar_turn2_set

dispcar_turn2_set:
        ld      (ix+7),l    ; _carinf->chrno

dispcar_turn2_ret:
        pop     hl
        ret

;            dispchr(&mycartbl[carinfp->chrno][0][0], 3*CHR_DY+1, 3*CHR_DX+1);
dispcar_mycar:
        push    ix
        push    bc

        ld      hl,_mycartbl
        ld      a,(ix+7)    ; _carinf->chrno
        sla     a           ; <<1 (*2)
        sla     a           ; <<2 (*4)
        sla     a           ; <<3 (*8)
        add     a,(ix+7)    ; _carinf->chrno * 9
        add     a,l
        ld      l,a
        jp      nc,dispcar_mycar_1
        inc     h
dispcar_mycar_1:
        push    hl

        ld      hl,3*CHR_DY+1
        push    hl

        ld      hl,3*CHR_DX+1
        push    hl

        call    _dispchr

        pop     hl
        pop     hl
        pop     hl

        pop     bc
        pop     ix

        ret

dispcar_encar_calc_y:
        ld      a,(_dispcar_mycar_y)
        ld      l,a
        ld      e,(ix+4)
        ld      a,e
        sub     l
        jp      nc,dispcar_encar_calc_y2

; _encar_sy < _mycar_sy
        ld      a,l
        sub     e
        cp      12+1
; a > 12 ret
        ret     nc

        cp      10
        jp      nc,dispcar_encar_calc_y1
; a 10<->1= dspy:0-9,offy:0,cpyy:3
        ld      e,a
        ld      a,10
        sub     a,e
        ld      (_dispcar_encar_dispy),a
        ld      a,3
        ld      (_dispcar_encar_cpyy),a
        ret

dispcar_encar_calc_y1:
; y方向に上にはみ出した場合の処理
; 12    = dspy:0,  offy:2,cpyy:1
; 11    = dspy:0,  offy:1,cpyy:2
        sub     10
        ld      (_dispcar_encar_offy),a
        ld      e,a
        ld      a,3
        sub     a,e
        ld      (_dispcar_encar_cpyy),a
        ld      a,0
        ld      (_dispcar_encar_dispy),a
        ret

dispcar_encar_calc_y2:
; _encar_sy <= _myxar_sy
        cp      12+1
; a > 12 ret
        ret     nc

        cp      10
        jp      nc,dispcar_encar_calc_y3
; a 0<->10= dspy:10-20,offy:0,cpyy:3
        add     a,10
        ld      (_dispcar_encar_dispy),a
        ld      a,3
        ld      (_dispcar_encar_cpyy),a
        ret

dispcar_encar_calc_y3:
; y方向に下にはみ出した場合の処理
; 11    = dspy:21, offy:0,cpyy:2
; 12    = dspy:22, offy:0,cpyy:1
        add     10
        ld      (_dispcar_encar_dispy),a
        sub     20
        ld      e,a
        ld      a,3
        sub     a,e
        ld      (_dispcar_encar_cpyy),a
        ret

dispcar_encar_calc_x:
;        ld      h,(_dispcar_mycar_x)
        ld      a,(_dispcar_mycar_x)
        ld      h,a
        ld      d,(ix+5)

        ld      a,d
        sub     h
        jp      nc,dispcar_encar_calc_x2

; _encar_sx < _myxar_sx
        ld      a,h
        sub     d
        cp      12+1
; a > 12 ret
        ret     nc

        cp      10
        jp      nc,dispcar_encar_calc_x1
; a 10<->1= dspx:0-9,offx:0,cpyx:3
        ld      d,a
        ld      a,10
        sub     a,d
        ld      (_dispcar_encar_dispx),a
        ld      a,3
        ld      (_dispcar_encar_cpyx),a
        ret

dispcar_encar_calc_x1:
; x方向に上にはみ出した場合の処理
; 12    = dspx:0,  offx:2,cpyx:1
; 11    = dspx:0,  offx:1,cpyx:2
        sub     10
        ld      (_dispcar_encar_offx),a
        ld      d,a
        ld      a,3
        sub     a,d
        ld      (_dispcar_encar_cpyx),a
        ld      a,0
        ld      (_dispcar_encar_dispx),a
        ret

dispcar_encar_calc_x2:
; _encar_sx <= _myxar_sx
        cp      12+1
; a > 12 ret
        ret     nc

        cp      10
        jp      nc,dispcar_encar_calc_x3
; a 0<->10= dspx:10-20,offx:0,cpyx:3
        add     a,10
        ld      (_dispcar_encar_dispx),a
        ld      a,3
        ld      (_dispcar_encar_cpyx),a
        ret

dispcar_encar_calc_x3:
; x方向に下にはみ出した場合の処理
; 11    = dspx:21, offx:0,cpyx:2
; 12    = dspx:22, offx:0,cpyx:1
        add     10
        ld      (_dispcar_encar_dispx),a
        sub     20
        ld      e,a
        ld      a,3
        sub     a,e
        ld      (_dispcar_encar_cpyx),a

        ret

dispcar_encar_dispchr:
        ld      hl,(_dispcar_encar_cpyy)
        ld      a,h
        add     a,l
        cp      6
        jp      nz,dispcar_encar_partscpy

;            dispchr(&encartbl[carinfp->chrno][0][0], _dispcar_encar_dispy, _dispcar_encar_dispx);
        push    ix
        push    bc

        ld      hl,_encartbl
        ld      a,(ix+7)    ; _carinf->chrno
        sla     a           ; <<1 (*2)
        sla     a           ; <<2 (*4)
        sla     a           ; <<3 (*8)
        add     a,(ix+7)    ; _carinf->chrno * 9
        add     a,l
        ld      l,a
        jp      nc,dispcar_encar_dispchr_1
        inc     h
dispcar_encar_dispchr_1:
        push    hl

        ld      hl,0

        ld      a,(_dispcar_encar_dispy)
        ld      l,a
        push    hl

        ld      a,(_dispcar_encar_dispx)
        ld      l,a
        push    hl

        call    _dispchr

        pop     hl
        pop     hl
        pop     hl

        pop     bc
        pop     ix

        ret

dispcar_encar_partscpy:
        ld      de,_encartbl
        ld      a,(ix+7)    ; _carinf->chrno
        sla     a           ; <<1 (*2)
        sla     a           ; <<2 (*4)
        sla     a           ; <<3 (*8)
        add     a,(ix+7)    ; _carinf->chrno * 9
        add     a,e
        ld      e,a
        jp      nc,dispcar_encar_partscpy_1
        inc     d
dispcar_encar_partscpy_1:
        ld      hl,(_dispcar_encar_offy)
        ld      a,l
        sla     a           ; <<1 (*2)
        add     a,l         ; _dispcar_encar_offy*3
        add     a,h         ; _dispcar_encar_offy*3 +_dispcar_encar_offx
        add     a,e
        ld      e,a
        jp      nc,dispcar_encar_partscpy_2
        inc     l
dispcar_encar_partscpy_2:

        push    ix
        push    bc

        ld      b,0
        ld      a,(_dispcar_encar_dispy)
        ld      c,a
        push    bc

        ld      a,(_dispcar_encar_dispx)
        ld      c,a
        push    bc

        call    _getscrnaddr
        pop     bc
        pop     bc

        ld      a,(_dispcar_encar_cpyy)
        ld      c,a

dispcar_encar_partscpy_3:
        ld      a,(_dispcar_encar_cpyx)
        ld      b,a
        push    de
        push    hl

dispcar_encar_partscpy_4:
        ld      a,(de)
        ld      (hl),a
        inc     hl
        inc     de
        djnz    dispcar_encar_partscpy_4

        pop     hl
        pop     de

        ld      a,c
        dec     a
        ld      c,a
        jp      z,dispcar_encar_partscpy_7

        ld      a,e
        add     a,CHR_DX
        ld      e,a
        jp      nc,dispcar_encar_partscpy_5
        inc     d
dispcar_encar_partscpy_5:

        ld      a,l
        add     a,33
        ld      l,a
        jp      nc,dispcar_encar_partscpy_6
        inc     h
dispcar_encar_partscpy_6:

        jp      dispcar_encar_partscpy_3

dispcar_encar_partscpy_7:
        pop     bc
        pop     ix

        ret

    #endasm
}

unsigned char keycheck()
{
    #asm
        ld  e,DIR_NONE

keycheck_read1:
        ld a,0xf7
        in a,(0xfe)
        ld h,a
        ld a,0xef
        in a,(0xfe)
        ld l,a

keycheck_up:
        ld a,l
        and 0x08    ; 7↑
        jp  nz,keycheck_down
        ld  a,e
        or  a,KBIT_UP
        ld  e,a

keycheck_down:
        ld a,l
        and 0x10    ; 6↓
        jp  nz,keycheck_left
        ld  a,e
        or  a,KBIT_DN
        ld  e,a

keycheck_left:
        ld a,h
        and 0x10    ; 5←
        jp  nz,keycheck_right
        ld  a,e
        or  a,KBIT_LT
        ld  e,a

keycheck_right:
        ld a,l
        and 0x04    ; 8→
        jp  nz,keycheck_read2
        ld  a,e
        or  a,KBIT_RT
        ld  e,a

keycheck_read2:
        ld a,0xbf
        in a,(0xfe)
        ld h,a
        ld a,0x7f
        in a,(0xfe)
        ld l,a

keycheck_enter:
        ld a,h
        and 0x01    ; ENTER
        jp  nz,keycheck_spc
        ld  a,e
        or  a,KBIT_ENT
        ld  e,a

keycheck_spc:
        ld a,l
        and 0x01    ; SPC
        jp  nz,keycheck_ret
        ld  a,e
        or  a,KBIT_SPC
        ld  e,a

keycheck_ret:
        ld  h,0
        ld  l,e
        ret

    #endasm
}

void movecar(int mycar_stp, int encar_stp)
{
    #asm
        ld      ix,2
        add     ix,sp

        ld      a, (ix+0)       ; encar_stp
        ld      (_movecar_encar_stp),a
        ld      a, (ix+2)       ; mycar_stp
        ld      (_movecar_mycar_stp),a

//    for (carno = 0; carno < car_num; carno++) {
//        carinfp = &carinf[carno];
        ld      ix,_carinf
        ld      a,(ix+4)            // maycar情報を変数へ代入
        ld      (_movecar_mycar_y),a
        ld      a,(ix+5)
        ld      (_movecar_mycar_x),a
        ld      a,(ix+6)
        ld      (_movecar_mycar_dirno),a

        ld      a,0
        ld      (_movecar_carno),a

        ld      a,(_car_num)
        ld      b,a
//        stp = (carno == 0) ? mycar_stp : encar_stp;
        ld      a,(_movecar_mycar_stp)
        ld      c,a

movecar_carloop:
//        if (carinfp->mode <= CARM_STOP) {
//            continue;
//        }
        ld      a,(ix+8)    ; _carinf->mode
        cp      CARM_WAIT
        jp      c,movecar_carloop_next

//        if (carinfp->mode > CARM_MOVE) {
//            carinfp->mode--;
//        }
//        else if (carinfp->mode < CARM_MOVE) {
//            carinfp->mode++;  
//        }
        cp      CARM_MOVE+1
        jp      c,movecar_carloop1
        dec     a
movecar_carloop1:
        cp      CARM_MOVE
        jp      nc,movecar_carloop2
        inc     a
movecar_carloop2:
        ld      (ix+8),a

//        if (carinfp->mode != CARM_MOVE) {
//            continue;
//        }
        cp      CARM_MOVE
        jp      nz,movecar_carloop_next

//        for (i = 0; i < stp; i++) {
movecar_stploop:
        ld      a,c
        cp      0
        jp      z,movecar_carloop_next

        push    bc
        call    movecar_init

        ld      a,(_movecar_carno)
        cp      0
        jp      nz,movecar_encar
movecar_mycar:
        call    movecar_mycar_routine
        call    movecar_freerun
        jp      movecar_move

movecar_encar:
//                if (carinfp->chrno != (carinfp->dirno << 1)) {
//                    continue;
//                }
        ld      a,(_challenging_stage)
        cp      0
        jp      nz,movecar_stploop_next

        ld      d,(ix+6)    ; _carinf->dirno << 1
        sla     d
        ld      a,(ix+7)    ; _carinf->chrno
        cp      d
        jp      nz,movecar_stploop_next

;        ld      a,(ix+2)        ; carinfp->step_y
;        cp      0
;        jp      nz,movecar_move
;        ld      a,(ix+3)        ; carinfp->step_x
;        cp      0
;        jp      nz,movecar_move

        call    movecar_encar_routine
        call    movecar_freerun
//            if (carinfp->dirno != newdirno) {
//                continue;
//            }
        ld      d,(ix+6)
        ld      a,(_movecar_newdirno)
        ld      (ix+6),a
        cp      d
        jp      nz,movecar_stploop_next

movecar_move:
        call    movecar_move_routine

movecar_stploop_next:
        pop     bc
        dec     c
        jp      movecar_stploop

movecar_carloop_next:
        ld      a,(_movecar_encar_stp)
        ld      c,a

        ld      a,(_movecar_carno)
        inc     a
        ld      (_movecar_carno),a

        ld      de,CARINF_SIZE  ; add ix,sizeof(struct CARINF)
        add     ix,de
        djnz    movecar_carloop

        ret

._movecar_mycar_stp         defb    0
._movecar_encar_stp         defb    0
._movecar_mycar_y           defb    0
._movecar_mycar_x           defb    0
._movecar_mycar_dirno       defb    0

._movecar_carno             defb    0
._movecar_flddir_num        defb    0
._movecar_flddir            defb    0
._movecar_newdirno          defb    0

._movecar_dy                defb    0
._movecar_dx                defb    0

movecar_init:
//            flddir = getflddir(carinfp->fld_y, carinfp->fld_x);
//            newdirno = 0xff;
        push ix
        ld      d,0
        ld      e,(ix+0)    ; _carinf->fld_y
        push    de
        ld      e,(ix+1)    ; _carinf->fld_x
        push    de
        call    _getflddir
        pop     de
        pop     de
        pop ix

        ld      a,l
        ld      (_movecar_flddir),a

        ld      a,h
        ld      (_movecar_flddir_num),a

        ld      a,0xff
        ld      (_movecar_newdirno),a

        ret

movecar_mycar_routine:
//                key = keycheck();
        call    _keycheck

movecar_mycar_v:
//                if (carinfp->step_x == 0) {
        ld      a,(ix+3)        ; carinfp->step_x
        cp      0
        jp      nz,movecar_mycar_h

//                    if (carinfp->dirno != DIR_UPNO) {
//                        if (key & KBIT_UP) {    // up arrow
//                            if ((carinfp->step_y != 0) || (flddir & DIR_UP)) {
//                                newdirno = DIR_UPNO;
//                            }
//                        }
//                    }
movecar_mycar_keyup:
        ld      a,(ix+6)        ; carinfp->dirno
        cp      DIR_UPNO
        jp      z,movecar_mycar_keyup_2
        ld      a,l
        and     KBIT_UP
        jp      z,movecar_mycar_keyup_2
        ld      a,(ix+2)        ; carinfp->step_y
        cp      0
        jp      nz,movecar_mycar_keyup_1
        ld      a,(_movecar_flddir)
        and     DIR_UP
        jp      z,movecar_mycar_keyup_2
movecar_mycar_keyup_1:
        ld      a,DIR_UPNO
        ld      (_movecar_newdirno),a
movecar_mycar_keyup_2:


//                    if (carinfp->dirno != DIR_DNNO) {
//                        if (key & KBIT_DN) {    // down arrow
//                            if ((carinfp->step_y != 0) || (flddir & DIR_DN)) {
//                                newdirno = DIR_DNNO;
//                            }
//                        }
//                    }
//                }
movecar_mycar_keydn:
        ld      a,(ix+6)        ; carinfp->dirno
        cp      DIR_DNNO
        jp      z,movecar_mycar_keydn_2
        ld      a,l
        and     KBIT_DN
        jp      z,movecar_mycar_keydn_2
        ld      a,(ix+2)        ; carinfp->step_y
        cp      0
        jp      nz,movecar_mycar_keydn_1
        ld      a,(_movecar_flddir)
        and     DIR_DN
        jp      z,movecar_mycar_keydn_2
movecar_mycar_keydn_1:
        ld      a,DIR_DNNO
        ld      (_movecar_newdirno),a
movecar_mycar_keydn_2:


//                if (carinfp->step_y == 0) {
movecar_mycar_h:
        ld      a,(ix+2)        ; carinfp->step_y
        cp      0
        jp      nz,movecar_mycar_keyspc
//                    if (carinfp->dirno != DIR_LTNO) {
//                        if (key & KBIT_LT) {    // left arrow
//                            if ((carinfp->step_x != 0) || (flddir & DIR_LT)) {
//                                newdirno = DIR_LTNO;
//                            }
//                        }
//                    }
movecar_mycar_keylt:
        ld      a,(ix+6)        ; carinfp->dirno
        cp      DIR_LTNO
        jp      z,movecar_mycar_keylt_2
        ld      a,l
        and     KBIT_LT
        jp      z,movecar_mycar_keylt_2
        ld      a,(ix+3)        ; carinfp->step_x
        cp      0
        jp      nz,movecar_mycar_keylt_1
        ld      a,(_movecar_flddir)
        and     DIR_LT
        jp      z,movecar_mycar_keylt_2
movecar_mycar_keylt_1:
        ld      a,DIR_LTNO
        ld      (_movecar_newdirno),a
movecar_mycar_keylt_2:


//                    if (carinfp->dirno != DIR_RTNO) {
//                        if (key & KBIT_RT) {    // right arrow
//                            if ((carinfp->step_x != 0) || (flddir & DIR_RT)) {
//                                newdirno = DIR_RTNO;
//                            }
//                        }
//                    }
//                }
movecar_mycar_keyrt:
        ld      a,(ix+6)        ; carinfp->dirno
        cp      DIR_RTNO
        jp      z,movecar_mycar_keyrt_2
        ld      a,l
        and     KBIT_RT
        jp      z,movecar_mycar_keyrt_2
        ld      a,(ix+3)        ; carinfp->step_x
        cp      0
        jp      nz,movecar_mycar_keyrt_1
        ld      a,(_movecar_flddir)
        and     DIR_RT
        jp      z,movecar_mycar_keyrt_2
movecar_mycar_keyrt_1:
        ld      a,DIR_RTNO
        ld      (_movecar_newdirno),a
movecar_mycar_keyrt_2:


//                if (key & KBIT_SPC) {           // space key
//                    // set smoke count
//                    if ((getsmokeidx() != -1) && (smokecnt == 0)) {
//                        smokecnt = 3;
//                    }
//                }
movecar_mycar_keyspc:
        ld      a,l
        and     KBIT_SPC
        ret     z

        call    _getsmokeidx
        ld      a,l
        cp      -1
        ret     z

        ld      a,(_smokecnt)
        cp      0
        ret     nz

        ld      a,3
        ld      (_smokecnt),a

        ret

movecar_encar_routine:
//                if (carinfp->free > 0) {
//                    carinfp->free--;
//                }
        ld      a,(ix+9)    ; _carinf->free
        cp      0
        jp      z,movecar_encar_routine1
        dec     a
        ld      (ix+9),a
        ret

movecar_encar_routine1:
//                     if ((carinfp->step_x == 0) && (carinfp->step_y == 0)) {
        ld      a,(ix+2)    ; _carinf->step_y
        cp      0
        ret     nz
        ld      a,(ix+3)    ; _carinf->step_x
        cp      0
        ret     nz

//                        if (flddir >= 0x0300) { // 3方向移動可能
        ld      a,(_movecar_flddir_num)
        cp      3
        ret     c

movecar_encar_up:
//                            if (carinfp->y > carinf[0].y) {
//                                dy = carinfp->y - carinf[0].y;
//                                if ((!(carno & 0x01)) && (carinf[0].dirno == DIR_UPNO)) {
//                                    dy += 6;
//                                }
//                                if (flddir & DIR_UP) {
//                                    newdirno = DIR_UPNO;
//                                }
//                            }
        ld      a,(_movecar_mycar_y)
        ld      d,(ix+4)    ; carinfp->y
        sub     a,d
        jp      nc,movecar_encar_dn

        neg
        ld      (_movecar_dy),a
        ld      a,(_movecar_carno)
        and     1
        jp      nz,movecar_encar_up_noadd
        ld      a,(_movecar_mycar_dirno)
        cp      DIR_UPNO
        jp      nz,movecar_encar_up_noadd
movecar_encar_up_add4:
        ld      a,(_movecar_dy)
        add     a,4*CHR_DY
        ld      (_movecar_dy),a
movecar_encar_up_noadd:
        ld      a,(_movecar_flddir)
        and     DIR_UP
        jp      z,movecar_encar_not_up
        ld      a,DIR_UPNO
        ld      (_movecar_newdirno),a
movecar_encar_not_up:
        jp      movecar_encar_lt


//                            else if (carinfp->y < carinf[0].y) {
//                                dy = carinf[0].y - carinfp->y;
//                                if ((!(carno & 0x01)) && (carinf[0].dirno == DIR_DNNO)) {
//                                    dy += 6;
//                                }
//                                if (flddir & DIR_DN) {
//                                    newdirno = DIR_DNNO;
//                                }
//                            }
movecar_encar_dn:
        ld      (_movecar_dy),a
        ld      a,(_movecar_carno)
        and     1
        jp      nz,movecar_encar_dn_noadd
        ld      a,(_movecar_mycar_dirno)
        cp      DIR_DNNO
        jp      nz,movecar_encar_dn_noadd
movecar_encar_dn_add4:
        ld      a,(_movecar_dy)
        add     a,4*CHR_DY
        ld      (_movecar_dy),a
movecar_encar_dn_noadd:
        ld      a,(_movecar_flddir)
        and     DIR_DN
        jp      z,movecar_encar_not_dn
        ld      a,DIR_DNNO
        ld      (_movecar_newdirno),a
movecar_encar_not_dn:
        jp      movecar_encar_lt

//                            if (carinfp->x > carinf[0].x) {
//                                dx = carinfp->x - carinf[0].x;
//                                if ((!(carno & 0x01)) && (carinf[0].dirno == DIR_LTNO)) {
//                                    dx += 6;
//                                }
//                                if (flddir & DIR_LT) {
//                                    if ((dx > dy) || (newdirno == -1))
//                                        newdirno = DIR_LTNO;
//                                }
//                            }
movecar_encar_lt:
        ld      a,(_movecar_mycar_x)
        ld      d,(ix+5)    ; carinfp->x
        sub    a,d
        jp      nc,movecar_encar_rt

        neg
        ld      (_movecar_dx),a
        ld      a,(_movecar_carno)
        and     1
        jp      nz,movecar_encar_lt_noadd
        ld      a,(_movecar_mycar_dirno)
        cp      DIR_LTNO
        jp      nz,movecar_encar_lt_noadd
movecar_encar_lt_add4:
        ld      a,(_movecar_dx)
        add     a,4*CHR_DX
        ld      (_movecar_dx),a
movecar_encar_lt_noadd:
        ld      a,(_movecar_flddir)
        and     DIR_LT
        jp      z,movecar_encar_not_lt
        ld      a,(_movecar_dy)
        ld      d,a
        ld      a,(_movecar_dx)
        sub     d
        jp      nc,movecar_encar_set_lt
        ld      a,(_movecar_newdirno)
        cp      0xff
        jp      nz,movecar_encar_not_lt
movecar_encar_set_lt:
        ld      a,DIR_LTNO
        ld      (_movecar_newdirno),a
movecar_encar_not_lt:
        jp      movecar_encar_free
        ret
//                            else if (carinfp->x < carinf[0].x) {
//                                dx = carinf[0].x - carinfp->x;
//                                if ((!(carno & 0x01)) && (carinf[0].dirno == DIR_RTNO)) {
//                                    dx += 6;
//                                }
//                                if (flddir & DIR_RT) {
//                                    if ((dx > dy) || (newdirno == -1))
//                                        newdirno = DIR_RTNO;
//                                }
//                            }
movecar_encar_rt:
        ld      (_movecar_dx),a
        ld      d,a
        ld      a,(_movecar_carno)
        and     1
        jp      nz,movecar_encar_rt_noadd
        ld      a,(_movecar_mycar_dirno)
        cp      DIR_RTNO
        jp      nz,movecar_encar_rt_noadd
movecar_encar_rt_add4:
        ld      a,4*CHR_DX
        add     a,d
        ld      (_movecar_dx),a
movecar_encar_rt_noadd:
        ld      a,(_movecar_flddir)
        and     DIR_RT
        jp      z,movecar_encar_not_rt
        ld      a,(_movecar_dy)
        ld      d,a
        ld      a,(_movecar_dx)
        sub     d
        jp      nc,movecar_encar_set_rt
        ld      a,(_movecar_newdirno)
        cp      0xff
        jp      nz,movecar_encar_not_rt
movecar_encar_set_rt:
        ld      a,DIR_RTNO
        ld      (_movecar_newdirno),a
movecar_encar_not_rt:

//                            if ((dx > 15) || (dy > 15)) {
//                                if (randum() > 0xf0) {
//                                    carinfp->free = 90;
//                                }
//                            }
movecar_encar_free:
        ld      a,(_movecar_dx)
        cp      16
        ret     c
        ld      a,(_movecar_dy)
        cp      16
        ret     c
        call    _randum
//        call    rand
        ld      a,0xf0
        cp      l
        ret     nc
        ld      a,90
        ld      (ix+9),a
        ret

movecar_freerun:
//            // free run
//            if (newdirno == 0xff) {
        ld      a,(_movecar_newdirno)
        cp      0xff
        ret     nz

//                newdirno = carinfp->dirno;
        ld      a,(ix+6)    ; carinfp->dirno
        ld      (_movecar_newdirno),a
        ld      b,a

//                if ((carinfp->step_y == 0) && (carinfp->step_x == 0)) {
        ld      a,(ix+2)    ; carinfp->step_y
        cp      0
        ret     nz

        ld      a,(ix+3)    ; carinfp->step_x
        cp      0
        ret     nz

//                    for (n = 0; n < 3; n++) {
//                        if (dirtbl[newdirno][n].dir & flddir) {
//                            break;
//                        }
//                    }
//                    newdirno = dirtbl[newdirno][n].dirno;
//                }
//            }
        ld      hl,_dirtbl
        ld      de, DIRTBL_SIZE
        ld      a,b
movecar_freerun1:
        cp      0
        jp      z,movecar_freerun2
        add     hl,de
        dec     a
        jp      movecar_freerun1
movecar_freerun2:

        ld      a,(_movecar_flddir)
        ld      c,a
        ld      b,4
movecar_freerun3:
        ld      a,(hl)      ; dirtbl[newdirno][n].dir

        and     c
        jp      z,movecar_freerun4
        inc     hl
        ld      a,(hl)      ; dirtbl[newdirno][n].dirno
        ld      (_movecar_newdirno),a
        ret

movecar_freerun4:
        inc     hl
        inc     hl
        djnz    movecar_freerun3

        ret

movecar_move_routine:
//            // move
//            carinfp->dirno = newdirno;
        ld      a,(_movecar_newdirno)
        ld      (ix+6),a

//            switch (carinfp->dirno) {
//                case DIR_UPNO:
//                    carinfp->y--;
//                    if (carinfp->step_y > 0) {
//                        carinfp->step_y--;
//                    }
//                    else {
//                        carinfp->step_y = CHR_DY-1;
//                        carinfp->fld_y--;
//                    }
//                    break;

movecar_collision:
movecar_move_up:
        cp      DIR_UPNO
        jp      nz,movecar_move_dn

        ld      a,(ix+4)    ; carinfp->y
        dec     a
        ld      (ix+4),a

        ld      a,(ix+2)    ; carinfp->step_y
        cp      0
        jp      z,movecar_move_up1
        dec     a
        ld      (ix+2),a
        ret
movecar_move_up1:
        ld      a,CHR_DY-1
        ld      (ix+2),a
        ld      a,(ix+0)    ; carinfp->fld_y
        dec     a
        ld      (ix+0),a
        ret

//                case DIR_DNNO:
//                    carinfp->y++;
//                    if (carinfp->step_y < CHR_DY-1) {
//                        carinfp->step_y++;
//                    }
//                    else {
//                        carinfp->step_y = 0;
//                        carinfp->fld_y++;
//                    }
//                    break;
movecar_move_dn:
        cp      DIR_DNNO
        jp      nz,movecar_move_lt

        ld      a,(ix+4)    ; carinfp->y
        inc     a
        ld      (ix+4),a

        ld      a,(ix+2)    ; carinfp->step_y
        cp      CHR_DY-1
        jp      nc,movecar_move_dn1
        inc     a
        ld      (ix+2),a
        ret
movecar_move_dn1:
        ld      a,0
        ld      (ix+2),a
        ld      a,(ix+0)    ; carinfp->fld_y
        inc     a
        ld      (ix+0),a
        ret

//                case DIR_LTNO:
//                    carinfp->x--;
//                    if (carinfp->step_x > 0) {
//                        carinfp->step_x -= 1;
//                    }
//                    else {
//                        carinfp->step_x = CHR_DX-1;
//                        carinfp->fld_x--;
//                    }
//                    break;
movecar_move_lt:
        cp      DIR_LTNO
        jp      nz,movecar_move_rt

        ld      a,(ix+5)    ; carinfp->x
        dec     a
        ld      (ix+5),a

        ld      a,(ix+3)    ; carinfp->step_x
        cp      0
        jp      z,movecar_move_lt1
        dec     a
        ld      (ix+3),a
        ret
movecar_move_lt1:
        ld      a,CHR_DX-1
        ld      (ix+3),a
        ld      a,(ix+1)    ; carinfp->fld_x
        dec     a
        ld      (ix+1),a
        ret

//                case DIR_RTNO:
//                    carinfp->x++;
//                    if (carinfp->step_x < CHR_DX-1) {
//                        carinfp->step_x += 1;
//                    }
//                    else {
//                        carinfp->step_x = 0;
//                        carinfp->fld_x++;
//                    }
//                    break;
movecar_move_rt:
        cp      DIR_RTNO
        ret     nz

        ld      a,(ix+5)    ; carinfp->x
        inc     a
        ld      (ix+5),a

        ld      a,(ix+3)    ; carinfp->step_x
        cp      CHR_DX-1
        jp      nc,movecar_move_rt1
        inc     a
        ld      (ix+3),a
        ret
movecar_move_rt1:
        ld      a,0
        ld      (ix+3),a
        ld      a,(ix+1)    ; carinfp->fld_x
        inc     a
        ld      (ix+1),a
        ret

    #endasm
}

int collision()
{
    #asm
//    ret = 0;
//    for (carno = 0; carno < car_num; carno++) {
        ld      a,0
        ld      (_collision_carno),a
        ld      (_collision_ret),a
        ld      a,(_car_num)
        ld      b,a
//        carinfp = &carinf[carno];
        ld      ix,_carinf
        ld      a,(ix+4)
        ld      (_collision_mycar_y),a
        ld      a,(ix+5)
        ld      (_collision_mycar_x),a

collision_loop:
        push    bc
//        if (carinfp->mode == CARM_NONE) {
//            continue;
        ld      a,(ix+8)    ; _carinf->mode
        cp      CARM_NONE
        jp      z,collision_loop_next

// challenging stag の速度アップ
        ld      a,(_collision_carno)
        cp      0
        jp      z,collision_getfldchr
        ld      a,(_challenging_stage)
        cp      0
        jp      z,collision_getfldchr
        ld      a,0
        ld      (_collision_fldchr),a
        jp      collision_encar

collision_getfldchr:
//        // 移動先の文字
//        c = getfldchr(carinfp->fld_y + ((carinfp->step_y !=2) ? 1 : 0),
//                      carinfp->fld_x + ((carinfp->step_x !=2) ? 1 : 0));
        push    ix
        ld      d,0
        ld      e,(ix+0)    ; carinfp->fld_y
        ld      a,(ix+2)    ; carinfp->step_y
        cp      2
        jp      nz,collision_getfldchr1
        inc     e
collision_getfldchr1:
        push    de
        ld      e,a
        ld      (_collision_fldy),a

        ld      e,(ix+1)    ; carinfp->fld_x
        ld      a,(ix+3)    ; carinfp->step_x
        cp      2
        jp      nz,collision_getfldchr2
        inc     e
collision_getfldchr2:
        push    de
        ld      e,a
//        ld      (_collision_fldx),a

        call _getfldchr
        ld      a,l
        ld      (_collision_fldchr),a
        pop     de
        pop     de
        pop     ix

//        if (carno == 0) {   // my car
        ld      a,(_collision_carno)
        cp      0
        jp      nz,collision_encar
collision_mycar:
        call    collision_mycar_routine
        jp      collision_loop_next
collision_encar:
        call    collision_encar_routine

collision_loop_next:
        pop     bc

        ld      a,(_collision_carno)
        inc     a
        ld      (_collision_carno),a

        ld      de,CARINF_SIZE  ; add ix,sizeof(struct CARINF)
        add     ix,de
        djnz    collision_loop

collision_loop_out:
        ld      a,(_collision_ret)
        ld      h,0
        ld      l,a
        ret

collision_mycar_routine:
//            // rock
//            if (c == CHR_ROCK) {
//                return (1);
//            }
        ld      a,(_collision_fldchr)
        cp      CHR_ROCK
        jp      z,collision_mycar_crash
//            // map
//            if ((carinf[0].step_y != 0) || (carinf[0].step_x != 0)) {
//                continue;
//            }
        ld      a,(ix+2)    ; carinfp->stepy
        cp      0
        ret     nz
        ld      a,(ix+3)    ; carinfp->stepx
        cp      0
        ret     nz

        ld      a,(_collision_fldchr)
        cp      CHR_SPC
        jp      z,collision_mycar_spc
        cp      CHR_SFLAG
        jp      z,collision_mycar_sflag
        cp      CHR_NFLAG
        jp      z,collision_mycar_nflag
        cp      CHR_LFLAG
        jp      z,collision_mycar_lflag
        ret

collision_mycar_crash:
        ld      e,0x01      ; 0001b

collision_setret:
        ld      a,(_collision_ret)
        or      a,e
        ld      (_collision_ret),a
        ret

collision_mycar_spc:
//                // smoke
//                case CHR_SPC:
//                    if (smokecnt > 0) {
        ld      a,(_smokecnt)
        cp      0
        ret     z
        ld      e,0x04      ; 0100b
        jp      collision_setret

collision_mycar_sflag:
        ld      e,0x02      ; 0010b
        jp      collision_setret

collision_mycar_nflag:
        ld      e,0x06      ; 0110b
        jp      collision_setret

collision_mycar_lflag:
        ld      e,0x0A      ; 1010b
        jp      collision_setret

collision_encar_routine:
//        else {  // enemy car
//            if (carinfp->mode == CARM_MOVE) {
        ld      a,(ix+8)        ; carinfp->mode
        cp      CARM_MOVE
        jp      nz,collision_encar_routine2

        ld      a,(_challenging_stage)
        cp      0
        jp      nz,collision_encar_routine2

//                if (carinfp->chrno == (carinfp->dirno << 1)) {
        ld      d,(ix+6)    ; _carinf->dirno << 1
        sla     d
        ld      a,(ix+7)    ; _carinf->chrno
        cp      d
        jp      nz,collision_encar_routine2

        ld      a,(_collision_fldchr)
        cp      CHR_ROCK
        jp      z,collision_encar_rock
        cp      CHR_SMOKE
        jp      z,collision_encar_smoke
        call    collision_encar_en2en
        jp      collision_encar_routine2

collision_encar_rock:
//                    if (c == CHR_ROCK) {
//                        carinfp->mode  = CARM_ROCK;
//                    }
        ld      a,CARM_ROCK
        ld      (ix+8),a
        call    collision_encar_turn
        jp      collision_encar_routine2

collision_encar_smoke:
//                    else if (c == CHR_SMOKE) {
//                        carinfp->mode  = CARM_SMOKE;
//                    }
        ld      a,CARM_SMOKE
        ld      (ix+8),a
        call    collision_encar_turn
        jp      collision_encar_routine2

collision_encar_turn:
//                    // 反転
//                        carinfp->dirno = dirtbl[carinfp->dirno][3].dirno;
        ld      a,(ix+6)    ; carinfp->dirno
        ld      hl,_dirtbl
        ld      de, DIRTBL_SIZE
collision_encar_turn1:
        cp      0
        jp      z,collision_encar_turn2
        add     hl,de
        dec     a
        jp      collision_encar_turn1
collision_encar_turn2:
        ld      de,7
        add     hl,de
        ld      a,(hl)
        ld      (ix+6),a    ; carinfp->dirno
        jp      movecar_collision   ; ret込み
//        ret

collision_encar_routine2:
//            if (carinfp->mode >= CARM_MOVE) {
        ld      a,(ix+8)        ; carinfp->mode
        cp      CARM_MOVE
        ret     c

//                if (abs(carinfp->y == carinf[0].y) +
//                    abs(carinfp->x == carinf[0].x) <= 2) {
//                    return (1);
//                }

        ld      e,(ix+4)
        ld      a,(_collision_mycar_y)
        sub     e
        jp      nc,collision_encar_routine_y
        neg
collision_encar_routine_y:
        cp      2
        ret     nc

        ld      e,(ix+5)
        ld      a,(_collision_mycar_x)
        sub      e
        jp      nc,collision_encar_routine_x
        neg
collision_encar_routine_x:
        cp      2
        jp      c,collision_mycar_crash

        ret

._collision_carno           defb    0
._collision_mycar_y         defb    0
._collision_mycar_x         defb    0
._collision_fldy            defb    0
._collision_fldx            defb    0
._collision_fldchr          defb    0
._collision_ret             defb    0

collision_encar_en2en:
//                        for (n = 1; n < car_num ; n++) {
//                            if (carno == n) {
//                                continue;
//                            }
//                            if ((carinfp->fld_x == carinf[n].fld_x) &&
//                                (carinfp->fld_y == carinf[n].fld_y)) {
//                                carinfp->mode  = CARM_EN2EN;
//                                break;
//                            }
//                        }

        ld      a,(ix+0)    ; carinfp->fld_y
        ld      (_collision_fldy),a
        ld      a,(ix+1)    ; carinfp->fld_x
        ld      (_collision_fldx),a

        push    bc
        push    de
        push    ix

        ld      ix,_carinf
        ld      b,1
        ld      c,0

collision_encar_en2en_loop:
        ld      de,CARINF_SIZE  ; add ix,sizeof(struct CARINF)
        add     ix,de

        ld      a,(_collision_carno)
        cp      b
        jp      z,collision_encar_en2en_next

        ld      e,(ix+0)    ; carinfp->fld_y
        ld      a,(_collision_fldy)
        cp      e
        jp      nz,collision_encar_en2en_next

        ld      e,(ix+1)    ; carinfp->fld_x
        ld      a,(_collision_fldx)
        cp      e
        jp      nz,collision_encar_en2en_next

        ld      c,1
        jp      collision_encar_en2en_loopout

collision_encar_en2en_next:
        inc     b
        ld      a,(_car_num)
        cp      b
        jp      nz,collision_encar_en2en_loop

collision_encar_en2en_loopout:
        pop     ix

        ld      a,c
        cp      0
        jp      z,collision_encar_en2en_ret

        ld      a,CARM_EN2EN
        ld      (ix+8),a
        call    collision_encar_turn

collision_encar_en2en_ret:
        pop     de
        pop     bc

        ret

    #endasm
}


void initsmoke()
{
    smokecnt = 0;
    memset(smokeinf, 0, sizeof(smokeinf));
}

int getsmokeidx()
{
    int  i;

    for (i = 0; i < SMOKENUM; i++) {
        if (smokeinf[i].time == 0) {
            return (i);
        }
    }
    return (-1);
}

void setsmoke()
{
    int n = getsmokeidx();
    if (n == -1) {
        return;
    }
    smokecnt--;
    // set smoke
    fldtbl[carinf[0].fld_y][carinf[0].fld_x] = CHR_SMOKE;
    smokeinf[n].fld_y = carinf[0].fld_y;
    smokeinf[n].fld_x = carinf[0].fld_x;
    smokeinf[n].time = SMOKETIME;
    makebak(&bakbuf[4*CHR_DY][4*CHR_DX],
            bakbuf_sy+4, bakbuf_sx+4, 1, 1, 0);
}

void clrsmoke()
{
    int  i, x, y;

    for (i = 0; i < SMOKENUM; i++) {
        if (smokeinf[i].time == 0) {
            continue;
        }
        y = smokeinf[i].fld_y;
        x = smokeinf[i].fld_x;
        fldtbl[y][x] = CHR_SPC;
    }
}

void smokeproc()
{
    int  i, x, y, by, bx;

    for (i = 0; i < SMOKENUM; i++) {
        if (smokeinf[i].time == 0) {
            continue;
        }
        smokeinf[i].time--;
        if (smokeinf[i].time == 0) {
            y = smokeinf[i].fld_y;
            x = smokeinf[i].fld_x;
            fldtbl[y][x] = CHR_SPC;
            by = y - bakbuf_sy;
            bx = x - bakbuf_sx;
            if ((by >= 0) && (by < 9) &&
                (bx >= 0) && (bx < 9)) {
                makebak(&bakbuf[by*CHR_DY][bx*CHR_DX],
                        bakbuf_sy+by, bakbuf_sx+bx, 1, 1, 0);
            }
        }
    }
}

/*
 *      ZX81 Boat Handling Module program
 */

#include <stdio.h>
#include <string.h>

#include "key.h"
#include "map.h"
#include "boat.h"
#include "objects.h"
#include "scrn.h"


// ==========================================================
// Speed Table Definition
// ==========================================================

typedef struct {
    char boat_y;        // Y position of the boat on screen
    char dy;            // Map scroll delta Y (-1=up, 0=none, 1=down)
    char speed_down;    // Next table index when Space key is released (Deceleration)
    char speed_up;      // Next table index when Space key is pressed (Acceleration)
    char can_steer;     // Steering availability flag (0: FALSE, 1: TRUE)
} SPEED_TBL;

static const SPEED_TBL speed_tbl[] = {
    // --- Stop / Deceleration States ---
    {33,  1,  1,  8, 0},// table 0: Stopped
    {33,  0,  0,  8, 0},// table 1: Stopped
    {32,  0,  1,  9, 1},// table 2: Decelerating
    {31,  0,  2, 10, 1},// table 3: Decelerating
    {30,  0,  3, 11, 1},// table 4: Decelerating
    {29,  0,  4, 12, 1},// table 5: Decelerating
    {28,  0,  5, 13, 1},// table 6: Decelerating
    {27,  0,  6, 14, 1},// table 7: Decelerating

    // --- Acceleration States ---
    {32,  0,  1,  9, 1},// table 8: Accelerating (Low)
    {31,  0,  2, 10, 1},// table 9: Accelerating
    {30, -1,  3, 11, 1},// table 10: Accelerating
    {29,  0,  4, 12, 1},// table 11: Accelerating
    {28, -1,  5, 13, 1},// table 12: Accelerating
    {27, -1,  6, 18, 1},// table 13: Accelerating
    {26, -1,  7, 15, 1},// table 14: Max Speed
    {26, -1, 14, 16, 1},// table 15: Max Speed
    {26, -1, 15, 17, 1},// table 16: Max Speed
    {26, -1, 16, 18, 1},// table 17: Max Speed
    {26, -1, 17, 18, 1} // table 18: Max Speed
};


// ==========================================================
// Steering / Tilt Table Definition
// ==========================================================

typedef struct {
    char dx;            // Horizontal move delta X (-1, 0, +1)
    char tilt;          // Sprite tilt index (0: Center, 1: 23°, 2: 45°)
    char tilt_down;     // Next table index when returning to center
    char tilt_left;     // Next table index when tilting left
    char tilt_right;    // Next table index when tilting right
} STEER_TBL;

static const STEER_TBL steer_tbl[] = {
    // --- Hard Left Turn ---
    { -1, 0,  1,  0,  1 },  // table 0: Move -1, Tilt 45° Left (Max)
    { -1, 0,  2,  0,  2 },  // table 1: Move -1, Tilt 45° Left (Max)
    { -1, 0,  3,  0,  3 },  // table 2: Move -1, Tilt 45° Left (Max)
    { -1, 0,  4,  0,  4 },  // table 3: Move -1, Tilt 45° Left (Max)
    { -1, 0,  5,  0,  5 },  // table 4: Move -1, Tilt 45° Left (Max)

    // --- Soft Left Turn ---
    { -1, 1,  6,  4,  6 },  // table 5: Move -1, Tilt 23° Left
    {  0, 1,  7,  5,  7 },  // table 6: Move  0, Tilt 23° Left
    { -1, 1,  8,  6,  8 },  // table 7: Move -1, Tilt 23° Left
    {  0, 1,  9,  7,  9 },  // table 8: Move  0, Tilt 23° Left

    // --- Center ---
    {  0, 2,  9,  8, 10 },  // table 9: Center Position

    // --- Soft Right Turn ---
    {  0, 3,  9,  9, 11 },  // table 10: Move  0, Tilt 23° Right
    {  1, 3, 10, 10, 12 },  // table 11: Move +1, Tilt 23° Right
    {  0, 3, 11, 11, 13 },  // table 12: Move  0, Tilt 23° Right
    {  1, 3, 12, 12, 14 },  // table 13: Move +1, Tilt 23° Right

    // --- Hard Right Turn ---
    {  1, 4, 13, 13, 18 },  // table 14: Move +1, Tilt 45° Right (Max)
    {  1, 4, 14, 14, 18 },  // table 15: Move +1, Tilt 45° Right (Max)
    {  1, 4, 15, 15, 18 },  // table 16: Move +1, Tilt 45° Right (Max)
    {  1, 4, 16, 16, 18 },  // table 17: Move +1, Tilt 45° Right (Max)
    {  1, 4, 17, 17, 18 }   // table 18: Move +1, Tilt 45° Right (Max)
};



#define BOAT_CH     5
#define BOAT_CW     5

unsigned char boat_ptn[2][2][5][BOAT_CH*BOAT_CW] =
{
 {
  {  // y:even, x:even  even-right45, right34, center, left23, left45
    {0x87,0x83,0x00,0x00,0x00, 0x85,0x00,0x03,0x04,0x00, 0x00,0x05,0x07,0x84,0x04, 0x00,0x02,0x82,0x80,0x00, 0x00,0x00,0x02,0x00,0x00},
    {0x87,0x86,0x04,0x00,0x00, 0x85,0x00,0x02,0x04,0x00, 0x00,0x82,0x03,0x86,0x00, 0x00,0x02,0x81,0x07,0x00, 0x00,0x00,0x01,0x00,0x00},
    {0x00,0x87,0x82,0x00,0x00, 0x00,0x06,0x02,0x04,0x00, 0x00,0x82,0x86,0x05,0x00, 0x00,0x82,0x83,0x05,0x00, 0x00,0x00,0x01,0x00,0x00},
    {0x00,0x00,0x83,0x86,0x00, 0x00,0x06,0x00,0x85,0x00, 0x87,0x03,0x86,0x05,0x00, 0x02,0x80,0x06,0x00,0x00, 0x00,0x00,0x01,0x00,0x00},
    {0x00,0x00,0x87,0x83,0x00, 0x00,0x06,0x01,0x85,0x00, 0x81,0x03,0x05,0x05,0x00, 0x85,0x82,0x07,0x00,0x00, 0x00,0x02,0x00,0x00,0x00}
  },
  {  // y:even, x:odd  even-right45, right34, center, left23, left45
    {0x00,0x83,0x04,0x00,0x00, 0x00,0x05,0x02,0x86,0x00, 0x00,0x85,0x85,0x03,0x82, 0x00,0x00,0x84,0x81,0x05, 0x00,0x00,0x00,0x01,0x00},
    {0x00,0x06,0x83,0x00,0x00, 0x00,0x05,0x00,0x86,0x00, 0x00,0x85,0x06,0x03,0x04, 0x00,0x00,0x86,0x80,0x01, 0x00,0x00,0x02,0x00,0x00},
    {0x00,0x00,0x81,0x04,0x00, 0x00,0x87,0x01,0x86,0x00, 0x00,0x85,0x06,0x81,0x00, 0x00,0x85,0x83,0x81,0x00, 0x00,0x00,0x02,0x00,0x00},
    {0x00,0x00,0x87,0x06,0x04, 0x00,0x87,0x01,0x00,0x05, 0x00,0x06,0x03,0x81,0x00, 0x00,0x84,0x82,0x01,0x00, 0x00,0x00,0x02,0x00,0x00},
    {0x00,0x00,0x00,0x83,0x04, 0x00,0x87,0x03,0x00,0x05, 0x87,0x07,0x84,0x85,0x00, 0x00,0x80,0x81,0x01,0x00, 0x00,0x00,0x01,0x00,0x00}
  }
 },
 {
  {  // y:odd, x:even  even-right45, right34, center, left23, left45
    {0x00,0x00,0x00,0x00,0x00, 0x85,0x03,0x83,0x00,0x00, 0x02,0x04,0x83,0x82,0x00, 0x00,0x86,0x05,0x81,0x01, 0x00,0x00,0x84,0x03,0x00},
    {0x00,0x04,0x00,0x00,0x00, 0x85,0x02,0x86,0x00,0x00, 0x02,0x04,0x83,0x05,0x00, 0x00,0x84,0x87,0x81,0x00, 0x00,0x00,0x07,0x01,0x00},
    {0x00,0x00,0x04,0x00,0x00, 0x00,0x85,0x84,0x00,0x00, 0x00,0x05,0x04,0x05,0x00, 0x00,0x07,0x02,0x05,0x00, 0x00,0x03,0x07,0x01,0x00},
    {0x00,0x00,0x00,0x04,0x00, 0x00,0x87,0x03,0x85,0x00, 0x00,0x82,0x04,0x06,0x00, 0x85,0x83,0x85,0x01,0x00, 0x00,0x03,0x05,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00, 0x00,0x87,0x06,0x84,0x00, 0x87,0x82,0x04,0x06,0x00, 0x84,0x04,0x82,0x01,0x00, 0x02,0x84,0x01,0x00,0x00}
  },
  {  // y:odd, x:odd  even-right45, right34, center, left23, left45
    {0x00,0x00,0x00,0x00,0x00, 0x00,0x07,0x86,0x04,0x00, 0x00,0x86,0x87,0x81,0x04, 0x00,0x02,0x81,0x87,0x07, 0x00,0x00,0x02,0x07,0x01},
    {0x00,0x87,0x00,0x00,0x00, 0x00,0x05,0x03,0x04,0x00, 0x00,0x86,0x87,0x81,0x00, 0x00,0x02,0x05,0x83,0x05, 0x00,0x00,0x85,0x03,0x00},
    {0x00,0x00,0x87,0x00,0x00, 0x00,0x00,0x07,0x05,0x00, 0x00,0x85,0x87,0x85,0x00, 0x00,0x85,0x01,0x84,0x00, 0x00,0x02,0x84,0x03,0x00},
    {0x00,0x00,0x00,0x87,0x00, 0x00,0x00,0x06,0x01,0x05, 0x00,0x85,0x83,0x87,0x01, 0x00,0x82,0x04,0x07,0x00, 0x00,0x02,0x84,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x83,0x03,0x05, 0x00,0x81,0x83,0x87,0x01, 0x02,0x82,0x85,0x06,0x00, 0x00,0x03,0x07,0x00,0x00}
  }
 }
};

unsigned char boat_spin_ptn[16][BOAT_CH*BOAT_CW] =
{
 {0x00,0x87,0x82,0x00,0x00, 0x00,0x06,0x02,0x04,0x00, 0x00,0x82,0x86,0x05,0x00, 0x00,0x82,0x83,0x05,0x00, 0x00,0x00,0x01,0x00,0x00},
 {0x00,0x00,0x83,0x86,0x00, 0x00,0x06,0x00,0x85,0x00, 0x87,0x03,0x86,0x05,0x00, 0x02,0x80,0x06,0x00,0x00, 0x00,0x00,0x01,0x00,0x00},
 {0x00,0x00,0x87,0x83,0x00, 0x00,0x06,0x01,0x85,0x00, 0x81,0x03,0x05,0x05,0x00, 0x85,0x82,0x07,0x00,0x00, 0x00,0x02,0x00,0x00,0x00},
 {0x00,0x00,0x87,0x83,0x00, 0x00,0x81,0x01,0x87,0x01, 0x84,0x04,0x05,0x06,0x00, 0x02,0x82,0x03,0x00,0x00, 0x00,0x00,0x00,0x00,0x00},
 {0x00,0x00,0x00,0x00,0x00, 0x85,0x84,0x03,0x83,0x00, 0x84,0x87,0x01,0x83,0x01, 0x02,0x03,0x03,0x00,0x00, 0x00,0x00,0x00,0x00,0x00},
 {0x00,0x83,0x00,0x00,0x00, 0x85,0x05,0x07,0x04,0x00, 0x03,0x83,0x01,0x85,0x00, 0x00,0x02,0x86,0x83,0x01, 0x00,0x00,0x00,0x00,0x00},
 {0x87,0x81,0x04,0x00,0x00, 0x81,0x01,0x07,0x04,0x00, 0x02,0x07,0x01,0x86,0x00, 0x00,0x02,0x86,0x81,0x00, 0x00,0x00,0x00,0x00,0x00},
 {0x00,0x83,0x05,0x00,0x00, 0x85,0x03,0x85,0x04,0x00, 0x00,0x07,0x01,0x86,0x00, 0x00,0x02,0x83,0x85,0x00, 0x00,0x00,0x00,0x01,0x00},
 {0x00,0x83,0x82,0x04,0x00, 0x00,0x82,0x87,0x05,0x00, 0x00,0x05,0x01,0x05,0x00, 0x00,0x85,0x85,0x00,0x00, 0x00,0x00,0x01,0x00,0x00},
 {0x00,0x00,0x82,0x04,0x00, 0x00,0x81,0x02,0x84,0x00, 0x87,0x01,0x03,0x05,0x00, 0x85,0x87,0x06,0x00,0x00, 0x00,0x01,0x00,0x00,0x00},
 {0x00,0x00,0x81,0x83,0x00, 0x00,0x06,0x05,0x84,0x04, 0x87,0x01,0x03,0x07,0x00, 0x85,0x83,0x03,0x00,0x00, 0x00,0x00,0x00,0x00,0x00},
 {0x00,0x00,0x87,0x04,0x00, 0x00,0x06,0x05,0x80,0x00, 0x85,0x00,0x86,0x06,0x01, 0x86,0x83,0x03,0x00,0x00, 0x00,0x00,0x00,0x00,0x00},
 {0x00,0x00,0x00,0x00,0x00, 0x87,0x06,0x84,0x84,0x00, 0x86,0x04,0x86,0x85,0x01, 0x00,0x02,0x03,0x03,0x00, 0x00,0x00,0x00,0x00,0x00},
 {0x87,0x83,0x00,0x00,0x00, 0x86,0x00,0x84,0x04,0x00, 0x02,0x04,0x05,0x81,0x01, 0x00,0x02,0x86,0x03,0x00, 0x00,0x00,0x00,0x00,0x00},
 {0x87,0x83,0x00,0x00,0x00, 0x85,0x00,0x03,0x04,0x00, 0x00,0x05,0x07,0x84,0x04, 0x00,0x02,0x82,0x80,0x00, 0x00,0x00,0x02,0x00,0x00},
 {0x87,0x86,0x04,0x00,0x00, 0x85,0x00,0x02,0x04,0x00, 0x00,0x82,0x03,0x86,0x00, 0x00,0x02,0x81,0x07,0x00, 0x00,0x00,0x01,0x00,0x00}
};

char boat_x;
char boat_y;
char boat_speed;
char boat_tilt;
char tilt_level;

typedef struct {
    unsigned char kbit;   // KBIT_LT / KBIT_RT / 0
    unsigned char count;  // このステップを何回繰り返すか
} Voyage_Step;

static const Voyage_Step voyage_steps[] = {
    {0,         13},
    {KBIT_LT,    6},
    {0,         21},
    {KBIT_RT,    8},
    {0,         14},
    {0,          0} 
};
static unsigned char voyage_step_index;
static unsigned char voyage_step_counter;


#asm
; --- Calculation Subroutines ---
        PUBLIC calc_speed_tbl_ptr

calc_speed_tbl_ptr:
        ld      a, (_boat_speed)
        ld      l, a
        ld      h, 0
        ld      e, l
        ld      d, h
        add     hl, hl                  ; A * 2
        add     hl, hl                  ; A * 4
        add     hl, de                  ; A * 5

        ld      de, _speed_tbl
        add     hl, de
        push    hl
        pop     ix                      ; IX = &speed_tbl[boat_speed]
        ret

calc_steer_tbl_ptr:
        ld      a, (_tilt_level)
        ld      l, a
        ld      h, 0
        ld      e, l
        ld      d, h
        add     hl, hl                  ; A * 2
        add     hl, hl                  ; A * 4
        add     hl, de                  ; A * 5

        ld      de, _steer_tbl
        add     hl, de
        push    hl
        pop     ix                      ; IX = &steer_tbl[tilt_level]
        ret
#endasm

// ----------------------------------------------------------
// 初期化
// ----------------------------------------------------------
void boat_init(void)
{
/*
    boat_x = 18;
    boat_speed = 0;
    boat_y = speed_tbl[boat_speed].boat_y;

    tilt_level = STEER_CENTER;
    boat_tilt = steer_tbl[tilt_level].tilt;

    voyage_step_index = 0;
    voyage_step_counter = 0;
*/
#asm

        ld      a,20
        ld      (_boat_x), a

        ld      a,0
        ld      (_boat_speed),a
        call    calc_speed_tbl_ptr      ; IX = &speed_tbl[boat_speed]
        ld      a,(ix+0)                ; boat_y
        ld      (_boat_y), a

        ld      a,STEER_CENTER
        ld      (_tilt_level),a
        call    calc_steer_tbl_ptr      ; IX = &steer_tbl[tilt_level]
        ld      a,(ix+1)                ; tilt
        ld      (_boat_tilt), a

        ld      a,0
        ld      (_voyage_step_index),a
        ld      (_voyage_step_counter),a
#endasm
}

// ----------------------------------------------------------
// 描画と衝突判定
// ----------------------------------------------------------
int boat_render_normal(void)
{
//    unsigned char *chrp;
//
//    chrp = &boat_ptn[boat_y & 1][boat_x & 1][boat_tilt][0];
//
//    return boat_render(chrp);
    return boat_render(&boat_ptn[boat_y & 1][boat_x & 1][boat_tilt][0]);
}

int boat_render_spin(int  spin_frame)
{
//    unsigned char *chrp;
//
//    chrp = &boat_spin_ptn[spin_frame][0];
//
//    return boat_render(chrp);
    return boat_render(&boat_spin_ptn[spin_frame][0]);
}

int boat_render(unsigned char *chrp)
{
/*
    unsigned char c;
    unsigned char *pos;
    unsigned char p;
    int p1;
    int  y, x;
    int  collision = 0;

    pos = getrenderaddr(boat_y>>1, boat_x>>1);

    for (y = 0; y < 5; y++)
    {
        for (x = 0; x < 5; x++)
        {
            // 左上0x01、右上0x02、左下0x04だが
            // 右下が含まれると0x80＋0x07（左上、右下、左下反転)となるため
            // 合成時に0x80があればBIT反転後orし、再度反転する処理を入れる

            // 船のBIT反転
            c = *chrp;
            if (c & 0x80)
            {
                c ^= 0x07;
            }

            //  mapのBIT反転
            p = *pos;
            if (p & 0x80)
            {
                p ^= 0x07;
            }

            // 合成
            p1 = p;
            p |= c;

            // 衝突判定
            if ((p ^ c) != p1)
            {
                collision = 1;
                // 衝突位置を仮に＊に置き換え
                p = 0x17;
            }
            else
            {
                // 合成結果をBIT戻しして書き戻し
                if (p & 0x80)
                {
                    p ^= 0x07;
                }
            }
//　　後日のアセンブラ化のためBIT反転処理メモ
//    bit 7, a        ; bit7 をチェック
//    jr  z, skip     ; 0 ならスキップ
//    xor 0x07        ; bit0?2 を反転
//skip:
            *pos = p;
            pos++;
            chrp++;
        }
        pos += RENDER_DX-5;
    }

    return (collision);
*/
#asm
        push hl                     ; hl = chrp

; hl = getrenderaddr(boat_y >> 1, boat_x >> 1)
        ld      hl,(_boat_y)
        srl     l
        ld      de,(_boat_x)
        srl     e
        call    asm_getrenderaddr      ; hl = render address

; de = chrp (boat pattern pointer)
        pop     de                  ; de <- hl(chrp)

; initialize collision flag (0 = no collision, 1 = collision)
        xor     a
        ld      (_boat_render_collision), a

; for (y = 0; y < BOAT_CH; y++)
        ld      c, BOAT_CH

boat_render_loop_y:

; for (x = 0; x < BOAT_CW; x++)
        ld      b, BOAT_CW

boat_render_loop_x:

        push    bc

; load boat character
        ld      a, (de)
        or      a                ; skip drawing if a = 0 (space)
        jr      z, boat_render_next_x

        bit     7, a             ; bit-flip check
        jr      z, boat_render_skip_bitflip_boat
        xor     0x07
boat_render_skip_bitflip_boat:
        ld      c, a

; load destination character (map tile)
        ld      a, (hl)
        bit     7, a             ; bit-flip check
        jr      z, boat_render_skip_bitflip_map
        xor     0x07
boat_render_skip_bitflip_map:
        ld      b, a

; collision check: (p & c) != 0
        and     c
        jr      z, boat_render_no_collision

        ld      a, 1
        ld      (_boat_render_collision), a

; mark collision position with '*'
        ld      (hl), 0x17
        jr      boat_render_next_x

boat_render_no_collision:
; merge boat and map bits
        ld      a, b
        or      c

; final bit-flip check
        bit     7, a
        jr      z, boat_render_skip_bitflip_final
        xor     0x07

boat_render_skip_bitflip_final:
        ld      (hl), a

; next x
boat_render_next_x:
        pop     bc
        inc     hl
        inc     de
        djnz    boat_render_loop_x

; next y (move to next row)
boat_render_next_y:
        push    bc
        ld      bc, RENDER_DX - BOAT_CW
        add     hl, bc
        pop     bc

        dec     c
        jr      nz, boat_render_loop_y

; return collision flag
        ld      h, 0
        ld      a, (_boat_render_collision)
        ld      l, a

        ret

._boat_render_collision     defb    0

#endasm
}

/**
 * 出航処理
 * @return dy: boat_moveの移動量 (0: 出航完了)
 */
int boat_bon_voyage(void)
{
    const Voyage_Step *step = &voyage_steps[voyage_step_index];
    int dy;

    // 終端（count == 0）に達していたら出航完了
    if (step->count == 0) {
        return 0;
    }

    // カウンタの更新処理
    voyage_step_counter++;
    if (voyage_step_counter >= step->count) {
        voyage_step_counter = 0;
        voyage_step_index++; // 次のステップへ
    }

    return KBIT_SPC | step->kbit;
}

// ----------------------------------------------------------
// Movement processing
//    Deceleration processing based on key input and elapsed time
// ----------------------------------------------------------
int boat_move(unsigned char key)
{
/*
    // --- Space key check ---
    if (key & KBIT_SPC) {
        boat_speed = speed_tbl[boat_speed].speed_up;
    } else {
        boat_speed = speed_tbl[boat_speed].speed_down;
    }

    // --- Check if steering is possible (can_steer) ---
    int  turn = 0;
    int  dx = 0;
    if (speed_tbl[boat_speed].can_steer) {
        if (key & KBIT_LT)
        {
            if (key & KBIT_SPC)
            {
                tilt_level = steer_tbl[tilt_level].tilt_left;
                dx = steer_tbl[tilt_level].dx;
                turn = 1;
            }
            else
            {
                if (tilt_level == STEER_CENTER)
                {
                    dx = -1;
                }
            }
        }
        else if (key & KBIT_RT)
        {
            if (key & KBIT_SPC)
            {
                tilt_level = steer_tbl[tilt_level].tilt_right;
                dx = steer_tbl[tilt_level].dx;
                turn = 1;
            }
            else
            {
                if (tilt_level == STEER_CENTER)
                {
                    dx = 1;
                }
            }
        }
    }

    // --- Reset tilt process (turn == 0) ---
    if (turn == 0)
    {
         tilt_level = steer_tbl[tilt_level].tilt_down;
        if (dx == 0)
        {
            dx = steer_tbl[tilt_level].dx;
        }
    }
    boat_tilt = steer_tbl[tilt_level].tilt;

    // --- Horizontal movement (Update boat_x) ---
    if (dx == -1)
    {
        if (boat_x > 0)
        {
            boat_x--;
        }
    }
    else if (dx == 1)
    {
        if (boat_x < 36)
        {
            boat_x++;
        }
    }

    boat_y = speed_tbl[boat_speed].boat_y;

    return (speed_tbl[boat_speed].dy);
*/
#asm
        ld      a, l
        ld      (_boat_move_key), a

        call    calc_speed_tbl_ptr      ; IX = &speed_tbl[boat_speed]

; --- Space key check ---
        ld      a, (_boat_move_key)
        and     #KBIT_SPC
        jr      z, boat_move_speed_down

boat_move_speed_up:
        ld      a, (ix+3)               ; speed_up
        jr      boat_move_set_speed

boat_move_speed_down:
        ld      a, (ix+2)               ; speed_down

boat_move_set_speed:
        ld      (_boat_speed), a
        call    calc_speed_tbl_ptr      ; IX = &speed_tbl[boat_speed]

; --- Check if steering is possible (can_steer) ---
        ld      c, 0                    ; C = dx (default 0)
        ld      b, 0                    ; B = turn (default 0)

        ld      a, (ix+4)               ; can_steer
        or      a
        jr      z, boat_move_skip_turn  ; Skip steering if can_steer == 0

; --- Steering control ---
        call    calc_steer_tbl_ptr      ; IX = &steer_tbl[tilt_level]

; --- Left key check ---
        ld      a, (_boat_move_key)
        and     #KBIT_LT
        jr      z, boat_move_check_rt

; Left key pressed
        ld      a, (_boat_move_key)
        and     #KBIT_SPC
        jr      z, boat_move_lt_no_spc

; LT + Space pressed
        ld      a, (ix+3)               ; tilt_left
        ld      (_tilt_level), a
        call    calc_steer_tbl_ptr      ; IX = &steer_tbl[tilt_level]

        ld      c, (ix+0)               ; dx
        ld      b, 1                    ; turn = 1
        jr      boat_move_turn_end

boat_move_lt_no_spc:
        ld      a, (_tilt_level)
        cp      STEER_CENTER            ; Updated from LR_CENTER
        jr      nz, boat_move_turn_end
        ld      c, -1                   ; dx = -1
        jr      boat_move_turn_end

; --- Right key check ---
boat_move_check_rt:
        ld      a, (_boat_move_key)
        and     #KBIT_RT
        jr      z, boat_move_turn_end

; Right key pressed
        ld      a, (_boat_move_key)
        and     #KBIT_SPC
        jr      z, boat_move_rt_no_spc

; RT + Space pressed
        ld      a, (ix+4)               ; tilt_right
        ld      (_tilt_level), a

; Move IX to updated tilt_level
        call    calc_steer_tbl_ptr      ; IX = &steer_tbl[tilt_level]

        ld      c, (ix+0)               ; dx
        ld      b, 1                    ; turn = 1
        jr      boat_move_turn_end

boat_move_rt_no_spc:
        ld      a, (_tilt_level)
        cp      STEER_CENTER            ; Updated from LR_CENTER
        jr      nz, boat_move_turn_end
        ld      c, 1                    ; dx = 1

boat_move_turn_end:

boat_move_skip_turn:
; --- Reset tilt process (turn == 0) ---
        ld      a, b                    ; A = turn
        or      a
        jr      nz, boat_move_update_tilt

; If turn == 0
        call    calc_steer_tbl_ptr      ; IX = &steer_tbl[tilt_level]

        ld      a, (ix+2)               ; tilt_down
        ld      (_tilt_level), a

        ld      a, c                    ; A = dx
        or      a
        jr      nz, boat_move_update_tilt

; If dx == 0
        call    calc_steer_tbl_ptr      ; IX = &steer_tbl[tilt_level]
        ld      c, (ix+0)               ; dx = steer_tbl[tilt_level].dx

boat_move_update_tilt:
; Update boat_tilt
        call    calc_steer_tbl_ptr      ; IX = &steer_tbl[tilt_level]
        ld      a, (ix+1)               ; tilt
        ld      (_boat_tilt), a

; --- Horizontal movement (Update boat_x) ---
; C = dx (-1, 0, 1)
        ld      a, c
        cp      -1
        jr      nz, boat_move_check_dx_right

; dx == -1
        ld      a, (_boat_x)
        or      a
        jr      z, boat_move_update_y
        dec     a
        ld      (_boat_x), a
        jr      boat_move_update_y

boat_move_check_dx_right:
        cp      1
        jr      nz, boat_move_update_y

; dx == 1
        ld      a, (_boat_x)
        cp      36
        jr      nc, boat_move_update_y
        inc     a
        ld      (_boat_x), a

boat_move_update_y:
        call    calc_speed_tbl_ptr      ; IX = &speed_tbl[boat_speed]

        ld      a, (ix+0)               ; boat_y
        ld      (_boat_y), a

        ld      a, (ix+1)               ; dy
        ld      l, a
        rla                             ; If dy is negative H=0xFF, if positive H=0x00
        sbc     a, a
        ld      h, a

        ret

._boat_move_key     defb    0

#endasm
}

// ----------------------------------------------------------
// int boat_contact(struct ACTIVE_TBL *actp)
//  Rectangle collision + X-center contact check
// ----------------------------------------------------------
int  boat_contact(struct ACTIVE_TBL *actp)
{
/*
    // オブジェクトの矩形（左下基準）
    int obj_w = (actp->w * 2)-1;
    int obj_h = (actp->h * 2);
    int obj_left = actp->x;
    int obj_top  = actp->y - obj_h;
    int obj_right  = obj_left + obj_w - 1;
    int obj_bottom = obj_top  + obj_h - 1;

    // ボートの矩形（左上基準）
    int boat_w = 5;
    int boat_h = 5;
    int boat_left   = boat_x+2;
    int boat_top    = boat_y+2;
    int boat_right  = boat_left + boat_w - 1;
    int boat_bottom = boat_top  + boat_h - 1;

    // 接触判定
    if (boat_left   <= obj_right  &&
        boat_right  >= obj_left   &&
        boat_top    <= obj_bottom &&
        boat_bottom >= obj_top)
    {
        // X方向センター接触判定
        int boat_cx = (boat_left + boat_right) / 2;
        int obj_cx  = (obj_left  + obj_right) / 2;
//        if ((abs(boat_cx - obj_cx) < 2) &&
        int diff = boat_cx - (obj_cx - 1);
        if ((diff >= 0 && diff <= 2) &&
            (boat_top >= obj_top) &&
            (boat_tilt == TILT_CENTER))
        {
            return 2;   // 直進状態でXセンター接触
        }

        return 1;
    }

    return 0;
*/
#asm
        push    hl
        pop     ix

PUBLIC asm_boat_contact
asm_boat_contact:
; ----------------------------------------------------------
; 1. Object rectangle (left-bottom based)
; ----------------------------------------------------------

        ld      a,(ix+4)        ; w
        add     a,a             ; w*2
        dec     a               ; (w*2)-1
        ld      b,a             ; obj_w = B

        ld      a,(ix+3)        ; h
        add     a,a             ; h*2
        ld      c,a             ; obj_h = C

        ld      a,(ix+2)        ; x
//        ld      (_boat_contact_obj_left),a
        ld      d,a

        ld      a,(ix+1)        ; y
        sub     c               ; y - obj_h
//        ld      (_boat_contact_obj_top),a
        ld      e,a

        ld      a,d             ; obj_left + obj_w -1
        add     a,b
        dec     a
        ld      (_boat_contact_obj_right),a

        ld      a,e             ; obj_top + obj_h -1
        add     a,c
        dec     a
        ld      (_boat_contact_obj_bottom),a

; ----------------------------------------------------------
; 2. Boat rectangle (left-top based)
; ----------------------------------------------------------

        ld      a,(_boat_x)
        add     a,2
//        ld      (_boat_contact_boat_left),a
        ld      b,a

        ld      a,(_boat_y)
        add     a,2
//        ld      (_boat_contact_boat_top),a
        ld      c,a

        ld      a,b
        add     a,BOAT_CW
        dec     a
        ld      (_boat_contact_boat_right),a

        ld      a,c
        add     a,BOAT_CH
        dec     a
        ld      (_boat_contact_boat_bottom),a

; ----------------------------------------------------------
; 3. AABB collision check
; ----------------------------------------------------------

; boat_left <= obj_right  -->  obj_right < boat_left
//        ld      a,(_boat_contact_boat_left)
//        ld      h,a
        ld      h,b
        ld      a,(_boat_contact_obj_right)
        cp      h
        jr      c,boat_contact_no_contact    ; obj_right < boat_left

; boat_right >= obj_left  -->  boat_right < obj_left
//        ld      a,(_boat_contact_obj_left)
//        ld      h,a
        ld      h,d
        ld      a,(_boat_contact_boat_right)
        cp      h
        jr      c,boat_contact_no_contact    ; boat_right < obj_left

; boat_top <= obj_bottom  -->  obj_bottom < boat_top
//        ld      a,(_boat_contact_boat_top)
//        ld      h,a
        ld     h,c
        ld      a,(_boat_contact_obj_bottom)
        cp      h
        jr      c,boat_contact_no_contact    ; obj_bottom < boat_top

; boat_bottom >= obj_top  -->  boat_bottom < obj_top
//        ld      a,(_boat_contact_obj_top)
//        ld      h,a
        ld      h,e
        ld      a,(_boat_contact_boat_bottom)
        cp      h
        jr      c,boat_contact_no_contact    ; boat_bottom < obj_top

; ----------------------------------------------------------
; 4. X-center contact check
; ----------------------------------------------------------

; boat_cx = (boat_left + boat_right) >> 1
//        ld      a,(_boat_contact_boat_left)
//        ld      h,a
        ld      h,b
        ld      a,(_boat_contact_boat_right)
        add     a,h
        srl     a
//        ld      (_boat_contact_boat_cx),a
        ld      l,a

; obj_cx = (obj_left + obj_right) >> 1
//        ld      a,(_boat_contact_obj_left)
//        ld      h,a
        ld      h,d
        ld      a,(_boat_contact_obj_right)
        add     a,h
        srl     a
//        ld      (_boat_contact_obj_cx),a

; diff = boat_cx - (obj_cx - 1)
//        ld      a,(_boat_contact_obj_cx)
        dec     a
        ld      h,a            ; obj_cx - 1

//        ld      a,(_boat_contact_boat_cx)
        ld      a,l
        sub     h              ; diff = A

; diff >= 0 ?
        jr      c,_boat_contact_return_1

; diff <= 2 ?
        cp      3
        jr      nc,_boat_contact_return_1

; boat_top >= obj_top -->  boat_top < obj_top
//        ld      a,(_boat_contact_obj_top)
//        ld      h,a
        ld      h,e
//        ld      a,(_boat_contact_boat_top)
        ld      a,c
        cp      h
        jr      c,_boat_contact_return_1

; boat_tilt == TILT_CENTER ?
        ld      a,(_boat_tilt)
        cp      TILT_CENTER
        jr      nz,_boat_contact_return_1

; X-center contact
        ld      hl,2
        ret

; ----------------------------------------------------------
; Normal contact
; ----------------------------------------------------------
_boat_contact_return_1:
        ld      hl,1
        ret

; ----------------------------------------------------------
; No contact
; ----------------------------------------------------------
boat_contact_no_contact:
        ld      hl,0
        ret

; ----------------------------------------------------------
; Local variables (static storage)
; ----------------------------------------------------------

//._boat_contact_obj_left     defb 0    -> REGISTER D
//._boat_contact_obj_top      defb 0    -> REGISTER E
._boat_contact_obj_right    defb 0
._boat_contact_obj_bottom   defb 0
//._boat_contact_boat_left    defb 0    -> REGISTER B
//._boat_contact_boat_top     defb 0    -> REGISTER C
._boat_contact_boat_right   defb 0
._boat_contact_boat_bottom  defb 0
//._boat_contact_boat_cx      defb 0    -> REGISTER L
//._boat_contact_obj_cx       defb 0    -> REGISTER A

#endasm
}
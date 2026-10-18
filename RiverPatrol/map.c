/*
 *      ZX81 Map Handling Module program
 */

#include <stdio.h>
#include <string.h>

#include "map.h"
#include "mapdata.h"
#include "scrn.h"

// river data
char rock_chr[4][2] = {
	{0x81, 0x05},{0x80, 0x01},
	{0x03, 0x00},{0x87, 0x04}
};

int start_y = 0;

// Render buffers for smooth 2x2 tile scrolling (even/odd frames)
unsigned char render_buf_even[RENDER_DY][RENDER_DX];
unsigned char render_buf_odd[RENDER_DY][RENDER_DX];
unsigned char render_buf[RENDER_DY][RENDER_DX];

int map_scroll_y;
char map_min_x;
char map_max_x;

#asm
;----------------------------------------
; HL = HL * 22
;----------------------------------------
_HL_MULT22:
    push    de

    ld      d, h
    ld      e, l
    add     hl, hl      ; *2
    add     hl, hl      ; *4
    add     hl, de      ; *5
    add     hl, hl      ; *10
    add     hl, de      ; *11
    add     hl, hl      ; *22

    pop     de
    ret
#endasm


unsigned char *getrenderaddr(int posy, int posx)
{
//    return (&render_buf[posy][posx]);
#asm
    ld      ix, 2
    add     ix, sp

    ld      l, (ix+2)       ; l=posy
    ld      e, (ix+0)       ; e=posx

PUBLIC asm_getrenderaddr
asm_getrenderaddr:

    ; HL = posy * 22
    ld      h, 0
    call    _HL_MULT22

    ; HL += posx
    ld      d, 0
    add     hl, de

    ; HL += render_buf
    ld      de, _render_buf
    add     hl, de

    ret
#endasm
}

void rock_render_line(int phys_y, int logical_row)
{
/*
    int dx;
    unsigned char bit = 0x80;
    int rockx = 0;
    unsigned char *ptnp;

    int rocky = (map_scroll_y + logical_row * 2 + 1) / 4;
    int ptn   = (map_scroll_y + logical_row * 2) & 0x03;   // 0 or 2

    if (map_scroll_y & 0x01)
    {
        memset(&render_buf_odd[phys_y][1], 0, RENDER_DX - 2);

        for (dx = 1; dx <= RENDER_DX - 2; dx++)
        {
//            if (rock_tbl[rocky][rockx] & bit)
//                memcpy(&render_buf_odd[phys_y][dx], &rock_chr[ptn][0], 2);
            ptnp = &rock_ptn[rock_tbl[rocky]][0];
            if (ptnp[rockx] & bit)
                memcpy(&render_buf_odd[phys_y][dx], &rock_chr[ptn][0], 2);

            bit >>= 1;
            if (!bit) { bit = 0x80; rockx++; }
        }
    }
    else
    {
        memset(&render_buf_even[phys_y][1], 0, RENDER_DX - 2);

        for (dx = 1; dx <= RENDER_DX - 2; dx++)
        {
//            if (rock_tbl[rocky][rockx] & bit)
//                memcpy(&render_buf_even[phys_y][dx], &rock_chr[ptn][0], 2);
            ptnp = &rock_ptn[rock_tbl[rocky]][0];
            if (ptnp[rockx] & bit)
                memcpy(&render_buf_even[phys_y][dx], &rock_chr[ptn][0], 2);

            bit >>= 1;
            if (!bit) { bit = 0x80; rockx++; }
        }
    }
*/
#asm
    ld      ix, 2
    add     ix, sp

    // map_scroll_y + logical_row * 2
    ld      hl, (_map_scroll_y)

    ld      a, (ix+0)       ; logical_row
    ld      d, 0
    ld      e, a
    add     hl, de  ; *1
    add     hl, de  ; *2

rock_render_line_setrockchr:
    // int ptn   = (map_scroll_y + logical_row * 2) & 0x03;
    // rock_render_line_rock_chr = &rock_chr[ptn][0];
    push    hl

    ld      a, 0x03
    and     a, l
    ld      hl, _rock_chr
    ld      d, 0
    ld      e, a
    add     hl, de  ; *1
    add     hl, de  ; *2
    ld      (_rock_render_line_rock_chr), hl

    pop     hl

rock_render_line_setroctbl:
//    // int rocky = (map_scroll_y + logical_row * 2 + 1) / 4;
//    // rock_render_line_rock_tbl = &rock_tbl[rocky][0];
//    inc     hl      ; +1
//    srl     h
//    rr      l       ; ÷ 2
//    srl     h
//    rr      l       ; ÷ 4
//    ld      de, _rock_tbl
//    ex      de, hl
//    add     hl, de
//    add     hl, de
//    add     hl, de
//    ld      (_rock_render_line_rock_tbl), hl

    // int rocky = (map_scroll_y + logical_row * 2 + 1) / 4;
    // rock_render_line_rock_tbl = &rock_ptn[rock_tbl[rocky]][0];
    inc     hl      ; +1
    srl     h
    rr      l       ; ÷ 2
    srl     h
    rr      l       ; ÷ 4
//    ld      de, _rock_tbl
    ld      de, (_p_rock_tbl)
    ex      de, hl
    add     hl, de
    ld      a, (hl)

    ld      h, 0
    ld      l, a
//    ld      de, _rock_ptn
    ld      de, (_p_rock_ptn)
    ex      de, hl
    add     hl, de
    add     hl, de
    add     hl, de
    ld      (_rock_render_line_rock_tbl), hl

    // render_buf_odd[phys_y][dx]
    // render_buf_even[phys_y][dx]

    // 奇数判定のため最下位BITチェック
    ld      a, (_map_scroll_y)
    bit     0, a
    jr      z, rock_render_line_setevenaddr

rock_render_line_setoddaddr:
    ld      bc, _render_buf_odd
    jr      rock_render_line_setrenderbuf

rock_render_line_setevenaddr:
    ld      bc, _render_buf_even

rock_render_line_setrenderbuf:
    ld      a, (ix+2)       ; phys_y

    ld      h, 0
    ld      l, a
    call    _HL_MULT22

    add     hl, bc

    // memset(&render_buf_odd[phys_y][1], 0, RENDER_DX - 2)
    inc     hl
    push    hl

//    ld      b,RENDER_DX-2
    ld      a,(_map_min_x)
    ld      b,RENDER_DX-2
    srl     a
rock_render_line_rockclr_1:
    cp      0x01
    jp      z,rock_render_line_rockclr_2
    dec     a
    dec     b
    dec     b
    inc     hl
    jr      rock_render_line_rockclr_1
rock_render_line_rockclr_2:


rock_render_line_rockclr:
    ld      (hl), 0
    inc     hl
    djnz    rock_render_line_rockclr

    pop     hl

rock_render_line_rockdraw:
    // for (dx = 1; dx <= RENDER_DX - 2; dx++)
    ld      b, RENDER_DX-2
    ld      c, 0x80

    // if (rock_tbl[rocky][rockx] & bit)
    ld      de, (_rock_render_line_rock_tbl)

rock_render_line_rockloop:
    ld      a, (de)
    and     a, c
    jp      z, rock_render_line_nextrock

    push    de

     // memcpy(&render_buf_odd[phys_y][dx], &rock_chr[ptn][0], 2);
    ld      de, (_rock_render_line_rock_chr)
    ld      a, (de)
    ld      (hl), a
    inc     de
    inc     hl
    ld      a, (de)
    ld      (hl), a
    dec     hl

    pop     de

rock_render_line_nextrock:
    // bit >>= 1;
    srl     c
    // if (!bit) { bit = 0x80; rockx++; }
    jr      nz, rock_render_line_chkloop

rock_render_line_nexttbl:
    ld      c, 0x80
    inc     de

rock_render_line_chkloop:
    inc     hl
    djnz    rock_render_line_rockloop

    ret

._rock_render_line_rock_chr     defw    0
._rock_render_line_rock_tbl     defw    0

#endasm
}

// ----------------------------------------------------------
// 初期化
// ----------------------------------------------------------
void map_init()
{
    int logical_row;
    int phys_y;

    map_scroll_y = 1000-48;
    start_y   = 0;

    memset(render_buf_even, 0x80, sizeof(render_buf_even));
    memset(render_buf_odd,  0x80, sizeof(render_buf_odd));

    // 左右の壁
    int  map_sx = (map_min_x>>1) -1;
    int  map_ex = RENDER_DX - map_sx -1;
    int  map_spc = map_ex - map_sx - 1;

    for (phys_y = 0; phys_y < RENDER_DY; phys_y++) {
        render_buf_even[phys_y][map_sx] = 0x82;
        render_buf_even[phys_y][map_ex] = 0x81;
        memset(&render_buf_even[phys_y][map_sx+1], 0x00, map_spc);

        render_buf_odd[phys_y][map_sx] = 0x07;
        render_buf_odd[phys_y][map_ex] = 0x84;
        memset(&render_buf_odd[phys_y][map_sx+1], 0x00, map_spc);
    }
}

// ----------------------------------------------------------
// スクロール
//   dy < 0 : 上へ 1dot
//   dy > 0 : 下へ 1dot
//
// ルール：
//   map_scroll_y++ の結果が奇数なら start_y++
//   map_scroll_y-- の結果が偶数なら start_y--
// ----------------------------------------------------------
int map_scroll(int dy)
{
    int phys_y;
    int logical_row;

    if (dy < 0)
    {
        if (map_scroll_y == 0)
        {
            return 0;
        }

        // 上へ 1dot
        map_scroll_y--;

        // 結果が偶数なら start_y を -1
        if ((map_scroll_y & 1) == 0)
            start_y = (start_y - 1 + RENDER_DY) % RENDER_DY;

        logical_row = 0;          // 画面の最上段が新規
        phys_y  = start_y;    // 論理 0 行目の物理行

        rock_render_line(phys_y, logical_row);
    }
    else if (dy > 0)
    {
        if (map_scroll_y == 1000-48)
        {
            return 0;
        }

        // 下へ 1dot
        map_scroll_y++;

        // 結果が奇数なら start_y を +1
        if (map_scroll_y & 1)
            start_y = (start_y + 1) % RENDER_DY;

        logical_row = RENDER_DY - 1;                    // 画面の最下段が新規
        phys_y      = (start_y + logical_row) % RENDER_DY;

        rock_render_line(phys_y, logical_row);
    }

    return (dy);
}

void map_render(void)
{
/*
    int logical_row;
    int src_y;
    unsigned char *curr_bufp, *top_bufp, *back_bufp;

    back_bufp = &render_buf[0][0];
    src_y = start_y;
    if (map_scroll_y & 1)
    {
        curr_bufp = &render_buf_odd[src_y][0];
        top_bufp  = &render_buf_odd[0][0];
    }
    else
    {
        curr_bufp = &render_buf_even[src_y][0];
        top_bufp  = &render_buf_even[0][0];
    }

    for (logical_row = 0; logical_row < RENDER_DY; logical_row++)
    {
        memcpy(back_bufp, curr_bufp, RENDER_DX);
        back_bufp += RENDER_DX;
        curr_bufp += RENDER_DX;
        src_y++;

        if (src_y >= RENDER_DY)
        {
            src_y     = 0;
            curr_bufp = top_bufp;
        }
    }
*/
#asm
    ld      hl, _render_buf
    ld      (_map_render_renderaddr), hl

    // 奇数判定のため最下位BITチェック
    ld      a, (_map_scroll_y)
    bit     0, a
    jr      z, map_render_setevenaddr

map_render_setoddaddr:
    ld      hl, _render_buf_odd
    jr      map_render_settopbuf

map_render_setevenaddr:
    ld      hl, _render_buf_even

map_render_settopbuf:
    ld      (_map_render_topbuf), hl

map_render_setcurrbuf:
    ; HL = start_y * RENDER_DX
    ld      hl, (_start_y)
    call    _HL_MULT22

    ld      de, (_map_render_topbuf)
    add     hl, de

map_render_buf2scrn:
    ld      b, RENDER_DY
    ld      de, (_map_render_renderaddr)
    ld      a, (_start_y)

map_render_linecopy:
    push    bc
    ld      bc, RENDER_DX
    ldir
    pop     bc

    inc     a
    cp      a, RENDER_DY
    jr      nz, map_render_nextline

    ld      a, 0
    ld      hl, (_map_render_topbuf)

map_render_nextline:
    djnz    map_render_linecopy

    ret

._map_render_renderaddr defw    0
._map_render_topbuf     defw    0

#endasm
}

// ----------------------------------------------------------
// VRAM へ描画
//   start_y を先頭として、論理 0..RENDER_DY-1 を順に出力
// ----------------------------------------------------------
void map_draw(void)
{
/*
    int logical_row;
    unsigned char *back_bufp;
    unsigned char *pos = getscrnaddr(0, 4);

    back_bufp = &render_buf[0][0];

    for (logical_row = 0; logical_row < RENDER_DY; logical_row++)
    {
        memcpy(pos, back_bufp, RENDER_DX);
        pos += D_DX;
        back_bufp += RENDER_DX;
    }
*/
    // 描画開始位置をHLレジスタに取得
    getscrnaddr(0, 4);

#asm
    ld      d, h
    ld      e, l

    ld      hl, _render_buf

    ld      b, RENDER_DY

map_draw_linecopy:
    push    bc
    ld      bc, RENDER_DX
    ldir
    pop     bc

    push    hl
    ld      hl, D_DX-RENDER_DX
    add     hl,de
    ex      de,hl
    pop     hl

    djnz    map_draw_linecopy

    ret

#endasm
}

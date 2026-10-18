/*
 *      ZX81 Objects Handling Module program
 */

#include <stdio.h>
#include <string.h>

#include "map.h"
#include "mapdata.h"
#include "boat.h"
#include "objects.h"
#include "scrn.h"

#define WHIRL_CW    5
#define WHIRL_CH    5
#define WHIRL_SIZE  (WHIRL_CW * WHIRL_CH)
unsigned char whirl_chr[2][WHIRL_SIZE] = {
 {0x87,0x03,0x01,0x87,0x00, 0x05,0x00,0x03,0x04,0x04, 0x05,0x05,0x00,0x01,0x05, 0x87,0x02,0x01,0x87,0x01, 0x00,0x00,0x03,0x01,0x00},
 {0x00,0x00,0x83,0x04,0x00, 0x02,0x87,0x04,0x02,0x04, 0x04,0x05,0x00,0x04,0x05, 0x05,0x00,0x83,0x01,0x00, 0x02,0x83,0x00,0x02,0x00}
};

#define ENEMY_CW    3
#define ENEMY_CH    5
#define ENEMY_SIZE  (ENEMY_CW * ENEMY_CH)
unsigned char enemy_chr[4][ENEMY_SIZE] = {
 {0x80,0x81,0x05, 0x05,0x00,0x05, 0x80,0x84,0x05, 0x02,0x07,0x00, 0x00,0x00,0x00},
 {0x85,0x82,0x80, 0x85,0x00,0x85, 0x85,0x07,0x80, 0x00,0x84,0x01, 0x00,0x00,0x00},
 {0x83,0x87,0x04, 0x07,0x03,0x05, 0x82,0x83,0x05, 0x84,0x81,0x01, 0x00,0x01,0x00},
 {0x87,0x04,0x83, 0x85,0x03,0x84, 0x85,0x83,0x81, 0x02,0x82,0x07, 0x00,0x02,0x00}
};

#define CROC_CW     3
#define CROC_CH     5
#define CROC_SIZE   (CROC_CW * CROC_CH)
unsigned char croc_chr[4][CROC_SIZE] = {
 {0x84,0x04,0x00, 0x84,0x80,0x04, 0x81,0x80,0x00, 0x85,0x07,0x01, 0x00,0x01,0x00},
 {0x02,0x82,0x00, 0x02,0x80,0x82, 0x87,0x80,0x05, 0x00,0x80,0x03, 0x00,0x02,0x00},
 {0x00,0x87,0x04, 0x87,0x80,0x04, 0x84,0x80,0x00, 0x81,0x80,0x01, 0x00,0x07,0x00},
 {0x00,0x00,0x83, 0x00,0x81,0x82, 0x02,0x80,0x05, 0x87,0x80,0x07, 0x00,0x85,0x01}
};

#define BARREL_CW   3
#define BARREL_CH   4
#define BARREL_SIZE (BARREL_CW * BARREL_CH)
unsigned char barrel_chr[4][BARREL_SIZE] = {
 {0x07,0x84,0x00, 0x07,0x84,0x00, 0x07,0x84,0x00, 0x03,0x03,0x00},
 {0x85,0x03,0x05, 0x85,0x03,0x05, 0x85,0x03,0x05, 0x02,0x03,0x01},
 {0x83,0x83,0x00, 0x82,0x81,0x00, 0x82,0x81,0x00, 0x82,0x81,0x00},
 {0x87,0x83,0x04, 0x85,0x83,0x05, 0x85,0x83,0x05, 0x85,0x83,0x05}
};

#define RAFT_CW     3
#define RAFT_CH     4
#define RAFT_SIZE   (RAFT_CW * RAFT_CH)
unsigned char raft_chr[4][RAFT_SIZE] = {
{0x06,0x86,0x00, 0x05,0x85,0x00, 0x07,0x84,0x00, 0x02,0x01,0x00},
{0x87,0x03,0x04, 0x85,0x00,0x05, 0x85,0x03,0x05, 0x00,0x03,0x00},
{0x87,0x04,0x00, 0x05,0x85,0x00, 0x82,0x81,0x00, 0x86,0x06,0x00},
{0x00,0x83,0x00, 0x85,0x00,0x05, 0x85,0x83,0x05, 0x02,0x83,0x01},
};

#define DRIFTER_CW  3
#define DRIFTER_CH  3
#define DRIFTER_SIZE  (DRIFTER_CW * DRIFTER_CH)
unsigned char drifter_chr[5][DRIFTER_SIZE] = {
 {0x04,0x04,0x04, 0x84,0x80,0x01, 0x00,0x00,0x00},
 {0x87,0x87,0x87, 0x02,0x80,0x07, 0x00,0x00,0x00},
 {0x04,0x00,0x04, 0x86,0x82,0x01, 0x02,0x03,0x00},
 {0x87,0x00,0x87, 0x02,0x81,0x06, 0x00,0x03,0x01}
};

unsigned char skull_chr[DRIFTER_SIZE] = {
  0x06,0x07,0x04, 0x84,0x84,0x01, 0x00,0x00,0x00
};


// active table
struct ACTIVE_TBL active_tbl[ACTIVE_MAX];

char rescue_score = 0;


//static char map_min_x = 0;
//static char map_max_x = 0;

static unsigned char randum_val;

unsigned char randum()
{
    #asm
        ld      a, (_randum_val)

	    rlca
	    rlca
	    rlca
	    rlca
	    ld	l,a
	    ld	a,r
	    add	a,l
	    ld	(_randum_val),a

	    ld	h,0
	    ld	l,a

	    ret
    #endasm
}

// ----------------------------------------------------------
// 初期化
// ----------------------------------------------------------
void objects_init()
{
    int  i;

    for (i = 0; i < ACTIVE_MAX; i++)
    {
        active_tbl[i].type = OBJ_NONE;
    }

    rescue_score = RESCUE_SCORE_MIN;
}

// ----------------------------------------------------------
// 出現
// ----------------------------------------------------------
static void objects_active(struct SPAWN_TBL *spawnp)
{
/*
    int  i;
    unsigned char *chrp;
    int chrw, chrh, size;
    int speed_step;
    unsigned char type;

    for (i = 0; i < ACTIVE_MAX; i++)
    {
        if (active_tbl[i].type == OBJ_NONE)
        {
            break;
        }
    }
    if (i == ACTIVE_MAX)
    {
        return;
    }

    speed_step = 4;

    type = GET_TYPE(spawnp->attr);
    switch (type)
    {
        case OBJ_WHIRL:
            chrp = &whirl_chr[0][0];
            chrw = WHIRL_CW;
            chrh = WHIRL_CH;
            size = WHIRL_SIZE;
            speed_step = 0;
            break;

        case OBJ_ENEMY:
            chrp = &enemy_chr[0][0];
            chrw = ENEMY_CW;
            chrh = ENEMY_CH;
            size = ENEMY_SIZE;
            break;

        case OBJ_CROC:
            chrp = &croc_chr[0][0];
            chrw = CROC_CW;
            chrh = CROC_CH;
            size = CROC_SIZE;
            speed_step = 2;
            break;

        case OBJ_BARREL:
            chrp = &barrel_chr[0][0];
            chrw = BARREL_CW;
            chrh = BARREL_CH;
            size = BARREL_SIZE;
            break;

        case OBJ_RAFT:
            chrp = &raft_chr[0][0];
            chrw = RAFT_CW;
            chrh = RAFT_CH;
            size = RAFT_SIZE;
            break;

        case OBJ_DRIFTER:
            chrp = &drifter_chr[0][0];
            chrw = DRIFTER_CW;
            chrh = DRIFTER_CH;
            size = DRIFTER_SIZE;
            break;

        default:
            return;
    }
//    active_tbl[i].type = spawnp->type;
    active_tbl[i].type = type;
    active_tbl[i].y = 1;
//    active_tbl[i].x = spawnp->x*2+2;
    active_tbl[i].x = GET_X(spawnp->attr)*2+2;
    active_tbl[i].w = chrw;
    active_tbl[i].h = chrh;
    active_tbl[i].size = size;
    active_tbl[i].chrp = chrp;

    active_tbl[i].speed_step = speed_step;
    active_tbl[i].speed_cnt = speed_step;

    active_tbl[i].contacted = 0;

    active_tbl[i].move_lr = (randum() % 4 == 0) ? 1 : 0;
    active_tbl[i].dx = (randum() % 3);
    active_tbl[i].dx_count = 0;
*/
#asm
        push hl                     ; hl = spawnp
        pop  ix                     ; ix = hl

;        ld      a, (ix+1)           ; a = x
;        ld      (_objects_active_spawn_x),a

;        ld      a, (ix+2)           ; a = type
;        ld      (_objects_active_spawn_type),a

; --- x の抽出 (下位5ビット: 0001 1111 = 0x1F) ---
        ld      a, (ix+1)           ; a = attr (XXXX XTTT)
        srl     a
        srl     a
        srl     a                   ; a = x (000X XXXX) 
        ld      (_objects_active_spawn_x), a

        ; --- type の抽出 (上位3ビット -> 右へ5回シフト) ---
        ld      a, (ix+1)           ; a = attr (XXXX XTTT)
        and     07h                 ; a = type (下位3ビットのみ抽出)
        ld      (_objects_active_spawn_type), a

; --------------------------------------------------
; active_tbl の空きスロット検索
; --------------------------------------------------
        ld      ix, _active_tbl
        ld      b, ACTIVE_MAX

objects_active_find_loop:
        ld      a, (ix+0)               ; type
        cp      OBJ_NONE
        jr      z, objects_active_found
        ld      de, ACTIVE_TBL_SIZE
        add     ix, de
        djnz    objects_active_find_loop

        ret                             ; 空きがない場合は終了 (return)

objects_active_found:
; --------------------------------------------------
; switch (_objects_active_spawn_type)
; --------------------------------------------------
        ld      a, (_objects_active_spawn_type)
        cp      OBJ_WHIRL
        jr      z, objects_active_type_whirl
        cp      OBJ_ENEMY
        jr      z, objects_active_type_enemy
        cp      OBJ_CROC
        jr      z, objects_active_type_croc
        cp      OBJ_BARREL
        jr      z, objects_active_type_barrel
        cp      OBJ_RAFT
        jr      z, objects_active_type_raft
        cp      OBJ_DRIFTER
        jr      z, objects_active_type_drifter

        ret                             ; default: return

; --------------------------------------------------
; 各オブジェクト初期パラメータセット
;  A = type, HL = chrp, D = chrh (高さ), E = chrw (幅), C = size, B = speed_step
; --------------------------------------------------

objects_active_type_whirl:
        ld      hl, _whirl_chr
        ld      d, #WHIRL_CH
        ld      e, #WHIRL_CW
        ld      c, #WHIRL_SIZE
        ld      b, 0
        jr      objects_active_setup

objects_active_type_enemy:
        ld      hl, _enemy_chr
        ld      d, #ENEMY_CH
        ld      e, #ENEMY_CW
        ld      c, #ENEMY_SIZE
        ld      b, 4
        jr      objects_active_setup

objects_active_type_croc:
        ld      hl, _croc_chr
        ld      d, #CROC_CH
        ld      e, #CROC_CW
        ld      c, #CROC_SIZE
        ld      b, 2
        jr      objects_active_setup

objects_active_type_barrel:
        ld      hl, _barrel_chr
        ld      d, #BARREL_CH
        ld      e, #BARREL_CW
        ld      c, #BARREL_SIZE
        ld      b, 4
        jr      objects_active_setup

objects_active_type_raft:
        ld      hl, _raft_chr
        ld      d, #RAFT_CH
        ld      e, #RAFT_CW
        ld      c, #RAFT_SIZE
        ld      b, 4
        jr      objects_active_setup

objects_active_type_drifter:
        ld      hl, _drifter_chr
        ld      d, #DRIFTER_CH
        ld      e, #DRIFTER_CW
        ld      c, #DRIFTER_SIZE
        ld      b, 4
        jr      objects_active_setup

; --------------------------------------------------
; active_tbl[i] への値書き込み
; --------------------------------------------------
objects_active_setup:
        ld      (ix+0), a               ; offset  0: type
        ld      (ix+1), 1               ; offset  1: y = 1

    ; x = spawn_x * 2 + 2
        ld      a, (_objects_active_spawn_x)
        add     a, a                    ; * 2
        add     a, 2                    ; + 2
        ld      (ix+2), a               ; offset  2: x

        ld      (ix+3), d               ; offset  3: h = chrh
        ld      (ix+4), e               ; offset  4: w = chrw
        ld      (ix+5), c               ; offset  5: size
        ld      (ix+6), l               ; offset  6: chrp (L)
        ld      (ix+7), h               ; offset  7: chrp (H)
        ld      (ix+8), b               ; offset  8: speed_step
        ld      (ix+9), b               ; offset  9: speed_cnt
        ld      (ix+10), 0              ; offset 10: contacted = 0

; --------------------------------------------------
; 乱数による移動初期設定
; --------------------------------------------------
    ; move_lr = (randum() % 4 == 0) ? 1 : 0
        call    _randum                 ; HL = randum()
        and     03h                     ; % 4 相当
        ld      a, 0
        jr      nz, $+3
        inc     a
        ld      (ix+11), a              ; offset 11: move_lr

    ; dx = randum() % 3
objects_active_get_rand3:
        call _randum
        and  3                   ; 0, 1, 2, 3 に制限
        cp   3                   ; 3 かどうかチェック
        jr   z, objects_active_get_rand3
        ld      (ix+12), a              ; offset 12: dx

        ld      (ix+13), 0              ; offset 13: dx_count = 0

        ret

; 作業用変数領域
._objects_active_spawn_x    defb 0
._objects_active_spawn_type defb 0

#endasm
}

// ----------------------------------------------------------
// 出現
// ----------------------------------------------------------
void objects_spawn(int map_scroll_row)
{
/*
    int no;

    // 出現位置の更新によるオブジェクト出現処理
    no = 0;
    while (p_spawn_tbl[no].y != 0) {
        if (p_spawn_tbl[no].y == map_scroll_row) {
            objects_active(&p_spawn_tbl[no]);
        }
        no++;
    }
*/
#asm
        ld      c,l                 ; C = map_scroll_row
        ld      hl, (_p_spawn_tbl)  ; HL = p_spawn_tbl

objects_spawn_loop:
        ld      a,(hl)              ; A = y
        or      a
        ret     z                   ; y == 0 なら終了

        cp      c                   ; y == map_scroll_row ?
        jr      nz, objects_spawn_next

        push    bc
        push    hl

        call    _objects_active

        pop     hl
        pop     bc

objects_spawn_next:
        inc     hl
        inc     hl
        
        jr      objects_spawn_loop

#endasm
}

// ----------------------------------------------------------
// 移動
// ----------------------------------------------------------
void objects_move(int dy)
{
/*
    int act_no, obj_x, obj_y;
    struct ACTIVE_TBL *actp;

    for (act_no = 0; act_no < ACTIVE_MAX; act_no++) {
        actp = &active_tbl[act_no];
        if (actp->type == OBJ_NONE) {
            continue;
        }
        if (actp->speed_step != 0) {
            actp->speed_cnt--;
            if (actp->speed_cnt == 0) {
// 変更前
//                // Y移動
//                actp->y++;
// 変更後
                obj_x = actp->x >> 1;
                obj_y = actp->y >> 1;
                if ((obj_y >= 0) && (obj_y < RENDER_DY)) {
                    unsigned char *pos = getrenderaddr(obj_y, obj_x);
                    if ((pos[0] == 0) && (pos[1] == 0) && (pos[2] == 0)) {
                        // Y移動
                        actp->y++;
                    }
                    else {
                        actp->move_lr = 1;
                    }
                }
                else {
                    // Y移動
                    actp->y++;
                }
// ここまで
                actp->speed_cnt = actp->speed_step;

                // X移動
                if (actp->move_lr) {
                    // 現在の方向に移動
                    actp->x += (actp->dx - 1);

                    // 画面端で直進
                    if (actp->x <= map_min_x) {
                        actp->x = map_min_x;
                        actp->dx = 1;
                    }
                    if (actp->x >= map_max_x) {
                        actp->x = map_max_x;
                        actp->dx = 1;
                    }

                    // カウンタ増加
                    actp->dx_count++;

                    // 3回移動したら方向再決定
                    if (actp->dx_count >= 3) {
                        actp->dx = randum() % 3;   // Left:0, Center:1, Right:2
                        actp->dx_count = 0;
                    }
                }
            }
        }

        actp->y -= dy;

        // 画面外は消去
        obj_y = actp->y >> 1;
        if ((obj_y < 0) ||
            (obj_y >= RENDER_DY + actp->h)) {
            if (actp->type == OBJ_DRIFTER) {
                rescue_score = RESCUE_SCORE_MIN;
            }
            actp->type = OBJ_NONE;
        }
    }
*/
#asm
        ld  a, l                ; a = dy
        ld  (_objects_move_dy), a

        ld  ix, _active_tbl     ; IX = active_tbl の先頭ポインタ
        ld  b, ACTIVE_MAX       ; ループカウンタ

objects_move_loop:

    ; if (actp->type == OBJ_NONE) continue;
        ld  a, (ix+0)           ; a = type
        cp  OBJ_NONE
        jp  z, objects_move_next_object

    ; if (actp->speed_step != 0)
        ld  a, (ix+8)           ; a = speed_step
        or  a
        jr  z, objects_move_skip_speed_check

    ; actp->speed_cnt--;
        dec (ix+9)              ; a = speed_cnt
        jr  nz, objects_move_skip_speed_check

; --- スピードカウント終了時の処理 ---
// 変更前
//    ; actp->y++;
//        inc (ix+1)              ; y++
// 変更後
; --- 下移動チェック ---
        call objects_move_check_down
        or a
        jr nz, objects_move_blocked_down

        ; A=0 → 下は空白 → y++
        inc (ix+1)
        jr objects_move_after_down

objects_move_blocked_down:
        ; A=1 → 下にモノあり → 横移動モードへ
        ld (ix+11),1      ; move_lr = 1
objects_move_after_down:
// ここまで

    ; actp->speed_cnt = actp->speed_step;
        ld  a, (ix+8)           ; a = speed_step
        ld  (ix+9), a           ; apeed_cnt = a

; --- X移動処理 ---
        call objects_move_x

objects_move_skip_speed_check:

; --- Y移動処理 ---
        call objects_move_y

objects_move_next_object:
    ; ポインタを次の構造体へ移動 (IX += ACTIVE_TBL_SIZE)
        ld  de, ACTIVE_TBL_SIZE
        add ix, de

        djnz objects_move_loop

        ret

; --- 下移動チェック ---
objects_move_check_down:
        ; obj_x = actp->x >> 1
        ld  a, (ix+2)       ; a = actp->x
        srl a               ; a >>1
        ld  e, a            ; e = a

        ; obj_y = actp->y >> 1
        ld  a, (ix+1)       ; a = actp->y
        srl a               ; a >>1
        ld  l, a            ; l = a

        ; 画面外なら常に空白扱い
        cp  RENDER_DY
        jr  nc, objects_down_empty

        ; getrenderaddr(obj_y, obj_x)
        call    asm_getrenderaddr   ; l=y, e=x -> HL = pos
        ; pos[0] | pos[1] | pos[2]
        ld  a, (hl)
        inc hl
        or  (hl)
        inc hl
        or  (hl)
        jr  nz, objects_down_blocked

objects_down_empty:
        xor a           ; A=0
        ret

objects_down_blocked:
        ld  a, 1        ; A=1
        ret


; --- X移動処理 ---
objects_move_x:
    ; if (actp->move_lr)
        ld  a, (ix+11)          ; a = move_lr
        or  a
        ret z

   ; actp->x += (actp->dx - 1);
        ld  a, (ix+12)          ; a = dx
        dec a                   ; dx - 1 (-1, 0, 1)
        add a, (ix+2)           ; a += x
        ld  (ix+2), a

objects_move_check_min_x:
    ; if (actp->x <= map_min_x)
        ld  c, a                ; c = 現在の x
        ld  a, (_map_min_x)
        cp  c                   ; map_min_x >= x ?
        jr  c, objects_move_check_max_x

    ; 左端到達処理
        ld  (ix+2), a           ; x = map_min_x
        ld  (ix+12), 1          ; dx = 1
        jr  objects_move_after_lr

objects_move_check_max_x:
    ; if (actp->x >= map_max_x)
        ld  a, (_map_max_x)
        cp  c                   ; map_max_x <= x ?
        jr  nc, objects_move_after_lr

    ; 右端到達処理
        ld  (ix+2), a           ; x = map_max_x
        ld  (ix+12), 1          ; dx = 1

objects_move_after_lr:
    ; actp->dx_count++;
        inc (ix+13)             ; a = dx_count

    ; if (actp->dx_count >= 3)
        ld  a, (ix+13)          ; a = dx_count
        cp  3
        ret c

    ; --- 方向再決定 (dx = randum() % 3) ---
objects_move_get_rand3:
        call _randum
        and  3                   ; 0, 1, 2, 3 に制限
        cp   3                   ; 3 かどうかチェック
        jr   z, objects_move_get_rand3
        ld  (ix+12), a          ; dx = a
        ld  (ix+13), 0          ; dx_count = 0

        ret

; --- Y移動処理 ---
objects_move_y:
    ; actp->y -= dy;
        ld  a, (_objects_move_dy)
        ld  c, a
        ld  a, (ix+1)           ; a = y
        sub c
        ld  (ix+1), a           ; y = a-c

; --- 画面外判定 ---
    ; obj_y = actp->y >> 1;
        srl a                   ; 右シフト1回 (1/2)
        ld  c, a                ; c = obj_y

    ; if (obj_y < 0) -> 符号なしで255付近（画面上部外）判定
    ; かつ obj_y >= RENDER_DY + actp->h
        ld  a, (ix+3)           ; a = h
        add a, RENDER_DY        ; a = RENDER_DY + actp->h
        cp  c                   ; (RENDER_DY + h) <= obj_y ?
        ret nc

    ; --- 画面外消去処理 ---
    ; if (actp->type == OBJ_DRIFTER)
        ld  a, (ix+0)           ; a = type
        cp  OBJ_DRIFTER
        jr  nz, objects_move_no_drifter

    ; rescue_score = RESCUE_SCORE_MIN;
        ld  a, RESCUE_SCORE_MIN
        ld  (_rescue_score), a

objects_move_no_drifter:
    ; actp->type = OBJ_NONE;
        ld  (ix+0), OBJ_NONE    ; type = OBJ_NONE

        ret

; 作業用変数領域
._objects_move_dy       defb 0
#endasm
}

// ----------------------------------------------------------
// 描画
// ----------------------------------------------------------
void objects_render(int layer, int draw_whirl)
{
/*
    int act_no, obj_y, y, obj_x;
    unsigned char *chrp;
    unsigned char *pos;
    int chrw, chrh, dy;

    for (act_no = 0; act_no < ACTIVE_MAX; act_no++)
    {
        if (active_tbl[act_no].type == OBJ_NONE)
        {
            continue;
        }
        if (layer == RENDER_COLLISION)
        {
            if (active_tbl[act_no].type > OBJ_RAFT)
            {
                continue;
            }
        }
        else // layer == RENDER_CONTACT
        {
            if (active_tbl[act_no].type <= OBJ_RAFT)
            {
                continue;
            }
        }

        if ((active_tbl[act_no].type == OBJ_WHIRL) &&
            (draw_whirl == 0))
        {
            continue;
        }

        // DRIFTER 正面接触ならドクロ表示
        if (active_tbl[act_no].type == OBJ_DRIFTER &&
            active_tbl[act_no].contacted == 2)
        {
            chrp = &skull_chr[0];   // ドクロのキャラコード
        }
        else
        {
            chrp = active_tbl[act_no].chrp;
            if (active_tbl[act_no].type == OBJ_WHIRL)
            {
                if (active_tbl[act_no].y & 0x01)
                {
                    chrp += active_tbl[act_no].area;
                }
            }
            else
            {
                if (active_tbl[act_no].x & 0x01)
                {
                    chrp += active_tbl[act_no].area;
                }
                if (active_tbl[act_no].y & 0x01)
                {
                    chrp += active_tbl[act_no].area * 2;
                }
            }
        }

        obj_y = (active_tbl[act_no].y >> 1) - active_tbl[act_no].h;
        obj_x = active_tbl[act_no].x >> 1;
        if (obj_y < 0)
        {
            pos = getrenderaddr(0, obj_x);
        }
        else
        {
            pos = getrenderaddr(obj_y, obj_x);
        }

        chrw = active_tbl[act_no].w;
        chrh = active_tbl[act_no].h;

        dy = obj_y;
        for (y = 0; y < chrh; y++)
        {
            if ((dy <= 23) && (dy >= 0))
            {
                // 衝突物かドクロの場合は船より上に描画
                if ((layer == RENDER_COLLISION) ||
                    (active_tbl[act_no].contacted == 2))
                {
                    memcpy(pos, chrp, chrw);
                }
                else
                {
                    for (int x = 0; x < chrw; x++)
                    {
                        if (pos[x] == 0)
                        {
                            pos[x] = chrp[x];
                        }
                    }
                }
            }
            if (dy >= 0)
            {
                pos += RENDER_DX;
            }
            chrp += chrw;
            dy++;
        }
    }
*/
#asm
        ld      ix, 2
        add     ix, sp

        ld      a, (ix+2)       ; a=layer
        ld      (_objects_render_layer), a

        ld      a, (ix+0)       ; a=draw_whirl
        ld      (_objects_render_draw_whirl), a

    ; IX = &active_tbl[0]
        ld      ix, _active_tbl
        ld      b, ACTIVE_MAX   ; b=counter

objects_render_loop:

    ; OBJ_NONE check
        ld      a, (ix+0)       ; c = type
        ld      c,a
        cp      OBJ_NONE
        jp      z, objects_render_next

    ; layer 判定
        ld      a, (_objects_render_layer)
        cp      RENDER_COLLISION
        jr      nz, objects_render_check_layer_contact

objects_render_check_layer_collision:
    ; RENDER_COLLISION: type > OBJ_RAFT (3) ならスキップ
        ld      a, OBJ_RAFT
        cp      c
        jp      c, objects_render_next
        jr      objects_render_check_whirl

objects_render_check_layer_contact:
    ; RENDER_CONTACT: type <= OBJ_RAFT (3) ならスキップ
        ld      a, OBJ_RAFT
        cp      c
        jp      nc, objects_render_next

objects_render_check_whirl:
    ; OBJ_WHIRL (4) かつ draw_whirl == 0 ならスキップ
        ld      a, c
        cp      OBJ_WHIRL
        jr      nz, objects_render_check_done
        ld      a, (_objects_render_draw_whirl)
        or      a
        jp      z, objects_render_next

objects_render_check_done:
    ; chrpを計算
        call    objects_render_calc_chrp

    ; chrpを描画
        push    bc
        call    objects_render_draw_chrp
        pop     bc

objects_render_next:
        ld      de, ACTIVE_TBL_SIZE
        add     ix, de

        djnz    objects_render_loop

        ret

; --------------------------------------------------
; パターンポインタ (chrp) の計算 -> HL に保持
;  IN: ix = &active_tbl[no];
;  OUT:
;      hl = chrp
;  破壊レジスタ:
;      a, de
; --------------------------------------------------
objects_render_calc_chrp:
    ; DRIFTER (6) && contacted (オフセット+10) == 2 の判定
        ld      a, (ix+0)           ; a = type
        cp      OBJ_DRIFTER
        jr      nz, objects_render_calc_normal_chrp
    
        ld      a, (ix+10)          ; a=contacted
        cp      2
        jr      nz, objects_render_calc_normal_chrp

    ; skull_chr を使用
        ld      hl, _skull_chr      ; hl = &skullchr[0];
        ret

objects_render_calc_normal_chrp:
        ld      e, (ix+5)           ; de = area
        ld      d, 0
        ld      l, (ix+6)           ; hl = chrp
        ld      h, (ix+7)

    ; OBJ_WHIRL check
        ld      a, (ix+0)           ; a = type
        cp      OBJ_WHIRL
        jr      z, objects_render_whirl_anim

    ; 通常オブジェクト: (x & 1) -> +area, (y & 1) -> +area*2
        ld      a, (ix+2)           ; a = x
        rra                         ; bit 0 -> Carry
        jr      nc, objects_render_no_x_anim
        add     hl, de

objects_render_no_x_anim:
        ld      a, (ix+1)           ; a = y
        rra                         ; bit 0 -> Carry
        ret     nc
        add     hl, de
        add     hl, de
        ret

objects_render_whirl_anim:
    ; 渦: (y & 1) -> +area
        ld      a, (ix+1)           ; a = y
        rra                         ; bit 0 -> Carry
        ret     nc
        add     hl, de

        ret

; --------------------------------------------------
; パターンポインタ (chrp) の描画
;   IN: hl = chrp
;       ix = &active_tbl[no];
;   破壊レジスタ:
;       a, bc, de, hl
; --------------------------------------------------
objects_render_draw_chrp:
        push    hl              ; chrp をスタック退避

    ; obj_y = (y >> 1) - h
        ld      a, (ix+1)       ; a = y
        ld      l, (ix+3)       ; l = h
        srl     a
        sub     l
        ld      (_objects_render_obj_y), a

    ; getrenderaddr(obj_y < 0 ? 0 : obj_y, obj_x) の呼び出し準備
        ld      l, a            ; l = obj_y
        bit     7, a            ; dy < 0 判定
        jr      z, objects_render_pos_y_ok
        ld      l, 0            ; 負数の場合は 0 にクリップ

objects_render_pos_y_ok:
    ; obj_x = x >> 1
        ld      a, (ix+2)       ; a= x
        srl     a
        ld      e, a            ; e = obj_x

        call    asm_getrenderaddr

        ex      de, hl          ; de = renderaddr
        pop     hl              ; hl = chrp 復元

    ; --------------------------------------------------
    ; 描画モード判定 (1: 上書き/memcpy, 0: 透過判定合成)
    ; --------------------------------------------------
    ; (layer == RENDER_COLLISION) || (contacted == 2)
        ld      a, (_objects_render_layer) ; 保存しておいた layer を参照
        cp      RENDER_COLLISION
        jr      z, objects_render_mode_overwrite

        ld      a, (ix+10)      ; contacted
        cp      2
        jr      z, objects_render_mode_overwrite

        ld      a, 0            ; モード0: 透過判定付き (pos[x]==0 の時のみ描画)
        jr      objects_render_start_draw

objects_render_mode_overwrite:
        ld      a, 1            ; モード1: 上書き (memcpy相当)

objects_render_start_draw:
        ld      (_objects_render_mode), a

        ld      a, (ix+3)       ; a = h (Y loop カウンタ)
        ld      (_objects_render_cnt_h), a

; --------------------------------------------------
; Y方向 ループ
; --------------------------------------------------
objects_render_y_loop:
        ld      a, (_objects_render_obj_y) ; a = dy

    ; dy <= 23 && dy >= 0 の判定
        cp      24              ; dy >= 24 (または dy < 0 の補数表現) かチェック
        jr      nc, objects_render_skip_line_draw ; 範囲外なら描画スキップ

    ; --- 1行分の描画処理 ---
        push    hl              ; chrp 保存
        push    de              ; pos 保存
        ld      b, (ix+4)       ; b = w (X loop カウンタ)

        ld      a, (_objects_render_mode)
        or      a
        jr      nz, objects_render_line_memcpy

; --- モード0: 透過判定描画 (pos[x] == 0 の場合のみ pos[x] = chrp[x]) ---
objects_render_line_trans:
        ld      a, (de)         ; A = *pos
        or      a               ; pos[x] == 0 ?
        jr      nz, objects_render_trans_next
        ld      a, (hl)         ; A = *chrp
        ld      (de), a         ; *pos = *chrp
objects_render_trans_next:
        inc     hl
        inc     de
        djnz    objects_render_line_trans
        jr      objects_render_line_done

; --- モード1: 高速上書き (memcpy) ---
objects_render_line_memcpy:
        ld      a, (hl)
        or      a
        jr      z, object_render_skip_copy
        ld      (de), a
object_render_skip_copy:
        inc     hl
        inc     de
        djnz    objects_render_line_memcpy

objects_render_line_done:
        pop     de              ; pos 復元
        pop     hl              ; chrp 復元

objects_render_skip_line_draw:
    ; dy >= 0 なら pos += RENDER_DX
        ld      a, (_objects_render_obj_y)
        bit     7, a            ; dy < 0 ?
        jr      nz, objects_render_skip_pos_add

        push    hl
        ld      hl, RENDER_DX
        add     hl, de
        ex      de, hl          ; de += RENDER_DX
        pop     hl

objects_render_skip_pos_add:
    ; chrp += chrw
        push    bc
        ld      c, (ix+4)       ; c = chrw
        ld      b, 0
        add     hl, bc          ; hl += chrw
        pop     bc

    ; dy++
        ld      a, (_objects_render_obj_y)
        inc     a
        ld      (_objects_render_obj_y), a

    ; Yループカウンタ更新
        ld      a, (_objects_render_cnt_h)
        dec     a
        ld      (_objects_render_cnt_h), a
        jr      nz, objects_render_y_loop

        ret

; 作業用変数領域
._objects_render_layer  defb 0
._objects_render_draw_whirl  defb 0
._objects_render_obj_y  defb 0
._objects_render_mode   defb 0
._objects_render_cnt_h  defb 0

#endasm
}

// ----------------------------------------------------------
// 接触
// ----------------------------------------------------------
int  objects_contact(void)
{
/*
    int  act_no;

    for (act_no = 0; act_no < ACTIVE_MAX; act_no++)
    {
        if (active_tbl[act_no].type > OBJ_RAFT)
        {
            if (active_tbl[act_no].contacted == 0)
            {
                active_tbl[act_no].contacted = boat_contact(&active_tbl[act_no]);
                if (active_tbl[act_no].contacted != 0)
                {
                    return (act_no);
                }
            }
        }
    }
    return (act_no);
*/
#asm
        ld   ix, _active_tbl    ; ix = active_tbl の先頭
        ld   a, 0               ; a = act_no
        ld   (_objects_contact_act_no), a

objects_contact_loop:
    ; type > OBJ_RAFT ?
        ld   a,(ix+0)           ; a = type
        cp   OBJ_RAFT+1         ; OBJ_RAFT より大きい？
        jr   c, objects_contact_next

    ; contacted == 0 ?
        ld   a,(ix+10)          ; a = contacted
        or   a
        jr   nz, objects_contact_next

    ; boat_contact(&active_tbl[act_no])
        call asm_boat_contact   ; IX = &active_tbl[act_no]

    ; contacted = 戻り値
        ld   a,l
        ld   (ix+10),a

    ; 戻り値が 0 でなければ接触
        cp   0
        jr   z, objects_contact_next

    ; act_no を返す
        ld   a,(_objects_contact_act_no)
        ld   l,a
        ld   h,0
        ret

objects_contact_next:
    ; IX += ACTIVE_TBL_SIZE
        ld   de, ACTIVE_TBL_SIZE
        add  ix,de

        ld   a, (_objects_contact_act_no)   ; a = act_no + 1
        inc  a
        ld   (_objects_contact_act_no), a
        cp   ACTIVE_MAX
        jr   nz, objects_contact_loop

    ; 見つからなかった → act_no (=ACTIVE_MAX)
        ld   l,a
        ld   h,0

        ret

; 作業用変数領域
._objects_contact_act_no    defb 0
#endasm
}
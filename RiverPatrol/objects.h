/*
 *      ZX81 Objects Handling Module header
 */

#ifndef _ZX81_OBJECTS_H
#define _ZX81_OBJECTS_H


// Object type
#define OBJ_NONE    0   // なし
#define OBJ_ENEMY   1   // 敵船
#define OBJ_BARREL  2   // ドラム缶
#define OBJ_RAFT    3   // 流木
#define OBJ_CROC    4   // ワニ
#define OBJ_DRIFTER 5   // 漂流者
#define OBJ_WHIRL   6   // 渦

// render type
#define RENDER_COLLISION    0   // 衝突対象（hit の対象）
#define RENDER_CONTACT      1   // 接触対象（contact の対象）

// active table
#define ACTIVE_MAX      5
#define ACTIVE_TBL_SIZE 14     // sizeof(struct ACTIVE_TBL)

struct ACTIVE_TBL {
    char type;               // 0:オブジェクト種別
    char y;                  // 1:画面上のY座標
    char x;                  // 2:画面上のX座標
    char h;                  // 3:描画文字の高さ
    char w;                  // 4:描画文字の幅
    char size;               // 5:描画文字のサイズ
    unsigned char *chrp;     // 6-7:描画文字のポインタ
    char speed_step;         // 8:Y移動速度（0=移動しない、1=毎回, 2=2回に1回）
    char speed_cnt;          // 9:カウンタ（speed_y で初期化）
    char contacted;          // 10:1度だけ接触判定するためのフラグ
    char move_lr;            // 11:左右移動するオブジェクトか？（1/4の確率で1）
    char dx;                 // 12:左右移動方向（Left:0, Center:1, Right:2）
    char dx_count;           // 13:方向維持カウンタ（3回移動したら再決定）
};
extern struct ACTIVE_TBL active_tbl[ACTIVE_MAX];

// rescue score
#define RESCUE_SCORE_MIN    10
#define RESCUE_SCORE_MAX    100
#define RESCUE_SCORE_STEP   10
                            
extern char rescue_score;

// randum
unsigned char randum();

// objects functions
void objects_init();
void objects_spawn(int map_scroll_row);
void objects_move(int dy);
void objects_render(int layer,  int draw_whirl);
int  objects_contact(void);

#endif  // _ZX81_OBJECTS_H


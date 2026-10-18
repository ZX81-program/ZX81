#include <stdio.h>
#include <string.h>
#include <input.h>      // in_Wait

#include "key.h"
#include "map.h"
#include "mapdata.h"
#include "boat.h"
#include "objects.h"
#include "scrn.h"

typedef enum {
    STATE_TITLE,
    STATE_GAME_INIT,
    STATE_GAME_LOOP,
    STATE_SINKING,
    STATE_STAGE_CLEAR
} GameState;

typedef enum {
    MODE_BON_VOYAGE,    // 出航モード
    MODE_NORMAL,        // 通常操作モード
    MODE_WHIRL_ALIGN,   // 渦整列ステップ
    MODE_WHIRL_PULL,    // 渦吸い込みステップ
    MODE_WHIRL_SPIN     // 渦回転ステップ
} ShipMode;

static ShipMode shipmode;

// キー入力
static unsigned char key;

// 渦回転
static int  whirl_target_x;
static int  whirl_target_y;
static int  whirl_dy;
static int  whirl_dx;
static unsigned char space_key_cnt;
static int  boat_spin_frame;

// 水量
static int water_leak_level;

#define FRAMES          0x4034

void wait_vsync(char frame)
{
/*
    for (int i = 0; i < frame; i++)
    {
        unsigned char f = *(unsigned char*)FRAMES;
        while (*(unsigned char*)FRAMES == f) ;
    }
*/
#asm
    ld      a,l
    ld      hl,FRAMES

wait_vsync_0wait_check:
    or      a
    ret     z

    ld      b,a
wait_vsync_next:
    ld      a,(hl)

wait_vsync_loop:
    cp      a,(hl)
    jp      z,wait_vsync_loop

    djnz    wait_vsync_next

    ret
#endasm
}

static int game_title()
{
    static unsigned char logostr[][20] ={
        {0x00,0x00,0x85,0x07,0x84,0x04,0x85,0x05,0x80,0x00,0x80,0x85,0x07,0x03,0x85,0x07,0x84,0x04,0x00,0x00},
        {0x00,0x00,0x85,0x05,0x85,0x05,0x85,0x05,0x80,0x00,0x80,0x85,0x82,0x83,0x85,0x05,0x85,0x05,0x00,0x00},
        {0x00,0x00,0x85,0x07,0x86,0x04,0x85,0x05,0x80,0x00,0x80,0x85,0x05,0x00,0x85,0x07,0x86,0x04,0x00,0x00},
        {0x00,0x00,0x85,0x05,0x85,0x82,0x85,0x05,0x02,0x80,0x01,0x85,0x82,0x83,0x85,0x05,0x85,0x82,0x00,0x00},
        {0x83,0x83,0x04,0x00,0x83,0x83,0x00,0x83,0x83,0x83,0x87,0x83,0x83,0x00,0x00,0x83,0x83,0x00,0x83,0x00},
        {0x80,0x00,0x80,0x85,0x05,0x85,0x05,0x00,0x80,0x00,0x85,0x05,0x85,0x05,0x85,0x05,0x85,0x05,0x80,0x00},
        {0x80,0x83,0x03,0x85,0x82,0x81,0x05,0x00,0x80,0x00,0x85,0x82,0x06,0x01,0x85,0x05,0x85,0x05,0x80,0x00},
		{0x80,0x00,0x00,0x85,0x05,0x85,0x05,0x00,0x80,0x00,0x85,0x05,0x85,0x05,0x85,0x05,0x85,0x05,0x80,0x00},
		{0x03,0x00,0x00,0x02,0x01,0x02,0x01,0x00,0x03,0x00,0x02,0x01,0x02,0x03,0x00,0x03,0x03,0x00,0x03,0x03}
    };
    static char *keyinfostr = "<>:MOVE,SPACE:BOOST";
    static char *hitinfostr[2] = {
        {"HIT SPACE KEY"},
        {"HIT       KEY"}
    };
    int  n;

    scrn_init();
    goal_meter_init();
    water_gauge_init();

    // draw logo
    for (n = 0; n < sizeof(logostr) / sizeof(logostr[0]); n++) {
        memcpy(getscrnaddr(3+n, 5), &logostr[n][0], sizeof(logostr[0]));
    }

    // key information
   zx81_write_str(getscrnaddr(15, 5), keyinfostr, 1);

    n = 0;
    while (1) {
        key = key_pressed();
        if (key & (KBIT_ENT|KBIT_SPC)) {
            break;
        }
        if (n == 0 || n == 10) {
            zx81_write_str(getscrnaddr(18, 9), hitinfostr[n / 10], 1);
        }

        game_speed_set(key);
        randum();
        wait_vsync(game_speed << 1);
        n++;
        if (n == 20) {
            n = 0;
        }
    }

    clear_game_screen();

    return (STATE_GAME_INIT);
}

static void proc_object_contact()
{
    int  i;

    int act_no = objects_contact();
    if (act_no < ACTIVE_MAX) {
        switch (active_tbl[act_no].type) {
        case OBJ_WHIRL:
            // 渦処理
            shipmode = MODE_WHIRL_ALIGN;
            whirl_target_x = active_tbl[act_no].x;
            whirl_target_y = active_tbl[act_no].y - (active_tbl[act_no].h*2);
            break;

        case OBJ_CROC:
            // ワニ処理
            for (int i = 0; i < 5; i++) {
                zx_fast();
                zx_slow();
                wait_vsync(5);
            }
            water_leak_level++;
            break;

        case OBJ_DRIFTER:   // 漂流者処理
            if (active_tbl[act_no].contacted == 1) {
                // 得点表示
                int  y = active_tbl[act_no].y - (active_tbl[act_no].h * 2) - 1;
                int  x = active_tbl[act_no].x;
                zx81_write_num(getscrnaddr((y >> 1) + 1, (x >> 1) + 3), rescue_score * 10, 4, 0);
                score += rescue_score;
                score_draw();
                rescue_score += RESCUE_SCORE_STEP;
                if (rescue_score > RESCUE_SCORE_MAX) {
                    rescue_score = RESCUE_SCORE_MAX;
                }
            }
            else {
                // 再描画でドクロ表示
                objects_render(RENDER_CONTACT, 1);
                // 全体描画
                map_draw();
                rescue_score = RESCUE_SCORE_MIN;
            }
            active_tbl[act_no].type = OBJ_NONE;
            wait_vsync(30);
            break;

        default:
            break;
        }
    }
}

// 自船の状態更新（ステートマシン）
static int ship_update(unsigned char key)
{
    int dx = 0, dy = 0;

    switch (shipmode) {

        case MODE_BON_VOYAGE:   // 出航モード
            key = boat_bon_voyage();
            if (key != 0) {
                return boat_move(key);
            }
            shipmode = MODE_NORMAL;
            // fall-through

        case MODE_NORMAL:       // 通常操作モード
            dy = boat_move(key);
            water_gauge_draw(water_leak_level);
            break;

        case MODE_WHIRL_ALIGN:  // 渦整列ステップ
            tilt_level = STEER_CENTER;
            boat_tilt = TILT_CENTER;

            if (boat_y & 0x01) {
                dy = boat_move(KBIT_SPC);
                break;
            }

            shipmode = MODE_WHIRL_PULL;
            whirl_dy = whirl_target_y - boat_y;
            whirl_dx = whirl_target_x - boat_x;
            break;

        case MODE_WHIRL_PULL:   // 渦吸い込みステップ
            dx = (whirl_dx > 0) - (whirl_dx < 0);
            dy = (whirl_dy > 0) - (whirl_dy < 0);

            if (dx != 0 || dy != 0) {
                if (whirl_dy != 0) whirl_dy -= dy;
                if (whirl_dx != 0) {
                    whirl_dx -= dx;
                    boat_x += dx;
                }
            } else {
                shipmode = MODE_WHIRL_SPIN;
                boat_spin_frame = 0;
                space_key_cnt = 0;
            }
            break;

        case MODE_WHIRL_SPIN:   // 渦回転ステップ
            if (key & KBIT_SPC) {
                if (space_key_cnt < 0x0f) space_key_cnt++;
            } else {
                space_key_cnt = 0;
            }

            boat_spin_frame++;
            if (boat_spin_frame == 16) {
                boat_spin_frame = 0;
                if ((space_key_cnt > 0) && (space_key_cnt < 4)) {
                    shipmode = MODE_NORMAL;
                }
            }

            water_gauge_draw(6 + water_leak_level);
            break;
    }

    return dy;
}

static int game_loop()
{
    int  dy;
    int  collision;
    unsigned int  map_scroll_row;
    unsigned int  map_last_row;

    // 初期化
    scrn_init();

    set_mapdata(round);
    map_init();
    objects_init();
    boat_init();
    goal_meter_init();
    water_gauge_init();

    map_last_row = 0xffff;
    water_leak_level = 2;
    shipmode = MODE_BON_VOYAGE;

    while (1) {
        // キー入力
        key = key_pressed();
        if (key & KBIT_ENT) {
            break;
        }

        // game speed
        game_speed_set(key);

        // 自船の状態更新
        dy = ship_update(key);

        // MAP、メーター更新
        dy = map_scroll(dy);
        map_render();

        // オブジェクト移動
        objects_move(dy);

        // MAP更新時の処理
        if (dy != 0) {
            goal_meter_draw(dy);

            map_scroll_row = map_scroll_y >> 2;
            if (map_last_row > map_scroll_row) {
                map_last_row = map_scroll_row;

                // オブジェクト出現
                objects_spawn(map_last_row);

                // スコア加算
                if (shipmode != MODE_BON_VOYAGE)
                {
                    score += 1;
                    score_draw();
                }
            }
        }
        // 各種描画
        objects_render(RENDER_COLLISION, 1);

        // ボート描画と衝突判定
        if (shipmode == MODE_WHIRL_SPIN)
            collision = boat_render_spin(boat_spin_frame);
        else
            collision = boat_render_normal();

        // 衝突以外の描画
        objects_render(RENDER_CONTACT, !((boat_spin_frame >> 2) & 0x01));

        if (shipmode == MODE_BON_VOYAGE) {
            if (map_scroll_row & 0x01) {
                zx81_write_str(getrenderaddr(6,6), "BON VOYAGE", 1);
            }
        }

        // 全体描画                                   
        map_draw();

        // 接触処理
        proc_object_contact();

        // クリア判定
        if (map_scroll_y == 0) {
            return (STATE_STAGE_CLEAR);
        }

        // 衝突判定
        if (collision == 1) {
            break;
        }

        // 沈没判定
        if (water_level == 0) {
            break;
        }

        wait_vsync(game_speed << 1);
    }

    return (STATE_SINKING);
}

static int game_sinking()
{
    int  n;

    for (n = 0; n < 0x30; n++) {
        key = key_pressed();
        if (key & KBIT_ENT) {
            break;
        }

        // 各種描画と衝突判定
        map_render();
        objects_render(RENDER_COLLISION, 1);

        // 自船点滅
        if (n & 0x08) {
            boat_render_normal();
            zx81_write_str(getrenderaddr((boat_y>>1)-1, (boat_x>>1)), "SINK", 1);
        }

        // 衝突以外の描画
        objects_render(RENDER_CONTACT, 1);

        // 全体描画                                   
        map_draw();

        wait_vsync(game_speed << 1);
    }

    boat_lives--;
    boat_lives_draw();
    if (boat_lives == 0) {
        zx81_write_str(getscrnaddr(6, 10), "GAME OVER", 1);
        wait_vsync(400);

        clear_game_screen();

        return (STATE_TITLE);
    }

    return (STATE_GAME_LOOP);
}

static int game_stage_clear()
{
    zx81_write_str(getscrnaddr(6,7), "CONGRATULATIONS", 1);

    while (water_level-- >= 0) {
        key = key_pressed();
        if (key & KBIT_ENT) {
            return;
        }
        water_gauge_draw(0);
        score++;
        score_draw();
        wait_vsync(game_speed << 1);
    }
    wait_vsync(100);
    round++;

    return (STATE_GAME_LOOP);
}

void main()
{
    // initialize
    GameState gameState = STATE_TITLE;
    hiscore = 0;
    key = 0;
    game_speed = 1;

    while (!(key & KBIT_ENT)) {
        switch (gameState) {
            case STATE_TITLE:
                gameState = game_title();
                break;

            case STATE_GAME_INIT:
                score = 0;
                round = 1;
                boat_lives = 3;
                boat_extend = 0;
                gameState = STATE_GAME_LOOP;
                break;

            case STATE_GAME_LOOP:
                gameState = game_loop();
                break;

            case STATE_SINKING:
                gameState = game_sinking();
                break;

            case STATE_STAGE_CLEAR:
                gameState = game_stage_clear();
                break;
        }
    }
}
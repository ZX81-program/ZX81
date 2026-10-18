/*
 *      ZX81 Boat Handling Module header
 */

#ifndef _ZX81_BOAT_H
#define _ZX81_BOAT_H


#define TILT_L45    0
#define TILT_L23    1
#define TILT_CENTER 2
#define TILT_R23    3
#define TILT_R4     4

#define ASM_TILT_CENTER  2

#define STEER_CENTER   9

// values
extern char boat_x;
extern char boat_y;
extern char boat_speed;
extern char boat_tilt;
extern char tilt_level;

// functions
void boat_init(void);

int  boat_render_normal(void);
int  boat_render_spin(int  spin_frame);
int  boat_render(unsigned char *chrp);

int  boat_bon_voyage(void);
int  boat_move(unsigned char key);
int  boat_contact(struct ACTIVE_TBL *active_tblp);
#asm
EXTERN  asm_boat_contact
#endasm

#endif  // _ZX81_BOAT_H


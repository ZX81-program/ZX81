/*
 *      ZX81 Keyboard Handling API program
 */

#include "key.h"


/*
 *  key_pressed:
 *      Scan keyboard and return bitmask of pressed keys.
 */

unsigned char key_pressed(void)
{
#asm

key_pressed_read:
    ld  a,0xf7
    in  a,(0xfe)
    ld  d,a

    ld  a,0xef
    in  a,(0xfe)
    ld  e,a

    ld a,0xbf
    in a,(0xfe)
    ld h,a

    ld a,0x7f
    in a,(0xfe)
    ld l,a

key_pressed_bitset:
    ld  a,0

key_pressed_1:
    bit 0,d
    jr  nz,key_pressed_2
    or  a,#KBIT_1

key_pressed_2:
    bit 1,d
    jr  nz,key_pressed_3
    or  a,#KBIT_2

key_pressed_3:
    bit 2,d
    jr  nz,key_pressed_left
    or  a,#KBIT_3

key_pressed_left:
    bit 4,d
    jr  nz,key_pressed_right
    or  a,#KBIT_LT

key_pressed_right:
    bit 2,e
    jr  nz,key_pressed_enter
    or  a,#KBIT_RT

key_pressed_enter:
    bit 0,h
    jr  nz,key_pressed_spc
    or  a,#KBIT_ENT

key_pressed_spc:
    bit 0,l
    jr  nz,key_pressed_bitret
    or  a,#KBIT_SPC

key_pressed_bitret:
    ld  h,0
    ld  l,a
    ret

#endasm
}

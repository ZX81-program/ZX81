/*
 *      ZX81 Keyboard Handling API header
 */

#ifndef _ZX81_KEY_H
#define _ZX81_KEY_H


// Bitmask flags for pressed keys (logical game input)
#define KBIT_1      (1 << 0)   // 1
#define KBIT_2      (1 << 1)   // 2
#define KBIT_3      (1 << 2)   // 3
#define KBIT_LT     (1 << 3)   // Left arrow (SHIFT + 5)
#define KBIT_RT     (1 << 4)   // Right arrow (SHIFT + 8)
#define KBIT_ENT    (1 << 5)   // ENTER
#define KBIT_SPC    (1 << 6)   // SPACE

// Key functions
unsigned char key_pressed(void);


#endif  // _ZX81_KEY_H


#include <string.h>

#define D_FILE      0x400c
#define D_DX        (32 + 1)
#define D_DY        24

#define CHR_DX      7
#define CHR_DY      7

static unsigned char chrtbl[CHR_DX*CHR_DY] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x02, 0x89, 0x03, 0x03, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x08, 0x09, 0x09, 0x09, 0x08, 0x08
};

unsigned char *scrnlinep[D_DY];
#define _countof(array)    (sizeof(array) / sizeof(array[0]))

void dispmole(int x, int y)
{
    unsigned char *scrnp, *chrp;
    int  i;

    scrnp = scrnlinep[y] + x;
    chrp = &chrtbl[0];
    for (i = 0; i < CHR_DY; i++) {
        memcpy(scrnp, chrp, CHR_DX);
        scrnp += D_DX;
        chrp += CHR_DX;
    }
}

int main()
{
    int  i;

    for (i = 0; i < _countof(scrnlinep); i++) {
        scrnlinep[i] = (unsigned char *)((*((unsigned int *)D_FILE) + 1 + i * D_DX));
    }

    dispmole(0, 0);

    return (0);
}


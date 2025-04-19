#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_DX    9
#define TABLE_DY    9

#define MINE_NUM    10

#define INVALID     (-1)
#define VAL_MINE    9


static int  s_table[TABLE_DY][TABLE_DX];

static int gettableval(int y, int x)
{
    if ((x < 0) || (x >= TABLE_DX) ||
        (y < 0) || (y >= TABLE_DY))
    {
        return (INVALID);
    }

    return (s_table[y][x]);
}

static int getcount(int y, int x)
{
    int  n;

    n = 0;

    if (gettableval(y, x) == VAL_MINE)
    {
        return (INVALID);
    }

    n += (gettableval(y-1, x-1) == VAL_MINE) ? 1 : 0;
    n += (gettableval(y-1, x  ) == VAL_MINE) ? 1 : 0;
    n += (gettableval(y-1, x+1) == VAL_MINE) ? 1 : 0;
    n += (gettableval(y,   x-1) == VAL_MINE) ? 1 : 0;
    n += (gettableval(y,   x+1) == VAL_MINE) ? 1 : 0;
    n += (gettableval(y+1, x-1) == VAL_MINE) ? 1 : 0;
    n += (gettableval(y+1, x  ) == VAL_MINE) ? 1 : 0;
    n += (gettableval(y+1, x+1) == VAL_MINE) ? 1 : 0;

    return (n);
}

static void initable()
{
    int  n, y, x;

    memset(s_table, 0, sizeof(s_table));

    n = 0;
    while (n < MINE_NUM)
    {
        y = rand() % TABLE_DY;
        x = rand() % TABLE_DX;
        if (s_table[y][x] == VAL_MINE)
        {
            continue;
        }

        s_table[y][x] = VAL_MINE;
        n++;
    }

    for (y = 0;  y < TABLE_DY; y++)
    {
        for (x = 0;  x < TABLE_DX; x++)
        {
            n = getcount(y, x);
            if (n == INVALID)
            {
                continue;
            }

            s_table[y][x] = n;
        }
    }
}

static const char gettablemark(int y, int x)
{
    int  n;

    n = gettableval(y, x);

    switch (n) {
        case INVALID:
            return ('#');

        case 0+200:
            return (' ');

        case VAL_MINE+200:
            return ('*');

        default:
            if (n < 100) {
                return ('.');
            }
            if (n < 200) {
                return ('+');
            }
        break;
    }

    return ('0' + (n - 200));
}

static void disptable()
{
    int  y, x;

    zx_setcursorpos(0, 0);

    for (y = -1;  y <= TABLE_DY; y++)
    {
        for (x = -1;  x <= TABLE_DX; x++)
        {
            printf("%c", gettablemark(y, x));
        }
        printf("\n");
    }

}

static void opentable(int y, int x)
{
    int  n;

    n = gettableval(y, x);
    if (n == INVALID) {
        return;
    }
    if (n >= 100) {
        return;
    }
	n = n % 100;

	s_table[y][x] = n + 200;
    zx_setcursorpos(y+1, x+1);
    printf("%c", gettablemark(y, x));

    if (n == 0) {
        opentable(y-1, x-1);
        opentable(y-1, x  );
        opentable(y-1, x+1);
        opentable(y  , x-1);
        opentable(y  , x+1);
        opentable(y+1, x-1);
        opentable(y+1, x  );
        opentable(y+1, x+1);
    }
}

static int judge()
{
    int  y, x;


    for (y = 0; y < TABLE_DY; y++) {
        for (x = 0; x < TABLE_DX; x++) {
            if ((s_table[y][x] < 100) &&
                (s_table[y][x] != VAL_MINE)) {
                return (0);
            }
        }
    }

    return (1);
}

int main()
{
    int  y, x, y1, x1;
    int  n1, n2, key;

    srand((unsigned int)clock());

    initable();

    disptable();

    x = 0;
    y = 0;
    n1 = 0;
    n2 = 0;
    key = 0;

    while (in_KeyPressed(0x01bf) == 0) {     // press enter to exit
        x1 = x;
        y1 = y;
        if (in_KeyPressed(0x10f7) != 0) {    // left arrow
            x1 = (x1 > 0) ? x1-1 : 0;
        }
        if (in_KeyPressed(0x04ef) != 0) {    // right arrow
            x1 = (x1 < TABLE_DX-1) ? x1+1 : TABLE_DX-1;
        }
        if (in_KeyPressed(0x08ef) != 0) {    // up arrow
            y1 = (y1 > 0) ? y1-1 : 0;
        }
        if (in_KeyPressed(0x10ef) != 0) {    // down arrow
            y1 = (y1 < TABLE_DY-1) ? y1+1 : TABLE_DY-1;
        }

        if ((x1 != x) || (y1 != y)) {
            n2 = (n2 < 3) ? n2+1 : 3;
            if ((n2 == 1) || (n2 == 3)) {
                zx_setcursorpos(y+1, x+1);
                printf("%c", gettablemark(y, x));
                x = x1;
                y = y1;
                zx_setcursorpos(y+1, x+1);
                printf("%c", '@');
            }
        }
        else {
            n1 = (n1 < 5) ? n1+1 : 0;
            n2 = 0;
            zx_setcursorpos(y+1, x+1);
            printf("%c", (n1 < 3) ? '@' : gettablemark(y, x));
        }

        if (in_KeyPressed(0x02fe) != 0) {    // z key
            opentable(y, x);
            if (s_table[y][x] == VAL_MINE+200) {
                for (y = 0; y < TABLE_DY; y++) {
                    for (x = 0; x < TABLE_DX; x++) {
                        if (s_table[y][x] % 100 == VAL_MINE) {
                            s_table[y][x] = VAL_MINE;
                            opentable(y, x);
                        }
                    }
                }
                zx_setcursorpos(y+2, 0);
                printf("++++++++++++++++++++\n");
                printf("+ BOOM...GAME OVER +\n");
                printf("++++++++++++++++++++\n");
                break;
            }
        }

        if (judge() == 1) {
            zx_setcursorpos(y+2, 0);
            printf("+++++++++++++++++++\n");
            printf("+ CONGRATULATION. +\n");
            printf("+++++++++++++++++++\n");
            break;
        }

        in_Wait(20);
    }
}


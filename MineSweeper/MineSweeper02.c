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

        case 0:
            return (' ');

        case VAL_MINE:
            return ('*');

        default:
            break;
    }

    return ('0' + n);
}

static void disptable()
{
    int  y, x;

    for (y = -1;  y <= TABLE_DY; y++)
    {
        for (x = -1;  x <= TABLE_DX; x++)
        {
            printf("%c", gettablemark(y, x));
        }
        printf("\n");
    }

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

    while (key != 13) {
        in_Wait(40);

        key = in_Inkey();
        if (key == 0) {
            n1 = (n1 < 5) ? n1+1 : 0;
            n2 = 0;
            zx_setcursorpos(y+1, x+1);
            printf("%c", (n1 < 3) ? '@' : gettablemark(y, x));
            continue;
        }

        x1 = x;
        y1 = y;

    	if ((n2 % 5)== 0) {
	        switch (key) {
	            case 8:
	                x1 = (x1 > 0) ? x1-1 : 0;
	                break;

	            case 9:
	                x1 = (x1 < TABLE_DX-1) ? x1+1 : TABLE_DX-1;
	                break;

	            case 11:
	                y1 = (y1 > 0) ? y1-1 : 0;
	                break;

	            case 10:
	                y1 = (y1 < TABLE_DY-1) ? y1+1 : TABLE_DY-1;
	                break;

	            default:
	                break;
	        }
        }
        
        n2 = (n2 < 5) ? n2+1 : 5;

        if ((x1 != x) || (y1 != y)) {
            zx_setcursorpos(y+1, x+1);
            printf("%c", gettablemark(y, x));
            x = x1;
            y = y1;
            zx_setcursorpos(y+1, x+1);
            printf("%c", '@');
        }
    }
}


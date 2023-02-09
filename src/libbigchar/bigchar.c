#include <libbigchar/bigchar.h>
#include <libterm/term.h>
#include <common/common.h>
#include <stdio.h>
#include <string.h>

#define HR_BOUND "\u2500"
#define VT_BOUND "\u2502"

int bc_printA(char *str)
{
    printf("%s", str);
    // if (printf("%s", str) != (int)strlen(str))
    // {
    // return FAIL_CODE;
    // }

    return SUCCESS_CODE;
}

int bc_box(int x1, int y1, int x2, int y2)
{
    mt_clrscr();

    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0)
    {
        return FAIL_CODE;
    }

    if (mt_gotoXX(x1, y1) == FAIL_CODE)
    {
        printf("FAIL");
        return FAIL_CODE;
    }

    for (int i = 0; i != x2; ++i)
    {
        if (bc_printA(VT_BOUND) == FAIL_CODE)
        {
            return FAIL_CODE;
        }
        for (int j = 0; j != y2; ++j)
        {
            if (bc_printA(HR_BOUND) == FAIL_CODE)
            {
                return FAIL_CODE;
            }
        }
        if (
            bc_printA(VT_BOUND) == FAIL_CODE)
        {
            return FAIL_CODE;
        }
        if (bc_printA("\n") == -1)
        {
            return FAIL_CODE;
        }
    }

    return SUCCESS_CODE;
}

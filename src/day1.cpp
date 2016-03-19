/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos@amosbuchanan.net $
   $Notice: (C) Copyright 2015 Amos Buchanan. All Rights Reserved. $
   ======================================================================== */

#include <stdio.h>
#include <string.h>
#include <day1.h>
#include <helpers.h>

/*
    (()) and ()() both result in floor 0.
    ((( and (()(()( both result in floor 3.
    ))((((( also results in floor 3.
    ()) and ))( both result in floor -1 (the first basement level).
    ))) and )())()) both result in floor -3.
*/

char test1a[] = "(())";
char test1b[] = "()()";
char test2a[] = "(((";
char test2b[] = "(()(()(";
char test3[] = "))(((((";
char test4a[] = "())";
char test4b[] = "))(";
char test5a[] = ")))";
char test5b[] = ")())())";

struct travel_info
{
    int32 FloorCount;
    int32 FirstBasementPos;
};


int32
countfloors(char *buffer, int32 len, travel_info *Info)
{
    int32 floor = 0;
    int32 pos = 0;

    for (int32 i = 0; i<len ;i++)
    {
        char paren = *(buffer+i);
        if(paren == '\0')
        {
            break;
        }

        else if( paren == '(')
        {
            ++floor;
        }
        else if(paren == ')')
        {
            --floor;
            if(pos == 0 && floor == -1)
            {
                pos = i+1;
            }
        }
    }

    Info->FirstBasementPos = pos;
    Info->FloorCount = floor;

    return floor;
}

void
day1(memory_arena *Arena, bool32 Testing)
{
    if(Testing)
    {
        travel_info Info;
        printf("Testing: \n");
        printf("0 - %d\n", countfloors(test1a, ArrayCount(test1a), &Info));
        printf("0 - %d\n", countfloors(test1b, ArrayCount(test1b), &Info));
        printf("3 - %d\n", countfloors(test2a, ArrayCount(test2a), &Info));
        printf("3 - %d\n", countfloors(test2b, ArrayCount(test2b), &Info));
        printf("3 - %d\n", countfloors(test3, ArrayCount(test3), &Info));
        printf("-1 - %d\n", countfloors(test4a, ArrayCount(test4a), &Info));
        printf("-1 - %d\n", countfloors(test4b, ArrayCount(test4b), &Info));
        printf("-3 - %d\n", countfloors(test5a, ArrayCount(test5a), &Info));
        printf("-3 - %d\n\n", countfloors(test5b, ArrayCount(test5b), &Info));
    }

    FILE *DAY1;
    int32 len = 0;
    char final[10000];


    DAY1 = fopen("files/day1.txt", "r");
    if(DAY1)
    {
        fgets(final, ArrayCount(final), DAY1);
        len = strlen(final);

        travel_info Info;
        countfloors(final, len, &Info);
        printf("Final floorcount: %d, First Basement: %d\n", Info.FloorCount, Info.FirstBasementPos);

        fclose(DAY1);
    }
    else
    {
        printf("Unable to open input file.");
    }
}


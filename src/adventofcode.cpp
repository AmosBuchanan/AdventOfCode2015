/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos@amosbuchanan.net $
   $Notice: (C) Copyright 2015 Amos Buchanan. All Rights Reserved. $
   ======================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <helpers.h>
#include <day1.h>
#include <day2.h>
#include <day3.h>
#include <day4.h>
#include <day5.h>
#include <day6.h>
#include <day7.h>
#include <day8.h>

typedef void (*DayFunctionPtr)(memory_arena *Arena, bool32 Testing);

DayFunctionPtr DayList[] = {
    NULL,
    day1,
    day2,
    day3,
    day4,
    day5,
    day6,
    day7,
    day8
};


void
PrintHelp()
{
    int32 maxdays = ArrayCount(DayList);

    printf("Usage: \n");
    printf("  advent <day>\n\n");
    printf("<day> - A day between 1 and %d, or 0 for output from all days.", maxdays);
}


int
main(int argc, char *argv[])
{
    memory_arena Arena = {};
    InitializeArena(&Arena, (size_t)Gigabytes(1));

    // freopen("err.log", "w", stderr);
    
    if(argc == 1)
    {
        int32 day = ArrayCount(DayList)-1;
        DayList[day](&Arena, true);
    }
    else if (argc == 2)
    {
        int32 day = (int32) strtol(argv[1], &argv[1], 0);
        if(day && day <ArrayCount(DayList))
        {
            DayList[day](&Arena, true);
        }
        else if (day == 0)
        {
            printf("Running All Days.\n");
            for (int32 i = 1; i < ArrayCount(DayList); ++i)
            {
                printf("---- Day %d ----\n", i);
                DayList[i](&Arena, false);
                ResetArena(&Arena);
                printf("\n");
            }
        }
        else
        {
            PrintHelp();
        }
    }
    else
    {
        PrintHelp();
    }

    FreeArena(&Arena);
}

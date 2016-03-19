/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos@amosbuchanan.net $
   $Notice: (C) Copyright 2015 Amos Buchanan. All Rights Reserved. $
   ======================================================================== */

#include <stdio.h>
#include <helpers.h>
#include <day1.h>
#include <day2.h>
#include <day3.h>
#include <day4.h>
#include <day5.h>
#include <day6.h>

void (*DayFunction)(memory_arena *Arena, bool32 Testing);

int
main()
{
    #if DEBUG
    printf("Debug Mode\n");
    #endif

    memory_arena Arena = {};
    InitializeArena(&Arena, (size_t)Gigabytes(1));
    
    day1(&Arena, true);

    FreeArena(&Arena);
}

/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos@amosbuchanan.net $
   $Notice: (C) Copyright 2015 Amos Buchanan. All Rights Reserved. $
   ======================================================================== */

/**
   --- Day 6: Probably a Fire Hazard ---

Because your neighbors keep defeating you in the holiday house
decorating contest year after year, you've decided to deploy one
million lights in a 1000x1000 grid.

Furthermore, because you've been especially nice this year, Santa has
mailed you instructions on how to display the ideal lighting
configuration.

Lights in your grid are numbered from 0 to 999 in each direction; the
lights at each corner are at 0,0, 0,999, 999,999, and 999,0. The
instructions include whether to turn on, turn off, or toggle various
inclusive ranges given as coordinate pairs. Each coordinate pair
represents opposite corners of a rectangle, inclusive; a coordinate
pair like 0,0 through 2,2 therefore refers to 9 lights in a 3x3
square. The lights all start turned off.

To defeat your neighbors this year, all you have to do is set up your
lights by doing the instructions Santa sent you in order.

For example:

    turn on 0,0 through 999,999 would turn on (or leave on) every
    light.

    toggle 0,0 through 999,0 would toggle the first line of 1000
    lights, turning off the ones that were on, and turning on the ones
    that were off.

    turn off 499,499 through 500,500 would turn off (or leave off) the
    middle four lights.

After following the instructions, how many lights are lit?
**/


#include <helpers.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <day6.h>

struct lightgrid
{
    uint8 *Grid;

    int32 Rows;
    int32 Cols;
    size_t Size;
};

enum operation
{
    OP_NOP,
    OP_ON,
    OP_OFF,
    OP_TOGGLE
};

void
InitializeGrid(lightgrid *Grid)
{
    
    uint64 *Pos = (uint64*) Grid->Grid;
    uint8 *End = Grid->Grid + Grid->Size;

    while((Pos+1) < (uint64 *)End)
    {
        *Pos++ = 0;
    }

    if(Grid->Size % 8)
    {
        uint8 *LastBit = (uint8 *)Pos;

        while(LastBit < End)
        {
            *LastBit++ = 0;
        }
    }

}

int32
GetCoord(char **String)
{
    int32 Result = 0;
    char num[4];
    char *pos = *String;
    int i = 0;
    while((*pos >= '0') && (*pos <= '9'))
    {
        num[i++] = *pos++;
    }
    num[i] = '\0';
    Result = atoi(num);

    *String = ++pos;

    return Result;
}

void
ModifyLights(lightgrid *Grid, char* cmd)
{
    char* pos = cmd;
    char cmdstring[10];
    operation Op = OP_NOP;
    v2 StartLoc = {0,0};
    v2 EndLoc = {0,0};


    for(int i=0; i<10; i++)
    {
        char l = *(pos++);
        if(l == ' ')
        {
            if(strcmp(cmdstring, "toggle") == 0)
                Op = OP_TOGGLE;
            if(strcmp(cmdstring, "turn on") == 0)
                Op = OP_ON;
            if(strcmp(cmdstring, "turn off") == 0)
                Op = OP_OFF;

            if(Op != OP_NOP)
                break;
        }
        cmdstring[i] = l;
    }

    if(Op == OP_NOP)
    {
        ERROR("Didn't get an operation for %s! Aborting!\n", cmd);
    }
    else
    {
        StartLoc.X = GetCoord(&pos);
        StartLoc.Y = GetCoord(&pos);
        while(*pos++ != ' '){}
        EndLoc.X = GetCoord(&pos);
        EndLoc.Y = GetCoord(&pos);
        
        INFO("%s", cmd);
        INFO("Operation: %d, From (%d, %d), To (%d, %d)\n", Op, StartLoc.X, StartLoc.Y, EndLoc.X, EndLoc.Y);

        int ChangeCount = 0;
        for(int Y = StartLoc.Y; Y <= EndLoc.Y; Y++)
        {
            for(int X = StartLoc.X; X <= EndLoc.X; X++)
            {
                uint8* pos = Grid->Grid + (X + Y*Grid->Cols);
                switch (Op)
                {
                    case(OP_TOGGLE):
                        *pos ^= 1;
                        break;
                    case(OP_ON):
                        *pos = 1;
                        break;
                    case(OP_OFF):
                        *pos = 0;
                        break;
                    default:
                        break;
                }
                ChangeCount++;
            }
        }

        INFO("Changed %d Bulbs.\n", ChangeCount);
    }

//    printf("\n");
}

/** Part 2

    You just finish implementing your winning light pattern when you
    realize you mistranslated Santa's message from Ancient Nordic
    Elvish.

The light grid you bought actually has individual brightness controls;
each light can have a brightness of zero or more. The lights all start
at zero.

The phrase turn on actually means that you should increase the
brightness of those lights by 1.

The phrase turn off actually means that you should decrease the
brightness of those lights by 1, to a minimum of zero.

The phrase toggle actually means that you should increase the brightness of those lights by 2.

What is the total brightness of all lights combined after following Santa's instructions?

For example:

    turn on 0,0 through 0,0 would increase the total brightness by 1.
    toggle 0,0 through 999,999 would increase the total brightness by 2000000.


 **/
void
ModifyBrightness(lightgrid *Grid, char* cmd)
{

    char* pos = cmd;
    char cmdstring[10];
    operation Op = OP_NOP;
    v2 StartLoc = {0,0};
    v2 EndLoc = {0,0};


    for(int i=0; i<10; i++)
    {
        char l = *(pos++);
        if(l == ' ')
        {
            if(strcmp(cmdstring, "toggle") == 0)
                Op = OP_TOGGLE;
            if(strcmp(cmdstring, "turn on") == 0)
                Op = OP_ON;
            if(strcmp(cmdstring, "turn off") == 0)
                Op = OP_OFF;

            if(Op != OP_NOP)
                break;
        }
        cmdstring[i] = l;
    }

    if(Op == OP_NOP)
    {
        ERROR("Didn't get an operation for %s! Aborting!\n", cmd);
    }
    else
    {
        StartLoc.X = GetCoord(&pos);
        StartLoc.Y = GetCoord(&pos);
        while(*pos++ != ' '){}
        EndLoc.X = GetCoord(&pos);
        EndLoc.Y = GetCoord(&pos);
        
        INFO("%s", cmd); 
        INFO("Operation: %d, From (%d, %d), To (%d, %d)\n", Op, StartLoc.X, StartLoc.Y, EndLoc.X, EndLoc.Y);

        int ChangeCount = 0;
        for(int Y = StartLoc.Y; Y <= EndLoc.Y; Y++)
        {
            for(int X = StartLoc.X; X <= EndLoc.X; X++)
            {
                uint8* pos = Grid->Grid + (X + Y*Grid->Cols);
                switch (Op)
                {
                    case(OP_TOGGLE):
                        *pos += 2;
                        break;
                    case(OP_ON):
                        *pos += 1;
                        break;
                    case(OP_OFF):
                        if(*pos>0)
                            *pos -= 1;
                        break;
                    default:
                        break;
                }
                ChangeCount++;
            }
        }

        INFO("Changed %d Bulbs.\n", ChangeCount);
    }

//    printf("\n");
}

void
day6(memory_arena *Arena, bool32 Testing)
{
    lightgrid LightGridBoolean = {};
    LightGridBoolean.Rows = 1000;
    LightGridBoolean.Cols = 1000;
    LightGridBoolean.Size = LightGridBoolean.Rows * LightGridBoolean.Cols * sizeof(int8);

    LightGridBoolean.Grid = (uint8 *)PushSize(Arena, LightGridBoolean.Size);
    InitializeGrid(&LightGridBoolean);
    
    lightgrid LightGridBrightness = {};
    LightGridBrightness.Rows = 1000;
    LightGridBrightness.Cols = 1000;
    LightGridBrightness.Size = LightGridBrightness.Rows * LightGridBrightness.Cols * sizeof(int8);

    LightGridBrightness.Grid = (uint8 *)PushSize(Arena, LightGridBrightness.Size);
    InitializeGrid(&LightGridBrightness);

    char FileName[] = "files/day6_test1.txt";
    FILE *DAY6;

    DAY6 = fopen(FileName, "r");
    if(DAY6)
    {
        char fileline[100];
        while(fgets(fileline, 100, DAY6))
        {
            ModifyLights(&LightGridBoolean, fileline);
            ModifyBrightness(&LightGridBrightness, fileline);
        }

        fclose(DAY6);
    }
    else
    {
        printf("Unable to open file.\n");
    }

    uint8 *pos = LightGridBrightness.Grid;
    uint32 Lit = 0;
    uint64 Brightness = 0;
    while(pos < (LightGridBrightness.Grid+LightGridBrightness.Size))
    {
        if(*pos)
        {
            Lit++;
            Brightness += *pos;
        }
        pos++;
    }

    uint32 BooleanLit = 0;
    {
        uint8 *pos = LightGridBoolean.Grid;
        BooleanLit = 0;
        while(pos < (LightGridBoolean.Grid+LightGridBoolean.Size))
        {
            if(*pos)
            {
                BooleanLit++;
            }
            pos++;
        }
    }
    printf("Light data (p1):\n");
    printf("There are %d lights on.\n", BooleanLit);

    printf("Light data (p2):\n");
    printf("There are %d lights on.\n", Lit);
    printf("Brightness is %llu.\n", Brightness);
}

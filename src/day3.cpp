/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos@amosbuchanan.net $
   $Notice: (C) Copyright 2015 Amos Buchanan. All Rights Reserved. $
   ======================================================================== */

#include <helpers.h>
#include <day3.h>
#include <stdio.h>

struct house
{
    house *NextInHash;
    v2 Location;
    int32 VisitCount;
};

struct world
{
    house *HousesHash[1024];
    int32 HouseCount;

    v2 SantaLocation;
    int32 SantaStepCount;
    v2 RoboSantaLocation;
    int32 RoboSantaStepCount;
};

house *
NewHouse(memory_arena *Arena, world *World, v2 Location)
{
    house *NextHouse = (house *)PushStruct(Arena, house);
    NextHouse->Location.X = Location.X;
    NextHouse->Location.Y = Location.Y;
    NextHouse->NextInHash = 0;
    NextHouse->VisitCount = 1;
    World->HouseCount++;

    return(NextHouse);
}

house *
GetHouse(memory_arena *Arena, world *World, v2 Location)
{
    v2 HashLoc = Location;
    if(Location.X < 0)
        HashLoc.X *= -1;
    if(Location.Y < 0)
        HashLoc.Y *= -1;
    
    uint32 HashInd = HashLoc.X+HashLoc.Y*3;
    if(HashInd >= ArrayCount(World->HousesHash))
    {
        HashInd = ArrayCount(World->HousesHash)-1;
    }

    bool32 HouseFound = 0;
    house *ThisHouse = World->HousesHash[HashInd];
    house *PrevHouse = 0;
    do
    {
        if(!ThisHouse)
        {
            house *AddedHouse = NewHouse(Arena, World, Location);
            HouseFound = 1;
            if(!World->HousesHash[HashInd])
            {
                World->HousesHash[HashInd] = AddedHouse;
            }
            else if(PrevHouse)
            {
                PrevHouse->NextInHash = AddedHouse;
            }
        }
        else if((ThisHouse) && (ThisHouse->Location.X == Location.X) && (ThisHouse->Location.Y == Location.Y))
        {
            ThisHouse->VisitCount++;
            HouseFound=1;
        }
        else
        {
            PrevHouse = ThisHouse;
            ThisHouse = PrevHouse->NextInHash;
        }
    }while(!HouseFound);

    return ThisHouse;
}

v2
GetNextLocation(v2 CurrentLocation, char Move)
{
    v2 NewLocation = CurrentLocation;
    
    switch(Move)
    {
        case '>':
        {
            NewLocation.X++;
        }break;
        case '<':
        {
            NewLocation.X--;
        }break;
        case '^':
        {
            NewLocation.Y++;
        }break;
        case 'v':
        {
            NewLocation.Y--;
        }break;
    }

    return(NewLocation);
}

void
MoveSanta(memory_arena *Arena, world *World, char Move)
{
    v2 NewLocation = GetNextLocation(World->SantaLocation, Move);
    house *NextHouse = GetHouse(Arena, World, NewLocation);
    World->SantaLocation = NewLocation;
    World->SantaStepCount++;

//    printf("Santa Movement: %c, Santa Location(%d, %d)\n", Move, NewLocation.X, NewLocation.Y);
}

void
MoveRoboSanta(memory_arena *Arena, world *World, char Move)
{
    v2 NewLocation = GetNextLocation(World->RoboSantaLocation, Move);
    house *NextHouse = GetHouse(Arena, World, NewLocation);
    World->RoboSantaLocation = NewLocation;
    World->RoboSantaStepCount++;
//    printf("RoboSanta Movement: %c, RoboSanta Location(%d, %d)\n", Move, NewLocation.X, NewLocation.Y);
}

void
FollowSantaPath(memory_arena *Arena, world *World, char *movement)
{
    while(*movement)
    {
       MoveSanta(Arena, World, *(movement++));
    }

}

void
FollowBothSantaPath(memory_arena *Arena, world *World, char *movement)
{
    while(*movement)
    {
       MoveSanta(Arena, World, *(movement++));
       if(*movement)
           MoveRoboSanta(Arena, World, *(movement++));
    }

}

world *
InitializeWorld(memory_arena *Arena)
{
    ResetArena(Arena);

    world *World = (world *)PushStruct(Arena, world);

    *World = {};
    v2 Location = {0, 0};
    World->HouseCount = 0;
    World->SantaLocation = Location;
    World->RoboSantaLocation = Location;
    World->SantaStepCount = 0;
    World->RoboSantaStepCount = 0;
    World->HousesHash[0] = NewHouse(Arena, World, Location);

    return(World);
}

int32
CountHouses(world *World)
{
    int32 Count = 0;
    for(uint32 i = 0; i < ArrayCount(World->HousesHash); i++)
    {
        if(World->HousesHash[i])
        {
            house *NextHouse = World->HousesHash[i];
            do
            {
                Count++;
                NextHouse = NextHouse->NextInHash;
            } while(NextHouse);
        }
    }
    return Count;
}



void
day3(memory_arena *Arena, bool32 Testing)
{

    if(Testing)
    {
        world *World;
        
        printf("Tests:\n");
        char test1[] = "^";
        char test2[] = "^>v<";
        char test3[] = "^v<>v^><";
        char test4[] = "^v^v^v^v^v";

        World = InitializeWorld(Arena);
        FollowSantaPath(Arena, World, test1);
        printf("%s\n", test1);
        printf("Number of Houses: %d\n", World->HouseCount);
        printf("Santa Location: (%d, %d); RS Loc: (%d, %d)\n", World->SantaLocation.X, World->SantaLocation.Y, World->RoboSantaLocation.X, World->RoboSantaLocation.Y);

        World = InitializeWorld(Arena);
        FollowSantaPath(Arena, World, test2);
        printf("%s\n", test2);
        printf("Number of Houses: %d\n", World->HouseCount);
        printf("Santa Location: (%d, %d); RS Loc: (%d, %d)\n", World->SantaLocation.X, World->SantaLocation.Y, World->RoboSantaLocation.X, World->RoboSantaLocation.Y);

        World = InitializeWorld(Arena);
        FollowSantaPath(Arena, World, test4);
        printf("%s\n", test4);
        printf("Number of Houses: %d\n", World->HouseCount);
        printf("Santa Location: (%d, %d); RS Loc: (%d, %d)\n", World->SantaLocation.X, World->SantaLocation.Y, World->RoboSantaLocation.X, World->RoboSantaLocation.Y);
    }

    world *LoneWorld = InitializeWorld(Arena);
    world *HelpedWorld = InitializeWorld(Arena);
    FILE *DAY3;
    DAY3 = fopen("files/day3.txt", "r");
    if(DAY3)
    {
        char Move[101];
        int32 numread = fread(Move, sizeof(char), 100, DAY3);
        
        int32 count =0;
        while(numread > 0)
        {
            FollowBothSantaPath(Arena, HelpedWorld, Move);
            count+=numread;
            numread = fread(Move, sizeof(char), 100, DAY3);
            Move[numread] = '\0';
            if(count >7900)
                printf("%s\n", Move);
        }

        if(Testing)
        {
            printf("Santa Stepcount: %d, RS Stepcount: %d\n", HelpedWorld->SantaStepCount, HelpedWorld->RoboSantaStepCount);
            printf("Number of houses, straight count: %d\n", CountHouses(HelpedWorld));
            printf("Total Moves: %d\n", count);
        }
        printf("Number of Houses to recieve one present, with Santa and RoboSanta (final): %d\n", HelpedWorld->HouseCount);
        fclose(DAY3);

    }
    else
    {
        printf("Unable to open day 3 file.\n");
    }

// printf("Wrong answer: 2343 (too high)\n");

}

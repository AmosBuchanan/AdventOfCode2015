/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos.buchanan@traxxautomation.com $
   $Notice: (C) Copyright 2015 Traxx Automation. All Rights Reserved. $
   ======================================================================== */

#include <helpers.h>

int
main()
{
    memory_arena Arena = {};
    InitializeArena(&Arena, (size_t)Gigabytes(1));
    

    filelines *FileLines = GetFile(&Arena, "files/day7_test1.txt");

    if(FileLines->FileSize == -1)
    {
        ERROR("Error parsing file.");
    }
    else
    {
        printf("File has %d Lines.\n", FileLines->NumLines);

        for(int32 i = 0; i < FileLines->NumLines; ++i)
        {
            printf("%d - %s\n", i, FileLines->Lines[i]);
        }

        printf("\nTotal Size: %d bytes. Dump:\n", FileLines->FileSize);

        for (int32 i = 0; i < FileLines->FileSize; ++i)
        {
            if(FileLines->File[i] == '\0')
                printf("*");
            else
                printf("%c", FileLines->File[i]);
        }
        printf("\n");
    }

    FreeArena(&Arena);
}

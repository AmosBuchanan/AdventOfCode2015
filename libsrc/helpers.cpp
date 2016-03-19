/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos@amosbuchanan.net $
   $Notice: (C) Copyright 2015 Amos Buchanan. All Rights Reserved. $
   ======================================================================== */

#include <helpers.h>
#include <stdio.h>

filelines *
GetFile(memory_arena *Arena, char* FileName)
{
    filelines *FileLines = {};

    FILE *DAY = fopen(FileName, "r");
    char fileline[100];
    if(DAY && !(fseek(DAY, 0L, SEEK_END)))
    {
        long len = ftell(DAY);
        char *EntireFile = (char*)PushSize(Arena, len);
        fseek(DAY, 0L, SEEK_SET);

        fread(EntireFile, sizeof(char), len, DAY);

        int32 numlines = 0;
        for(int i=0; i < len; ++i)
        {
            char Character = *(EntireFile + i);
            if( Character == '\n')
            {
                *(EntireFile + i) = '\0';
                numlines++;
            }
        }

        char **LineArray = (char **) PushArray(Arena, numlines, size_t);

        LineArray[0] = EntireFile;
        for(int i=0; i<(len-1); ++i)
        {
            if(*(EntireFile+i) == '\0')
            {
                LineArray[1+i] = (EntireFile+i+1);
                i++;
            }
        }

        FileLines = (filelines *)PushStruct(Arena, filelines);
        FileLines->Lines = LineArray;
        FileLines->NumLines = numlines;
        
        fclose(DAY);
    }
    else
    {
        printf("Unable to open file: %s.\n", FileName);
    }


    return(FileLines);
}
    


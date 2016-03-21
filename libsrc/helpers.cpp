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
#include <sys/stat.h>

filelines *
GetFile(memory_arena *Arena, char* FileName)
{
    filelines *FileLines = PushStruct(Arena, filelines);

    struct stat sb;
    if(stat (FileName, &sb) != 0)
    {
        ERROR("Unable to STAT file: %s.", FileName);
    }
    else
    {
        int32 len = sb.st_size;


        FILE *DAY = fopen(FileName, "r");
        if(DAY)
        {
            FileLines->File = (char *)PushSize(Arena, len);
            fread(FileLines->File, sizeof(char), (len), DAY);
            FileLines->Lines[0] = FileLines->File;
            
            int32 numlines = 1;
            for(int i=0; i < len; ++i)
            {
                char Character = *(FileLines->File + i);
                if( Character == EOF)
                {
                    len = i;
                    break;
                }
                if( Character == '\n')
                {
                    *(FileLines->File + i) = '\0';
                    FileLines->Lines[numlines] = FileLines->File + i + 1;
                    numlines++;
                }
            }

            FileLines->File[len] = '\0';
            FileLines->NumLines = --numlines;
            FileLines->FileSize = len;

            fclose(DAY);
        }
        else
        {
            ERROR("Unable to open file: %s.\n", FileName);
            FileLines->File = 0;
            FileLines->NumLines = 0;
            FileLines->FileSize = -1;
            
        }
    }


    return(FileLines);
}
    


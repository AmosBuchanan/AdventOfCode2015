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
#include <string.h>
#include <sys/stat.h>

filelines *
GetFile(memory_arena *Arena,
        const char *FileName)
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
      for(int i = 0; i < len; ++i)
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


char *
SafeStrCpy(char *Dest,
           const char *Source,
           uint32 Len)
{
  char *Value = NULL;
  int32 i = 0;
  for(; i < (Len - 1); i++)
  {
    *(Dest++) = *(Source++);
  }

  if(i == Len)
  {
    strcpy(&Dest[Len - 1], "\0");
  }

  Value = Dest;

  return (Value);
}

int32
SplitString(char *StringToSplit,
            char **StringList,
            int32 MaxStrings,
            char Delim)
{
  StringList[0] = StringToSplit;
  int32 NumStrings = 1;
  if(StringToSplit && *StringToSplit != '\0')
  {
    while(*StringToSplit)
    {
      if(*StringToSplit == Delim && *(StringToSplit + 1) != '\0')
      {
        StringList[NumStrings] = (StringToSplit + 1);
        *StringToSplit = '\0';
        NumStrings++;
      }

      if(NumStrings == MaxStrings)
      {
        break;
      }

      StringToSplit++;
    }
  }
  else
  {
    NumStrings = 0;
  }

  return NumStrings;
}

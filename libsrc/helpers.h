#if !defined(HELPERS_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos@amosbuchanan.net $
   $Notice: (C) Copyright 2015 Amos Buchanan. All Rights Reserved. $
   ======================================================================== */

#define HELPERS_H

#include <stddef.h>
#include <stdint.h>
#include <malloc.h>
#include <stdio.h>
    
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef intptr_t intptr;
typedef uintptr_t uintptr;

typedef size_t memory_index;
    
typedef float real32;
typedef double real64;


#define ArrayCount(array) sizeof(array)/sizeof(array[0])
#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#if DEBUG
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#define INFO(string, ...) fprintf(stderr, "INFO: %s:%d - " string "\n", __FILE__,__LINE__, ##__VA_ARGS__)
#define ERROR(string, ...) fprintf(stderr, "ERROR: %s:%d - " string "\n", __FILE__,__LINE__, ##__VA_ARGS__)
#else
#define Assert(Expression)
#define INFO(string, ...)
#define ERROR(string, ...) fprintf(stderr, "ERROR: %s:%d - " string "\n", __FILE__,__LINE__, ##__VA_ARGS__)
#endif

struct memory_arena
{
    size_t Size;
    uint8 *Base;
    size_t Used;
};

struct temporary_memory
{
    memory_arena *Arena;
    size_t Used;
};

inline void
InitializeArena(memory_arena *Arena, size_t Size)
{
    Arena->Base = (uint8 *)malloc(Size);
    Arena->Size = Size;
    Arena->Used = 0;
}

inline void
FreeArena(memory_arena *Arena)
{
    free(Arena->Base);
}

inline void
ResetArena(memory_arena *Arena)
{
    Arena->Used = 0;
}

#define PushStruct(Arena, type) (type *)PushSize_(Arena, sizeof(type))
#define PushArray(Arena, Count, type) (type *)PushSize_(Arena, (memory_index)(Count)*sizeof(type))
#define PushSize(Arena, Size) PushSize_(Arena, Size)
inline void *
PushSize_(memory_arena *Arena, size_t Size)
{
    Assert((Arena->Used + Size) <= Arena->Size);
    void *Result = Arena->Base + Arena->Used;
    Arena->Used += Size;
    
    return(Result);
}

struct v2
{
    int32 X;
    int32 Y;
};

struct filelines
{
    char *File; 
    char *Lines[10000]; // Assuming a file has a max of 10k lines; about 640k memory used.

    int32 FileSize;
    int32 NumLines;
};

filelines *
GetFile(memory_arena *Arena, char* FileName);




#endif

/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos@amosbuchanan.net $
   $Notice: (C) Copyright 2015 Amos Buchanan. All Rights Reserved. $
   ======================================================================== */

/*
--- Day 8: Matchsticks ---

Space on the sleigh is limited this year, and so Santa will be
bringing his list as a digital copy. He needs to know how much space
it will take up when stored.

It is common in many programming languages to provide a way to escape
special characters in strings. For example, C, JavaScript, Perl,
Python, and even PHP handle special characters in very similar ways.

However, it is important to realize the difference between the number
of characters in the code representation of the string literal and the
number of characters in the in-memory string itself.

For example:

    "" is 2 characters of code (the two double quotes), but the string
    contains zero characters.

    "abc" is 5 characters of code, but 3 characters in the string
    data.

    "aaa\"aaa" is 10 characters of code, but the string itself
    contains six "a" characters and a single, escaped quote character,
    for a total of 7 characters in the string data.

    "\x27" is 6 characters of code, but the string itself contains
    

Santa's list is a file that contains many double-quoted string
literals, one on each line. The only escape sequences used are \\
(which represents a single backslash), \" (which represents a lone
double-quote character), and \x plus two hexadecimal characters (which
represents a single character with that ASCII code).

Disregarding the whitespace in the file, what is the number of
characters of code for string literals minus the number of characters
in memory for the values of the strings in total for the entire file?

For example, given the four strings above, the total number of
characters of string code (2 + 5 + 10 + 6 = 23) minus the total number
of characters in memory for string values (0 + 3 + 7 + 1 = 11) is 23 -
11 = 12.
)
*/

#include <helpers.h>

struct charcounts
{
    uint32 CodeCount;
    uint32 MemCount;
    uint32 SubCount;
    uint32 EncodedCount;
    uint32 SubEncodedCount;
};

charcounts
CountChars(memory_arena *Arena, char *Filename)
{
    filelines *Day8 = GetFile(Arena, Filename);

    uint32 codecount = 0;
    uint32 memcount = 0;
    uint32 encodedcount = 0;
    for(int32 l = 0; l < Day8->NumLines; ++l)
    {
        uint8 *ch = (uint8 *)Day8->Lines[l];
        uint32 inner_codecount = 0;
        uint32 inner_memcount = 0;
        uint32 inner_enccount = 2; // 2 represents the added '"' at each end.
        while(*ch)
        {
            switch (*ch)
            {
                case '"':
                {
                    inner_enccount += 2;
                }break;

                case '\\':
                {
                    inner_enccount += 2;
                    if(*(ch+1) == '\\')
                    {
                        inner_codecount++;
                        ch++;
                        inner_memcount++;
                        inner_enccount += 2;
                    }
                    else if(*(ch+1) == 'x')
                    {
                        inner_codecount += 3;
                        inner_memcount++;
                        ch += 3;
                        inner_enccount += 3;
                    }
                    else if(*(ch+1) == '"')
                    {
                        inner_codecount++;
                        ch++;
                        inner_memcount++;
                        inner_enccount += 2;
                    }
                }break;
                default:
                {
                    inner_memcount++;
                    inner_enccount++;
                }break;
            }
            ch++;
            inner_codecount++;
        }

        codecount += inner_codecount;
        memcount += inner_memcount;
        encodedcount += inner_enccount;
    }

    charcounts Counts = {};
    Counts.CodeCount = codecount;
    Counts.MemCount = memcount;
    Counts.SubCount = (codecount - memcount);
    Counts.EncodedCount = encodedcount;
    Counts.SubEncodedCount = encodedcount - codecount;

    return(Counts);
}


void
day8(memory_arena *Arena, bool32 Testing)
{

    if(Testing)
    {
        charcounts Counts = CountChars(Arena, "files/Day8_test1.txt");
        printf("Testing: Code: %d, Memory: %d, Subtraction: %d\n", Counts.CodeCount, Counts.MemCount, Counts.SubCount);
        printf("  Part2: New Encoding: %d, New Sub: %d\n", Counts.EncodedCount, Counts.SubEncodedCount);
    }

    {
        charcounts Counts = CountChars(Arena, "files/Day8.txt");
        printf("Part 1: Code: %d, Memory: %d, Subtraction: %d\n", Counts.CodeCount, Counts.MemCount, Counts.SubCount);
        printf("Part 2: New Encoding: %d, New Sub: %d\n", Counts.EncodedCount, Counts.SubEncodedCount);
    }

}

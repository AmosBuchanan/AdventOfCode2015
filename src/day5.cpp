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

/**
   A nice string is one with all of the following properties:

    It contains at least three vowels (aeiou only), like aei, xazegov, or aeiouaeiouaeiou.
    It contains at least one letter that appears twice in a row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd).
    It does not contain the strings ab, cd, pq, or xy, even if they are part of one of the other requirements.

For example:

    ugknbfddgicrmopn is nice because it has at least three vowels (u...i...o...), a double letter (...dd...), and none of the disallowed substrings.
    aaa is nice because it has at least three vowels and a double letter, even though the letters used by different rules overlap.
    jchzalrnumimnmhp is naughty because it has no double letter.
    haegwjzuvuyypxyu is naughty because it contains the string xy.
    dvszwmarrgswjxmb is naughty because it contains only one vowel.

*/

union doublechar
{
    char Chars[2];
    int16 Value;
};

typedef bool32 (*nicefunc)(char *String);

bool32
IsVowel(char Letter)
{
    bool32 Result = 0;

    switch(Letter)
    {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            Result = 1;
    };

    return(Result);
}

bool32
IsSameAsNext(char *FirstLetter)
{
    bool32 Result = 0;
    char This = *FirstLetter;
    char Next = *(FirstLetter+1);

    if((This != '\0') && (Next != '\0') && (This == Next))
        Result = 1;

    return(Result);
}

bool32
IsBadDouble(char *FirstLetter)
{
    bool32 Result = 0;
    char This = *FirstLetter;
    char Next = *(FirstLetter+1);
    doublechar BadCombos[] = { { {'a', 'b'} },
                               { {'c', 'd'} },
                               { {'p', 'q'} },
                               { {'x', 'y'} }};

    if((This != '\0') && (Next != '\0'))
    {
        doublechar Test = {{ This, Next }};
        for (int i = 0; i < ArrayCount(BadCombos); ++i)
        {
            if(BadCombos[i].Value == Test.Value)
            {
                Result = 1;
                break;
            }
                
        }
        
    }

    return(Result);
}

bool32
IsNice(char *String)
{
    bool32 Result = 0;
    char *pos = String;

    int32 VowelCount = 0;
    bool32 HasRepeat = 0;
    bool32 HasForbidden = 0;

    while(*pos != '\0')
    {
        if(IsVowel(*pos))
            VowelCount++; 

        if(IsSameAsNext(pos))
            HasRepeat = 1;

        if(IsBadDouble(pos))
        {
            HasForbidden = 1;
            break;
        }

        pos++;
        
    }

    if(HasForbidden)
    {
        Result = 0;
    }
    else
    {
        Result = (VowelCount > 2) && (HasRepeat);
    }

    return(Result);
}

/**
   
    - It contains a pair of any two letters that appears at least twice in the
      string without overlapping, like xyxy (xy) or aabcdefgaa (aa), but not
      like aaa (aa, but it overlaps).
      
    - It contains at least one letter which repeats with exactly one letter
      between them, like xyx, abcdefeghi (efe), or even aaa.
*/

bool32
HasNonOverlappingPair(char *String, int32 Length)
{
    char *pos = String;
    bool32 Result = 0;
    doublechar TestPair = {};
                         

    if(Length > 3)
    {
        TestPair = { { *pos, *(pos+1) } };
        char *it = pos+2;
        while(*(it+1) != '\0')
        {
            doublechar TempPair = { {*it, *(it+1)}};
            if(TestPair.Value == TempPair.Value)
            {
                Result = 1;
                break;
            }
            it++;
        }

        if(Result == 0)
        {
            Result = HasNonOverlappingPair(String+1, Length-1);
        }
        else
        {
            printf("2-letter Pair: %c%c   ", TestPair.Chars[0], TestPair.Chars[1]);
        }
    }
    else
    {
        Result = 0;
        printf("No Pair.  ");
    }
    

    return (Result);
}

bool32
HasMatchOneLetterOut(char *String)
{
    char *pos = String;
    bool32 Result = 0;

    while(*(pos+2) != '\0')
    {
        if((*pos) == (*(pos+2)))
        {
            Result = 1;
            break;
        }

        pos++;
    }

    if(Result)
    {
        printf("Valid Sequence: %c%c%c", *pos, *(pos+1), *(pos+2));
    }
    else
    {
        printf("No Sequence Found.");
    }

    return(Result);
}

bool32
IsNice2(char *String)
{
    bool32 Result = 0;
    char *pos = String;

    bool32 Pair = HasNonOverlappingPair(String, strlen(String)-1);
    bool32 Seq = HasMatchOneLetterOut(String);

    

    Result = Pair && Seq;

    printf("\n");

    return(Result);
}

int32
CountNice(char *FileName, nicefunc NiceFunc, bool32 PrintResults)
{
    int32 NiceCount = 0;
    FILE *DAY5;
    char fileline[100];


    DAY5 = fopen(FileName, "r");
    if(DAY5)
    {
        while(fgets(fileline, 100, DAY5))
        {
            bool32 isNice = 0;

            if(NiceFunc(fileline))
            {
                NiceCount++;
                isNice = 1;
            }

            if(PrintResults)
                printf("%d -- %s\n", isNice, fileline);
            
        }


        fclose(DAY5);
    }
    else
    {
        printf("Unable to open file.\n");
    }


    return(NiceCount);
    
}


void
day5()
{

    char FileName[] = "files/day5.txt";

    int32 Count = CountNice(FileName, IsNice2, 1);

    printf("Nice Count: %d\n", Count);
    
}

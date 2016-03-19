/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos@amosbuchanan.net $
   $Notice: (C) Copyright 2015 Amos Buchanan. All Rights Reserved. $
   ======================================================================== */
#include <helpers.h>
#include <md5.h>
#include <stdio.h>
#include <string.h>

void
day4(memory_arena *Arena, bool32 Testing)
{

    unsigned char digest[16];
    char copy[33];
    char mdString[33];
    const char *start = "bgvyzdsv";

    bool32 found6 = 0;
    bool32 found5 = 0;
    uint32 num6 = 1;
    uint32 num5 = 1;
    char string[100];

    while(!found5 || !found6)
    {
        string[0] = '\0';
        sprintf(string, "%s%d", start, num6);

        MD5_CTX ctx;
        MD5_Init(&ctx);
        MD5_Update(&ctx, string, strlen(string));
        MD5_Final(digest, &ctx);

        for(int i = 0; i < 16; i++)
        {
            sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
        }


        strcpy(copy, mdString);
        copy[6] = '\0';

        if((num6%100) == 0)
        {
            if(Testing)
                printf(".");
        }

        if(strcmp("000000", copy)==0)
        {
            if(Testing)
                printf("\nFound for 6: %d\n", num6);
            found6=1;
        }
        else
        {
            num6++;
        }

        copy[5] = '\0';
        if(!found5 && (strcmp("00000", copy)==0))
        {
            if(Testing)
                printf("\nFound for 5: %d\n", num6);
            found5=1;
            num5 = num6;
        }
    }


    if(Testing)
        printf("md5 digest: %s\n", mdString);
    
    printf("Number for 5: %d\n", num5);
    printf("Number for 6: %d\n", num6);

 
}

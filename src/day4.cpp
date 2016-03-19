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
day4()
{

    unsigned char digest[16];
    char copy[33];
    char mdString[33];
//    const char *start = "bgvyzdsv";
    const char *start = "bgvyzdsv";

    bool32 found=0;
    uint32 num = 1;
    char string[100];

    while(!found)
    {
        string[0] = '\0';
        sprintf(string, "%s%d", start, num);

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

        if((num%100) == 0)
        {
//            printf("String: %s, length: %d\n", string, (int32)strlen(string));
//            printf("Copy: %s\n", copy);
            printf(".");
        }

        if(strcmp("000000", copy)==0)
        {
            printf("Found: %d\n", num);
            found=1;
        }
        else
        {
            num++;
        }

    }



    printf("md5 digest: %s\n", mdString);
    printf("Number: %d\n", num);

 
}

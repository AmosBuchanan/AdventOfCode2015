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
#include <day2.h>


void
quicksort(int *list, int left, int right)
{
    int i=left;
    int j=right;
    int pivot = list[((left+right)/2)];
    while(i<=j)
    {
        while(list[i]<pivot)
        {
            i++;
        }
        while(list[j]>pivot)
        {
            j--;
        }
        if(i<=j)
        {
            int tmp = list[i];
            list[i] = list[j];
            list[j] = tmp;
            i++;
            j--;
        }
    }

    if(left < j)
        quicksort(list, left, j);
    if(i < right)
        quicksort(list, i, right);
}

void
day2()
{
    FILE *DAY2;
    char fileline[100];


    DAY2 = fopen("files/day2.txt", "r");
    if(DAY2)
    {
        int wraptotal = 0;
        int ribtotal = 0;

        while(fgets(fileline, 100, DAY2))
        {
            int sides[3];

            sscanf(fileline, "%dx%dx%d", &sides[0], &sides[1], &sides[2]);

            quicksort(sides, 0, 2);

            wraptotal += 2*(sides[0]*sides[1] + sides[1]*sides[2] + sides[2]*sides[0]);
            wraptotal += sides[0]*sides[1];

            ribtotal += 2*(sides[0]+sides[1]);
            ribtotal += sides[0]*sides[1]*sides[2];
        
        }
        printf("wrapping total: %d sqft\nribbon total: %d ft", wraptotal, ribtotal);

        fclose(DAY2);
    }
    else
    {
        printf("Unable to open file.\n");
    }

}

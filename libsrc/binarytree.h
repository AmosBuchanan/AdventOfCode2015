#if !defined(BINARYTREE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos.buchanan@traxxautomation.com $
   $Notice: (C) Copyright 2015 Traxx Automation. All Rights Reserved. $
   ======================================================================== */

#define BINARYTREE_H

struct node
{
    char Key[16];
    int32 Value;

    node *Left;
    node *Right;
};

int32
get_value(node *Node, char *Key);

void
insert_value(memory_arena *Arena, node **Node, char *Key, int32 Value);


#endif

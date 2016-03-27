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

#include <helpers.h>

struct node
{
    char Key[16];
    void *Value;

    node *Left;
    node *Right;
};

void *
get_value(node **Node, char *Key);

void
 insert_value(memory_arena *Arena, node **Node, char *Key, void *Value);

//void
//print_tree(node *Node);

#endif

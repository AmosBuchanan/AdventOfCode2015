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
    char *Value;

    node *Left;
    node *Right;
};

char *
get_value(node *Node, char *Key, char *Value, int32 len);

void
insert_value(memory_arena *Arena, node **Node, char *Key, char *Value, int32 len);

void
print_tree(node *Node);

#endif

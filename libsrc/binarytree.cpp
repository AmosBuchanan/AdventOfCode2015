/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos.buchanan@traxxautomation.com $
   $Notice: (C) Copyright 2015 Traxx Automation. All Rights Reserved. $
   ======================================================================== */

#include <helpers.h>
#include <binarytree.h>
#include <string.h>

void
insert_value(memory_arena *Arena, node **Node, char *Key, void *Value)
{
    if(!(*Node))
    {
        *Node = (node *)PushStruct(Arena, node);
        
        (*Node)->Value = Value;
        strcpy((*Node)->Key, Key);
        (*Node)->Left = 0;
        (*Node)->Right = 0;
    }
    else if(strcmp((*Node)->Key, Key) < 0)
    {
        insert_value(Arena, &((*Node)->Left), Key, Value);
    }
    else if(strcmp((*Node)->Key, Key) > 0)
    {
        insert_value(Arena, &((*Node)->Right), Key, Value);
    }
    else
    {
        (*Node)->Value = Value;
    }

    return;
}


void *
get_value(node **Node, char *Key)
{
    void *Value = NULL;
    if(!(*Node))
    {
        Value = NULL;
    }
    else if(strcmp((*Node)->Key, Key) == 0)
    {
        Value = (*Node)->Value;
    }
    else if(strcmp((*Node)->Key, Key) < 0)
    {
        Value = get_value(&((*Node)->Left), Key);
    }
    else if(strcmp((*Node)->Key, Key) > 0)
    {
        Value = get_value(&((*Node)->Right), Key);   
    }

    return Value;
}

#if 0
void

print_tree(node *Node)
{
    if(Node->Right)
    {
        print_tree(Node->Right);
    }

    printf("%s: %s\n", Node->Key, Node->Value);
    
    if(Node->Left)
    {
        print_tree(Node->Left);
    }
    
}

#endif

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
insert_value(memory_arena *Arena, node **Node, char *Key, char *Value, int32 len)
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
        insert_value(Arena, &((*Node)->Left), Key, Value, len);
    }
    else if(strcmp((*Node)->Key, Key) > 0)
    {
        insert_value(Arena, &((*Node)->Right), Key, Value, len);
    }
    else
    {
        SafeStrCpy((*Node)->Value, Value, len);
    }

    return;
}


char *
get_value(node *Node, char *Key, char *Value, int32 len)
{
    if(Node == 0)
    {
        Value = NULL;
    }
    else if(strcmp(Node->Key, Key) == 0)
    {
        SafeStrCpy(Value, Node->Value, len);;
    }
    else if(strcmp(Node->Key, Key) < 0)
    {
        Value = get_value(Node->Left, Key, Value, len);
    }
    else if(strcmp(Node->Key, Key) > 0)
    {
        Value = get_value(Node->Right, Key, Value, len);   
    }

    return Value;
}

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

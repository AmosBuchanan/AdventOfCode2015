#if !defined(DAY7_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos.buchanan@traxxautomation.com $
   $Notice: (C) Copyright 2015 Traxx Automation. All Rights Reserved. $
   ======================================================================== */

#define DAY7_H


#define MAX_OPERANDS 2
#define VALUE_LEN 10

enum operation
{
    OP_NOP,
    OP_DEFINE,
    OP_AND,
    OP_OR,
    OP_LSHIFT,
    OP_RSHIFT,
    OP_NOT
};

struct connection
{
    char Variable[16];
    operation Op;

    char Operands[MAX_OPERANDS][VALUE_LEN];
    int32 NumOperands;
};

struct equation
{
    connection *C;
    /*
    char Variable[16];
    operation Op;
    uint16 OuputValue;

    equation *Operands[MAX_OPERANDS];
    int32 NumOperands;
    */
    bool32 isOperated;
    char OutputValue[VALUE_LEN];

    equation *OperandEquations[MAX_OPERANDS];

    equation *Left;
    equation *Right;
};


void day7(memory_arena *Arena, bool32 Testing);
#endif


#if 0



/**
   Return 1 if the variable was successfully added.
 **/
#if 0
bool32
AddToTree(memory_arena *Arena, connection *Con, node **WireTree)
{
    bool32 Added = false;
    char Operand1[VALUE_LEN], Operand2[VALUE_LEN], Result[VALUE_LEN];
    get_value(*WireTree, Con->Operands[0], Operand1, VALUE_LEN);
    get_value(*WireTree, Con->Opearnds[1], Operand2, VALUE_LEN);

    if (*Operand1 != '\0' && *Operand2 != '\0')
        doOperation(Arena, Con->Op, Operand1, Operand2, Value, VALUE_LEN);

    if(Value)
    {
        insert_value(Arena, WireTree, Con->OutputVariable, Value);
        Added = true;
    }
    
    return(Added);
}
#endif

int32
CountNodes(connection *Node)
{
    connection **Root = &Node;
    connection **CurNode = &Node;
    int32 count = 0;
    do
    {
        count++;
        CurNode = &((*CurNode)->Next);

    }while (CurNode != Root);

    return(count);
    
}

void
BuildTree(memory_arena *Arena, filelines *Day7, node **WireTree)
{
    connection *RootNode = 0;
    connection *CurNode = 0;
    for(int32 i = 0; i < Day7->NumLines; ++i)
    {
        printf("%d - ", i);
        connection *NewCon = getConnection(Arena, Day7->Lines[i]);

        if(!doOperation(Arena, NewCon, WireTree))
        {
            if(!CurNode)
            {
                CurNode = NewCon;
                RootNode = NewCon;
            }
            else
            {
                CurNode->Next = NewCon;
                CurNode = CurNode->Next;
            }
        }
    }

    CurNode->Next = RootNode;
    connection *PrevNode = CurNode;
    CurNode = CurNode->Next;
    bool32 isDone = 0;
    int32 i = 0;
    while(!isDone)
    {
//        if((i++ % 1000) == 0)
//            printf("Nodes Left: %d\n", CountNodes(CurNode));
        
        if(!doOperation(Arena, CurNode, WireTree))
        {
            for(int32 i = 0; i < ArrayCount(CurNode->Operands); ++i)
            {
                connection *Con = (connection *)get_value(WireTree, CurNode->Operands[i]);
                if(Con)
                {
                    sprintf(CurNode->Operands[i], "%d", Con->OutputValue);
                }
            }
            CurNode = CurNode->Next;
        }
        else
        {
            if(CurNode->Next == CurNode)
            {
                isDone = 1;
            }
            else
            {
                PrevNode->Next = CurNode->Next;
                CurNode = CurNode->Next;
            }
        }
    }
//    printf("\n");
}


/**
   If the operands are not variables, do the operation and store the connection into the tree.
 **/
bool32
doOperation(memory_arena *Arena, connection *Con, node **WireTree)
{
    bool32 isOpDone = 0;

    if((!isVariable(Con->Operands[0])) && (!isVariable(Con->Operands[1])))
    {
        char Key[VALUE_LEN];
        SafeStrCpy(Key, Con->OutputVariable, VALUE_LEN);
        uint16 Operand1 = 0;
        uint16 Operand2 = 0;
        char *stopstring;
        if(Con->Operands[0][0] != '\0')
            Operand1 = (uint16)strtol(Con->Operands[0], &stopstring, VALUE_LEN);
        if(Con->Operands[1][0] != '\0')
            Operand2 = (uint16)strtol(Con->Operands[1], &stopstring, VALUE_LEN);
        
        operationresult Result = doOperation(Con->Op, Operand1, Operand2);
        if(Result.isSuccess)
        {
            Con->OutputValue = Result.Output;
            insert_value(Arena, WireTree, Key, (void *)Con);
            isOpDone = 1;
        }
    }

    return(isOpDone);
}


void
print_tree(node *Node)
{
    if(Node->Right)
    {
        print_tree(Node->Right);
    }

    connection *Con = (connection *)Node->Value;
    printf("%s: %d\n", Node->Key, Con->OutputValue);
    
    if(Node->Left)
    {
        print_tree(Node->Left);
    }

}


#endif

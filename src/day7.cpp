/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos.buchanan@traxxautomation.com $
   $Notice: (C) Copyright 2015 Traxx Automation. All Rights Reserved. $
   ========================================================================

- Day 7: Some Assembly Required ---

This year, Santa brought little Bobby Tables a set of wires and
bitwise logic gates! Unfortunately, little Bobby is a little under the
recommended age range, and he needs help assembling the circuit.

Each wire has an identifier (some lowercase letters) and can carry a
16-bit signal (a number from 0 to 65535). A signal is provided to each
wire by a gate, another wire, or some specific value. Each wire can
only get a signal from one source, but can provide its signal to
multiple destinations. A gate provides no signal until all of its
inputs have a signal.

The included instructions booklet describes how to connect the parts
together: x AND y -> z means to connect wires x and y to an AND gate,
and then connect its output to wire z.

For example:

    123 -> x means that the signal 123 is provided to wire x.
    x AND y -> z means that the bitwise AND of wire x and wire y is provided to wire z.
    p LSHIFT 2 -> q means that the value from wire p is left-shifted by 2 and then provided to wire q.
    NOT e -> f means that the bitwise complement of the value from wire e is provided to wire f.

Other possible gates include OR (bitwise OR) and RSHIFT
(right-shift). If, for some reason, you'd like to emulate the circuit
instead, almost all programming languages (for example, C, JavaScript,
or Python) provide operators for these gates.

For example, here is a simple circuit:

123 -> x
456 -> y
x AND y -> d
x OR y -> e
x LSHIFT 2 -> f
y RSHIFT 2 -> g
NOT x -> h
NOT y -> i

After it is run, these are the signals on the wires:

d: 72
e: 507
f: 492
g: 114
h: 65412
i: 65079
x: 123
y: 456

In little Bobby's kit's instructions booklet (provided as your puzzle
input), what signal is ultimately provided to wire a?

*/

#include <day7.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <binarytree.h>

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
    char Values[2][16];
    char Output[16];
    
    operation Op;

    connection *Next;
};

operation 
getOperation(char *OperationString)
{
    operation Op = OP_NOP;

    if(strcmp(OperationString, "AND") == 0)
        Op = OP_AND;
    else if(strcmp(OperationString, "OR") == 0)
        Op = OP_OR;
    else if(strcmp(OperationString, "LSHIFT") == 0)
        Op = OP_LSHIFT;
    else if(strcmp(OperationString, "RSHIFT") == 0)
        Op = OP_RSHIFT;
    else if(strcmp(OperationString, "NOT") == 0)
        Op = OP_NOT;

    return Op;
}

int32
doOperation(operation Op, uint16 Val1, uint16 Val2)
{

    int32 Value = 0;

    switch(Op)
    {
        case OP_DEFINE:
        {
            Value = Val1;
        }break;

        case OP_AND:
        {
            Value = Val1 & Val2;
        }break;

        case OP_OR:
        {
            Value = Val1 | Val2;
        }break;

        case OP_LSHIFT:
        {
            Value = Val1 << Val2;
        }break;

        case OP_RSHIFT:
        {
            Value = Val1 >> Val2;
        }break;

        case OP_NOT:
        {
            Value = ~Val1;
        }break;

        default:
        {
        }break;
    }

    return (Value);
        

}

connection
getConnection(memory_arena *Arena, char *Line)
{
    char *scratch, *txt;
    size_t len = strlen(Line);

    txt = strtok_r(Line, " ", &scratch);
    uint16 Valnum = 0;

    connection NewCon = {};

    while(scratch)
    {
        operation ThisOp = getOperation(txt);
        if(ThisOp != OP_NOP)
        {
            NewCon.Op = ThisOp;
        }
        else if((strcmp(txt, "->") == 0) && (NewCon.Op == OP_NOP))
        {
            NewCon.Op = OP_DEFINE;
        }
        else if((strcmp(txt, "->") != 0))
        {
            strcpy(NewCon.Values[Valnum++], txt);
        }
        txt = strtok_r(NULL, " ", &scratch);
    }

    strcpy(NewCon.Output, txt);

    // INFO("OP: %d; %5s %5s, Output: %s", Con.Op, Con.Values[0], Con.Values[1], Con.Output);

    return (NewCon);

}

/**
   Return 1 if variable inserted, 0 if variable caltulated.
 **/
bool32
AddToTree(memory_arena *Arena, connection *Con, node **WireTree)
{
    bool32 Value = 0;
    char Val1[10], Val2[10];
    get_value(*WireTree, Con->Values[0], Val1, 10);
    get_value(*WireTree, Con->Values[1], Val2, 10);
    if((Con->Values[0][0] >= 0x30) && (Con->Values[0][0] <= 0x39) &&
       (Con->Values[1][0] >= 0x30) && (Con->Values[1][0] <= 0x39))
    {
        char *stopstring;
        uint16 uintVal1 = (uint16) strtol(Con->Values[0], &stopstring, 10);
        uint16 uintVal2 = (uint16) strtol(Con->Values[1], &stopstring, 10);

        uint16 FinalValue = doOperation(Con->Op, uintVal1, uintVal2);
        char FinalValueString[10];
        sprintf(FinalValueString, "%u", FinalValue);
        insert_value(Arena, WireTree, Con->Output, FinalValueString, 10);
    }
    else
    {
        insert_value(Arena, WireTree, Con->Output, Con->Output, 10);
        Value = 1;
    }

    return(Value);
}

void
BuildTree(memory_arena *Arena, filelines *Day7, node **WireTree)
{
    connection *RootNode = 0;
    connection *CurNode = 0;
    for(int32 i = 0; i < Day7->NumLines; ++i)
    {
        connection NewCon = getConnection(Arena, Day7->Lines[i]);

        bool32 ThisRoundAddedVariable = AddToTree(Arena, &NewCon, WireTree);
        if(ThisRoundAddedVariable)
        {
            connection *NewNode = (connection *)PushStruct(Arena, connection);
            NewNode->Op = NewCon.Op;
            SafeStrCpy(NewNode->Values[0], NewCon.Values[0], 10);
            SafeStrCpy(NewNode->Values[1], NewCon.Values[1], 10);
            SafeStrCpy(NewNode->Output, NewCon.Output, 10);
            
            if(!CurNode)
            {
                CurNode = NewNode;
            }
            else
            {
                CurNode->Next = NewNode;
                CurNode = CurNode->Next;
            }
        }
    }

    CurNode = RootNode;
    connection *PrevNode = 0;
    while(RootNode)
    {
        bool32 AddedVariable = AddToTree(Arena, CurNode, WireTree);
        if(!AddedVariable)
        {
            if(PrevNode)
            {
                PrevNode->Next = CurNode->Next;
                CurNode = CurNode->Next;
            }
            else
            {
                RootNode = CurNode->Next;
                CurNode = RootNode;
            }
        }
        else
        {
            CurNode = CurNode->Next;
        }
        if(CurNode == 0)
        {
            CurNode = RootNode;
            PrevNode = 0;
        }        
    }

}

void
day7(memory_arena *Arena, bool32 Testing)
{

    if(Testing)
    {
        char TestFileName[] = "files/day7_test1.txt";

        node *WireTree = NULL;

        filelines *Day7 = GetFile(Arena, TestFileName);

        BuildTree(Arena, Day7, &WireTree);

        print_tree(WireTree);
    }

    ResetArena(Arena);

    {
        char TestFileName[] = "files/day7.txt";
        node *WireTree = NULL;
        filelines *Day7 = GetFile(Arena, TestFileName);
        BuildTree(Arena, Day7, &WireTree);

        char PartA[] = "a";
        char WireA[10];
        get_value(WireTree, PartA, WireA, 10);
        printf("Wire A: %s\n", WireA);        
    }
}

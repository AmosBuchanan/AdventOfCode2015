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

connection
getConnection(char *Line)
{
    char *scratch, *txt;
    size_t len = strlen(Line);

    txt = strtok_r(Line, " ", &scratch);
    uint32 Valnum = 0;
    connection Con = {};

    while(scratch)
    {
        operation ThisOp = getOperation(txt);
        if(ThisOp != OP_NOP)
        {
            Con.Op = ThisOp;
        }
        else if((strcmp(txt, "->") == 0) && (Con.Op == OP_NOP))
        {
            Con.Op = OP_DEFINE;
        }
        else if((strcmp(txt, "->") != 0))
        {
            strcpy(Con.Values[Valnum++], txt);
        }
        txt = strtok_r(NULL, " ", &scratch);
    }

    strcpy(Con.Output, txt);

    INFO("OP: %d; %5s %5s, Output: %s", Con.Op, Con.Values[0], Con.Values[1], Con.Output);

    return (Con);

}

void
day7(memory_arena *Arena, bool32 Testing)
{
    if(Testing)
    {
        node *WireTree = (node *)PushStruct(Arena, node);

        char TestFileName[] = "files/day7_test1.txt";

        filelines *Day7 = GetFile(Arena, TestFileName);

        for(int32 i = 0; i < Day7->NumLines; ++i)
        {
            connection Con = getConnection(Day7->Lines[i]);

            char *stopstring;
            int32 Val1 = strtol(Con.Values[0], &stopstring, 10);
            if(!Val1)
                Val1 = get_value(WireTree, Con.Values[0]);
            
        }

    }
    
    
}

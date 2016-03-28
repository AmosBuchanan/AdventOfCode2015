/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Amos Buchanan $
   $Email: amos@amosbuchanan.net $
   $Notice: (C) Copyright 2015 Amos Buchanan. All Rights Reserved. $
   ======================================================================== */

/**   
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

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <binarytree.h>
#include <helpers.h>
#include <day7.h>

#define isVariable(operand) (((*operand >= 0x41) && (*operand <= 0x5A)) || ((*operand >= 0x61) && (*operand <= 0x7A)))
void printEquation(equation *Equation);

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

operationresult
doOperation(operation Op, uint16 Val1, uint16 Val2)
{
    operationresult OpResult = {};
    OpResult.isSuccess = true;
    
    switch(Op)
    {
        case OP_DEFINE:
        {
            OpResult.Output = Val1;
        }break;

        case OP_AND:
        {
            OpResult.Output = Val1 & Val2;
        }break;

        case OP_OR:
        {
            OpResult.Output = Val1 | Val2;
        }break;

        case OP_LSHIFT:
        {
            OpResult.Output = Val1 << Val2;
        }break;

        case OP_RSHIFT:
        {
            OpResult.Output = Val1 >> Val2;
        }break;

        case OP_NOT:
        {
            OpResult.Output = ~Val1;
        }break;

        default:
        {
            OpResult.isSuccess = false;
        }break;
    }

    return (OpResult);
}

connection *
getConnection(memory_arena *Arena, char *Line)
{
    char *scratch, *txt;
    size_t len = strlen(Line);

    txt = strtok_r(Line, " ", &scratch);

    connection *NewCon = PushStruct(Arena, connection);
    NewCon->Op = OP_NOP;
    NewCon->Operands[0][0] = '\0';
    NewCon->NumOperands = 0;

    while(scratch)
    {
        operation ThisOp = getOperation(txt);
        if(ThisOp != OP_NOP)
        {
            NewCon->Op = ThisOp;
        }
        else if((strcmp(txt, "->") == 0) && (NewCon->Op == OP_NOP))
        {
            NewCon->Op = OP_DEFINE;
        }
        else if((strcmp(txt, "->") != 0))
        {
            strcpy(NewCon->Operands[NewCon->NumOperands++], txt);
        }
        txt = strtok_r(NULL, " ", &scratch);
    }

    strcpy(NewCon->Variable, txt);

    INFO("OP: %d; %5s %5s, Variable: %s", NewCon->Op, NewCon->Operands[0], NewCon->Operands[1], NewCon->Variable);

    return (NewCon);
}

void
insertEquation(memory_arena *Arena, equation **ENode, equation *Equation)
{
    if(!(*ENode))
    {
        *ENode = Equation;
        (*ENode)->Left = 0;
        (*ENode)->Right = 0;
    }
    else if(strcmp((*ENode)->C->Variable, Equation->C->Variable) < 0)
    {
        insertEquation(Arena, &((*ENode)->Left), Equation);
    }
    else if(strcmp((*ENode)->C->Variable, Equation->C->Variable) > 0)
    {
        insertEquation(Arena, &((*ENode)->Right), Equation);
    }
    else
    {
        *ENode = Equation;
    }
}

equation *
getEquation(equation **ENode, char *Operand)
{
    equation *Value = NULL;
    if(!(*ENode))
    {
        Value = NULL;
    }
    else if(strcmp((*ENode)->C->Variable, Operand) == 0)
    {
        Value = *ENode;
    }
    else if(strcmp((*ENode)->C->Variable, Operand) < 0)
    {
        Value = getEquation(&((*ENode)->Left), Operand);
    }
    else if(strcmp((*ENode)->C->Variable, Operand) > 0)
    {
        Value = getEquation(&((*ENode)->Right), Operand);
    }

    return(Value);
}

void
BuildEquationTree(memory_arena *Arena, filelines *Day7, equation **EquationTree)
{
    for(int32 i = 0; i < Day7->NumLines; ++i)
    {
        equation *NewEquation = PushStruct(Arena, equation);

        NewEquation->C = getConnection(Arena, Day7->Lines[i]);
        NewEquation->OutputValue[0] = '\0';
        NewEquation->isOperated = false;

        bool32 isHaveAllValues = true;
        uint16 Vals[MAX_OPERANDS] = {};
        char *stopstring;
        for(int32 i = 0; i < NewEquation->C->NumOperands; ++i)
        {
            if(isVariable(NewEquation->C->Operands[i]))
            {
                NewEquation->OperandEquations[i] = getEquation(EquationTree, NewEquation->C->Operands[i]);
                isHaveAllValues = false;
            }
            else
            {
                Vals[i] = (uint16) strtol(NewEquation->C->Operands[i], &stopstring, 10);
            }
        }

        if(isHaveAllValues)
        {
            operationresult Result = doOperation(NewEquation->C->Op, Vals[0], Vals[1]);
            sprintf(NewEquation->OutputValue, "%d", Result.Output);
            NewEquation->isOperated = Result.isSuccess;
        }

        insertEquation(Arena, EquationTree, NewEquation);
    }
}

char *
CalculateTreeNode(equation *ENode, equation **EquationTree)
{
    char *Value = 0;
    if(ENode->isOperated)
    {
        Value = ENode->OutputValue;
    }
    else
    {
        char *Operands[MAX_OPERANDS];
        uint16 Vals[MAX_OPERANDS];
        char *stopstring;
        for(int i = 0; i < ENode->C->NumOperands; ++i)
        {
            if(isVariable(ENode->C->Operands[i]))
            {
                if(!ENode->OperandEquations[i])
                {
                    ENode->OperandEquations[i] = getEquation(EquationTree, ENode->C->Operands[i]);
                }
                Operands[i] = CalculateTreeNode(ENode->OperandEquations[i], EquationTree);
                Vals[i] = (uint16) strtol(Operands[i], &stopstring, 10);
            }
            else
            {
                Vals[i] = (uint16) strtol(ENode->C->Operands[i], &stopstring, 10);
            }

        }
        
        operationresult Result = doOperation(ENode->C->Op, Vals[0], Vals[1]);
        sprintf(ENode->OutputValue, "%d", Result.Output);
        ENode->isOperated = Result.isSuccess;
        Value = ENode->OutputValue;
    }

    return(Value);
}

void
CalculateTree(equation *ENode, equation **EquationTree)
{
    if(ENode->Right)
    {
        CalculateTree(ENode->Right, EquationTree);
    }
    CalculateTreeNode(ENode, EquationTree);
    if(ENode->Left)
    {
        CalculateTree(ENode->Left, EquationTree);
    }
}

void
printEquation(equation *Equation)
{
    printf("%2s: %-5s -> [%d] ", Equation->C->Variable, Equation->OutputValue, Equation->C->Op);
    int32 numOps = Equation->C->NumOperands;
    for (int i = 0; i < numOps; ++i)
    {
        printf("%4s", Equation->C->Operands[i]);
        if(i < (numOps-1))
            printf(", ");
    }
    printf(" (");
    (Equation->isOperated) ? printf("YES") : printf("NO");
    printf(")");
    

    printf("\n");
}

void
printEquationTree(equation *ENode)
{
    if(ENode->Right)
    {
        printEquationTree(ENode->Right);
    }

    printEquation(ENode);

    if(ENode->Left)
    {
        printEquationTree(ENode->Left);
    }
    
}

void
ResetEquations(equation *ENode)
{
    if(ENode->Right)
    {
        ResetEquations(ENode->Right);
    }

    ENode->isOperated = false;

    if(ENode->Left)
    {
        ResetEquations(ENode->Left);
    }
    
}

void
day7(memory_arena *Arena, bool32 Testing)
{
    if(Testing)
    {
        char TestFileName[] = "files/day7_test1.txt";
        filelines *Day7 = GetFile(Arena, TestFileName);

        equation *EquationTree = 0;

        BuildEquationTree(Arena, Day7, &EquationTree);
        printEquationTree(EquationTree);

        printf("\nCalculated Tree:\n");
        CalculateTree(EquationTree, &EquationTree);
        printEquationTree(EquationTree);
        printf("----\n");
    }

    ResetArena(Arena);

    {
        char TestFileName[] = "files/day7.txt";
        filelines *Day7 = GetFile(Arena, TestFileName);

        equation *EquationTree = 0;

        BuildEquationTree(Arena, Day7, &EquationTree);

        char WireALabel[5] = "a";
        equation *WireA = getEquation(&EquationTree, WireALabel);
        CalculateTreeNode(WireA, &EquationTree);

        printf("Part 1: Wire A: %s\n", WireA->OutputValue);
        ResetEquations(EquationTree);

        char WireBLabel[5] = "b";
        equation *WireB = getEquation(&EquationTree, WireBLabel);
        strcpy(WireB->OutputValue, WireA->OutputValue);
        WireB->isOperated = true;

        CalculateTreeNode(WireA, &EquationTree);
        WireA = getEquation(&EquationTree, WireALabel);
        printf("Part 2: Wire A: %s\n", WireA->OutputValue);
    }
}
 

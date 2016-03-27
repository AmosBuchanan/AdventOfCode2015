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

struct operationresult
{
    uint16 Output;
    bool32 isSuccess;
};

void day7(memory_arena *Arena, bool32 Testing);
#endif


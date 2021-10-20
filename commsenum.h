#ifndef COMMSENUM_H_INCLUDED
#define COMMSENUM_H_INCLUDED

#define DEF_CMD(num, name, ...)     \
name = num,     
     

enum commands
{
    #include "commands.def"
};

/*
enum commands
{
    PUSH = 1,
    POP  = 2,
    MUL  = 3,
    DIV  = 4,
    SUB  = 5,
    AFF  = 6,
    OUT  = 7,
    INCORRECT_INPUT = 8,
};
*/
#endif
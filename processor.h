#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include <iostream>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include <sys\stat.h>

#include "stackfunc.h"

extern FILE* log_file;  // да, глобалка, да плохо, а что поделать
extern FILE* input_file;

enum commands
{
    CTOR = 1,
    PUSH,
    POP,
    MUL,
    DIV,
    SUB,
    AFF,
    OUT,
    INCORRECT_INPUT
};

struct com_buff
{
    elem_t* data;
    int tmp_elem = 0;
};
int get_file_stat (FILE* input_file);
int get_commands_from_asm (FILE* input_file, com_buff* buf);
int do_one_command (com_buff* buf);
int type_of_command (elem_t command);


#endif 
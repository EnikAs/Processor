#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include <iostream>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include "stackfunc.h"

typedef int elem_t;

//extern FILE* assembler_file;
extern FILE* calc_file;// a >> w 

enum end_of_file
{
    END_OF_FILE,
    NOT_END_OF_FILE
};

struct Commands
{
    char* command;
    int lenght;
};

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

struct buffer 
{
    char* buffer;
    int string_cunt = 0;
    int tmp_string_cunt = 0;
    int buffer_size = 0;
    int tmp_pos = 0;
};

elem_t       get_int_from_com           (Commands com);

size_t       scanf_file_size            (FILE* file_stream);

int          print_all_commands         (FILE* file_stream);

int          buffer_init                (buffer* buf, FILE* file_stream);

void         buf_string_cunt            (buffer* buf);

int          get_all_commands           (Commands* com, buffer* buf);

int          get_one_command            (Commands* com, buffer* buf);

int          type_of_command            (Commands com, Stack* proc_stack);

void         case_push                  (Commands com, Stack* proc_stack);

void         case_pop                   (Commands com, Stack* proc_stack);

void         case_mul                   (Commands com, Stack* proc_stack);

void         case_div                   (Commands com, Stack* proc_stack);

void         case_aff                   (Commands com, Stack* proc_stack);

void         case_sub                   (Commands com, Stack* proc_stack);

void         case_out                   (Commands com, Stack* proc_stack);

int          push_one_command          (Commands com, Stack* proc_stack);

Commands*    commands_init              (buffer* buf);

#endif 
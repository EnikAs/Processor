
#include "processor.h"

FILE* input_file = fopen("assembler.txt", "rb");

int get_file_stat (FILE* input_file)
{
    struct stat file;
    if ( fstat(fileno(input_file), &file ) == -1 )
    {
        printf("FILE READING ERROR");
        return EOF;
    }
    return file.st_size;
}

com_buff* get_commands_from_asm (FILE* input_file)
{
    int file_size = get_file_stat(input_file);
    com_buff* buf = (com_buff*) calloc(1, sizeof(com_buff));
    buf->data = (elem_t*) calloc(file_size, 1);
    fread(buf->data, sizeof(elem_t), file_size, input_file);
    buf->com_cunt = buf->data[0];
    buf->tmp_elem += 1;

    return buf;
}

int init_one_command (com_buff* buf)
{
    switch(buf->data[buf->tmp_elem])
    {
        case PUSH:
            return PUSH;
            break;
        case POP:
            return POP;
            break;
        case MUL:
            return MUL;
            break;
        case DIV:
            return DIV;
            break;
        case SUB:
            return SUB;
            break;
        case AFF:   
            return AFF;
            break;
        case OUT:
            return OUT;
            break;
        case INCORRECT_INPUT:
            return INCORRECT_INPUT;
            break;
    }
}

int do_one_command (com_buff* buf, Stack* stk)
{
    int command = init_one_command(buf);
    buf->tmp_elem += 1;
    if (command == PUSH)
    {
        StackPush(stk, buf->data[buf->tmp_elem]);
        buf->tmp_elem += 1;                                 // передвигаем tmp_elem, чтобы перейти через значение для пуша
    }

    if (command == POP)
    {
        StackPop(stk);
    }

    if (command == MUL)
    {
        StackPush(stk, StackPop(stk) * StackPop(stk));
    }

    if (command == DIV)
    {
        StackPush(stk, StackPop(stk) / StackPop(stk));
    }

    if (command == SUB)
    {
        StackPush(stk, StackPop(stk) - StackPop(stk));
    }

    if (command == AFF)
    {
        StackPush(stk, StackPop(stk) + StackPop(stk));
    }

    if (command == OUT)
    {
        StackDtor(stk);
    }

    if (command == INCORRECT_INPUT)
    {
        return INCORRECT_INPUT;
    }
    return CORRECT;

}

int do_all_commands (FILE* input_file, Stack* stk)
{
    com_buff* buf = get_commands_from_asm(input_file); 
    int correct_check = -1;
    for (int i = 0 ; i < buf->com_cunt ; i++)
    {
        correct_check = do_one_command(buf, stk);
        if (correct_check == INCORRECT_INPUT)
            return INCORRECT_INPUT;
    }
    return CORRECT;
}

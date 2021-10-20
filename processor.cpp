
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

com_buff* get_commands_from_asm (FILE* input_file, com_buff* buf)
{
    int file_size = get_file_stat(input_file);
    
    buf->data = (elem_t*) calloc(file_size, 1);
    fread(buf->data, sizeof(elem_t), file_size, input_file);
    buf->com_cunt = buf->data[0] + 1;
    buf->tmp_elem += 1;
    fclose(input_file);

    return buf;
}

#undef DEF_CMD

#define DEF_CMD(num, name, ...)         \
    case name:                          \
        return name;                    \
        break;                          \

int init_one_command (com_buff* buf)
{
    switch(buf->data[buf->tmp_elem])
    {
        #include "commands.def"
    }
}

#undef DEF_CMD

#define DEF_CMD(num, name, arg, code)   \
    else if(command == name)            \
    {                                   \
        code                            \
        buf->tmp_elem += arg;           \
    }                                   \

int do_one_command (com_buff* buf, Stack* stk)
{
    int command = init_one_command(buf);
    buf->tmp_elem += 1;
    if(0);

    #include "commands.def"
    /*
    else if (command == PUSH)
    {
        StackPush(stk, buf->data[buf->tmp_elem]);
        buf->tmp_elem += 1;                                 // передвигаем tmp_elem, чтобы перейти через значение для пуша
    }

    else if (command == POP)
    {
        StackPop(stk);
    }

    else if (command == MUL)
    {
        StackPush(stk, StackPop(stk) * StackPop(stk));
    }

    else if (command == DIV)
    {
        StackPush(stk, StackPop(stk) / StackPop(stk));
    }

    else if (command == SUB)
    {
        StackPush(stk, StackPop(stk) - StackPop(stk));
    }

    else if (command == AFF)
    {
        StackPush(stk, StackPop(stk) + StackPop(stk));
    }

    else if (command == OUT)
    {
        StackDtor(stk);
    }
    */
    else
    {
        return INCORRECT_INPUT;
    }
    return CORRECT;

}

int do_all_commands (FILE* input_file, Stack* stk)
{
    com_buff buf = {};
    get_commands_from_asm(input_file, &buf); 
    int correct_check = -1;
    for (int i = 0 ; i < buf.com_cunt ; i++)
    {
        correct_check = do_one_command(&buf, stk);
        if (correct_check == INCORRECT_INPUT)
            return INCORRECT_INPUT;
    }
    return CORRECT;
}

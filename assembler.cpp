#include "assembler.h"

FILE* calc_file = fopen("calc.txt", "r");
FILE* assembler_file = fopen("assembler.jopa", "wb");

int print_all_commands (FILE* file_stream)
{
    if (assembler_file == NULL)
    printf("NULL!!!");

    buffer buf = {};
    Stack proc_stack = {};
    StackCtor(&proc_stack, 1000-7);

    buffer_init(&buf, file_stream);
    Commands* com = commands_init(&buf);
    get_all_commands(com, &buf);

    for (int i = 0 ; i < buf.buffer_size ; i++)
    {
        if (buf.buffer[i] == ';' && buf.buffer[i])
        {
            buf.buffer[i] = '\0';
        }
    }

    StackPush(&proc_stack, buf.tmp_string_cunt);

    for (int i = 0 ; i < buf.tmp_string_cunt ; i++)
    {
        push_one_command(com[i], &proc_stack);
    }

    fwrite(proc_stack.data, sizeof(elem_t), proc_stack.size_of_stack, assembler_file);
    fclose(assembler_file);
    
    return 0;
}

size_t scanf_file_size (FILE* file_stream)
{
    fseek(file_stream, 0, SEEK_END);
    size_t size_of_file = ftell(file_stream);
    fseek(file_stream, 0, SEEK_SET);//rewind

    return size_of_file;
}

int buffer_init (buffer* buf, FILE* file_stream)
{
    buf->buffer_size = scanf_file_size (file_stream);
    buf->buffer = (char*) calloc(buf->buffer_size, sizeof(char));
    if (buf->buffer == NULL)
    {
        printf("buf->buffer == NULL\n");
    }

    buf->buffer_size = fread(buf->buffer, sizeof(char), buf->buffer_size, file_stream);

    buf_string_cunt(buf);
    
    return 0;
}

void buf_string_cunt (buffer* buf)
{
    size_t string_cunt = 0;
    for (int i = 0 ; i < buf->buffer_size ; i++)
    {
        if (buf->buffer[i] == '\n' && buf->buffer[i])
        {
            buf->string_cunt += 1;
            buf->buffer[i] = '\0';       
        }
    }

}

Commands* commands_init (buffer* buf)
{
    Commands* com = (Commands*) calloc(buf->string_cunt + 1, sizeof(Commands));// СТАЛО
    if (com == NULL)
    {        printf("NOT ENOUGH MEMORY");
        return NULL;
    }
    return com;
}

int get_all_commands (Commands* com, buffer* buf)
{
    int end_check = -1;
    while (end_check != END_OF_FILE)
    {
        end_check = get_one_command(com, buf);
    }

    return 0;
}

int get_one_command (Commands* com, buffer* buf)
{
    com[buf->tmp_string_cunt].command = ((char*)buf->buffer + buf->tmp_pos);
    com[buf->tmp_string_cunt].lenght = strlen(com[buf->tmp_string_cunt].command);

    while (buf->buffer[buf->tmp_pos] != '\0')
    {
        buf->tmp_pos++;
    } 

    buf->tmp_pos++;
    buf->tmp_string_cunt++;
    
    if (buf->tmp_pos > buf->buffer_size)
        return END_OF_FILE;
    return -1;
}

int type_of_command (Commands com)
{
    //printf("|%s|\n", com.command);
    if (strncmp(com.command, "pus", 3) == 0)
        return PUSH;
    
    if (strncmp(com.command, "mul", 3) == 0)
        return MUL;

    if (strncmp(com.command, "pop", 3) == 0)
        return POP;

    if (strncmp(com.command, "sub", 3) == 0)
        return SUB;

    if (strncmp(com.command, "div", 3) == 0)
        return DIV;

    if (strncmp(com.command, "out", 3) == 0)
        return OUT;

    if (strncmp(com.command, "aff", 3) == 0)
        return AFF;

    else
        return INCORRECT_INPUT;
}

elem_t get_int_from_com (Commands com)
{
    elem_t tmp_int = 0;
    for(int i = 0 ; i < com.lenght ; i++)
    {
        if (com.command[i] <= '9' && com.command[i] >= '0')
        {
            tmp_int *= 10;
            tmp_int += com.command[i] - '0';
        }
    }
    return tmp_int;
}

void case_push(Commands com, Stack* proc_stack)
{
    elem_t tmp_int = get_int_from_com(com);
    StackPush(proc_stack, PUSH);
    StackPush(proc_stack, tmp_int); 
}

void case_pop(Commands com, Stack* proc_stack)
{
    StackPush(proc_stack, POP);
}

void case_mul(Commands com, Stack* proc_stack)
{
    StackPush(proc_stack, MUL);
}

void case_div(Commands com, Stack* proc_stack)
{
    StackPush(proc_stack, DIV);
}

void case_aff(Commands com, Stack* proc_stack)
{
    StackPush(proc_stack, AFF);
}

void case_sub(Commands com, Stack* proc_stack)
{
    StackPush(proc_stack, SUB);
}

void case_out(Commands com, Stack* proc_stack)
{
    StackPush(proc_stack, OUT);
}

int push_one_command (Commands com, Stack* proc_stack)
{
    switch(type_of_command(com))
    {
        case PUSH:
                    case_push(com, proc_stack);
                    break;
        case MUL:
                    case_mul(com, proc_stack);                    
                    break;
        case POP:
                    case_pop(com, proc_stack);        
                    break;
        case SUB:
                    case_sub(com, proc_stack);        
                    break;
        case DIV:
                    case_div(com, proc_stack);        
                    break;
        case OUT:
                    case_out(com, proc_stack);        
                    break;
        case AFF:
                    case_aff(com, proc_stack);        
                    break;
        case INCORRECT_INPUT:
                    StackPush(proc_stack, INCORRECT_INPUT);
                    break;                   
    }
    return 0;
}
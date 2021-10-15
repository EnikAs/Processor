#include "stackfunc.h"
#include "processor.h"

//extern FILE* log_file;// a >> w Определяется в stackfunc.cpp
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
com_buffer get_commands_from_asm (FILE* input_file)
{
    int file_size = get_file_stat(input_file);
    com_buffer buf = {};
    buf.data = (elem_t*) calloc(file_size, 1);
    fread(buf->data, sizeof(elem_t), file_size, input_file);
    return buf;
}

int do_one_command (com_buff* buf)
{
    switch(buf->data[buf->tmp_elem])
    {
        case PUSH:
            case_push(buf);
            break;
        case POP:
            case_pop(buf);
            break;
        case MUL:
            case_mul(buf);
            break;
        case DIV:
            case_div(buf);
            break;
        case SUB:
            case_sub(buf);
            break;
        case AFF:   
            case_aff(buf);
            break;
        case OUT:
            case_out(buf);
            break;
        case default:
            int ERROR = 0;
            assert(ERROR && "INCORRECT_INPUT!!!");
            break;
    }
}
int do_all_commands (FILE* file_stream, Stack* stk)
{
    buffer buf = {};
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

    for (int i = 0 ; i < buf.tmp_string_cunt ; i++)
    {   
        printf(" >%d<  ", i);
        do_one_command(com[i], stk);
        printf(" >%d< ", i);
    }

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
    printf("STR CUNT = %d", buf->string_cunt);
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

void case_push(com_buff* buf)// Остановился тут, хочу спать ! Нужно продолжить с обработки случаев
{
    elem_t tmp_int = get_int_from_com(com); 
    StackPush(stk, tmp_int);
}

void case_pop(com_buff* buf)
{
    StackPop(stk);
}

void case_mul(com_buff* buf)
{
    elem_t tmp1 = 0, tmp2 = 0;
    tmp1 = StackPop(stk);
    tmp2 = StackPop(stk);
    StackPush(stk, tmp1 * tmp2);
}

void case_div(com_buff* buf)
{
    elem_t tmp1 = 0, tmp2 = 0;
    tmp2 = StackPop(stk);
    tmp1 = StackPop(stk);// ВОПРОС О ТОМ, ЧТО НА ЧТО НУЖНО ДЕЛИТЬ (ПЕРВЫЙ ЧЛЕН НА ВТОРОЙ ИЛИ НАОБОРОТ!!!) 
    StackPush(stk, tmp1 / tmp2);
}

void case_aff(com_buff* buf)
{
    elem_t tmp1 = 0, tmp2 = 0;
    tmp1 = StackPop(stk);
    tmp2 = StackPop(stk);
    StackPush(stk, tmp1 + tmp2);
}

void case_sub(com_buff* buf)
{
    elem_t tmp1 = 0, tmp2 = 0;
    tmp2 = StackPop(stk);
    tmp1 = StackPop(stk);// ВОПРОС О ТОМ, ЧТО ИЗ ЧЕГО НУЖНО ВЫЧИТАТЬ!!! 
    StackPush(stk, tmp1 - tmp2);
}

void case_out(com_buff* buf)
{
    StackDtor(stk);
}

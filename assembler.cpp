#include "assembler.h"

FILE* calc_file = fopen("calc.txt", "r");
FILE* assembler_file = fopen("assembler.jopa", "wb");

int print_all_commands (FILE* file_stream)
{
    if (assembler_file == NULL)
        printf("NULL!!!");

    buffer* buf = (buffer*) calloc(1, sizeof(buffer));

    Stack proc_stack = {};
    StackCtor(&proc_stack, 7);
    char code[1000] = {};// TODO calloc на sizeof(typdef)*2*колличество строк

    buffer_init(buf, file_stream);
    Commands* com = commands_init(buf);
    get_all_commands(com, buf);

    for (int i = 0 ; i < buf->buffer_size ; i++)
    {
        if (buf->buffer[i] == ';' && buf->buffer[i])
        {
            buf->buffer[i] = '\0';
        }
    }

    StackPush(&proc_stack, buf->words_cunt);

    int tmp_com = 0;
    
    for (tmp_com = 0 ; tmp_com < buf->tmp_string_cunt ; tmp_com++)
    {
        push_one_command(com, &proc_stack, &tmp_com);
    }

    fwrite(proc_stack.data, sizeof(elem_t), proc_stack.size_of_stack, assembler_file);
    
    fclose(calc_file);
    fclose(assembler_file);
    free(com);
    free(buf->buffer);

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
        if (buf->buffer[i] == '\n' || buf->buffer[i] == ' ')
        {
            if(buf->buffer[i] == '\n')
                buf->string_cunt += 1;

            buf->buffer[i] = '\0'; 
            buf->words_cunt += 1;      
        }
    }
    buf->words_cunt += 1; 

}

Commands* commands_init (buffer* buf)
{
    Commands* com = (Commands*) calloc(buf->words_cunt + 1, sizeof(Commands));// СТАЛО
    if (com == NULL)
    {   
        printf("NOT ENOUGH MEMORY");
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
        else 
            assert(ERROR && "INCORRECT INPUT!!");
    }
    return tmp_int;
}

#undef DEF_CMD

#define DEF_CMD(num, name, arg, ...)                                            \
    else if (strcmp(com[*tmp_com].command, #name) == 0)                         \
    {                                                                           \
        StackPush(proc_stack, name);                                            \
        if (arg > 0)                                                            \
        {                                                                       \
            *tmp_com += arg;                                                    \
            StackPush(proc_stack, get_int_from_com(com[*tmp_com])) ;            \
        }                                                                       \
    }                                                                           \


int push_one_command (Commands* com, char_mas* array, int* tmp_com)
{
    Cmd bait = {};
    int tmp_int = 0; 
    char tmp_reg = NULL;
    char skobka = NULL;
    if (0);
    //#include "commands.def"
    else if (strcmp(com[*tmp_com].command, "PUSH") == 0)
    {
        *tmp_com += 1;
        if (sscanf(com[*tmp_com].command, "[%d+%cx%c", &tmp_int, &tmp_reg, &skobka) == 3) // ввод в ОП с регистром, команда вида push [123+ax]
        {
            if (skobka == ']')
            {
            bait.ram = 1;
            bait.reg = 1;
            bait.konst = 1;
            bait.cmd = PUSH;
            }
            else 
                return INCORRECT_INPUT;
                
            *((char*)array->mas + array->ip) = bait;
            array->ip += 1; // sizeof(bait)
            *((char*)array->mas + array->ip) = tmp_reg - 'a';
            array->ip += 1; // sizeof(tmp_reg)
            *((char*)array->mas + array->ip) = tmp_int;
            array->ip += 4;
        }
        else if (sscanf(com[*tmp_com].command, "[%cx%c", &tmp_reg, &skobka) == 2) // команда вида push [ax]
        {
            if (skobka == ']')
            {
            bait.ram = 1;
            bait.reg = 1;
            bait.cmd = PUSH;
            }
            else 
                return INCORRECT_INPUT;

            *((char*)array->mas + array->ip) = bait;
            array->ip += 1; // sizeof(bait)
            *((char*)array->mas + array->ip) = tmp_reg - 'a';
            array->ip += 1; // sizeof(tmp_reg)
        }    
        else if (sscanf(com[*tmp_com].command, "[%d%c", &tmp_int, &skobka) == 2) // команда вида push [123]
        {
            if (skobka == ']')
            {
            bait.ram = 1;
            bait.konst = 1;
            bait.cmd = PUSH;
            }
            else 
                return INCORRECT_INPUT;

            *((char*)array->mas + array->ip) = bait;
            array->ip += 1; // sizeof(bait)
            *((char*)array->mas + array->ip) = tmp_int;
            array->ip += 4;
        }  
        else if (sscanf(com[*tmp_com].command, "%cx", &tmp_reg) == 1) // просто регистр, команда вида push ax
        {
            bait.reg = 1;
            bait.cmd = PUSH;

            *((char*)array->mas + array->ip) = bait;
            array->ip += 1; // sizeof(bait)
            *((char*)array->mas + array->ip) = tmp_reg - 'a';
            array->ip += 1; // sizeof(tmp_reg)
        }   
        else if(sscanf(com[*tmp_com].command, "%d", &tmp_int) == 1)// команда вида push 123
        {
            bait.konst = 1;
            bait.cmd = PUSH;

            *((char*)array->mas + array->ip) = bait;
            array->ip += 1; // sizeof(bait)
            *((char*)array->mas + array->ip) = tmp_int;
            array->ip += 4;
        }
    }

    else if (strcmp(com[*tmp_com].command, "MUL") == 0)
    {
        StackPush(proc_stack, MUL);
    }

    else if (strcmp(com[*tmp_com].command, "POP") == 0)
    {
        StackPush(proc_stack, POP);
    }

    else if (strcmp(com[*tmp_com].command, "SUB") == 0)
    {
        StackPush(proc_stack, SUB);
    }

    else if (strcmp(com[*tmp_com].command, "DIV") == 0)
    {
        StackPush(proc_stack, DIV);
    }

    else if (strcmp(com[*tmp_com].command, "OUT") == 0)
    {
        StackPush(proc_stack, OUT);
    }

    else if (strcmp(com[*tmp_com].command, "AFF") == 0)
    {
        StackPush(proc_stack, AFF);
    }

    else
        StackPush(proc_stack, INCORRECT_INPUT);
    
    return 0;
}
    /*
    
    */
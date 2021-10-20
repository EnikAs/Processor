#include "stackfunc.h"
#include "processor.h"
#include "assembler.h"
//#include "disassembler.h"

extern FILE* calc_file;
extern FILE* assembler_file;

int main()
{

    Stack stk = {};
    StackCtor(&stk, 15);
    
    print_all_commands(calc_file);
    
    assembler_file = fopen("assembler.jopa", "rb");
    if (assembler_file == NULL)
        printf("INPUT ERROR!!! (ASSEMBER FILE = NULL)");
    
    int correct_check = -1;
    //com_buff buf ={};
    //get_commands_from_asm(assembler_file, &buf);
    correct_check = do_all_commands(assembler_file, &stk);

    fclose(assembler_file);
    $StackDump(&stk);
    if (correct_check == INCORRECT_INPUT)
        assert(ERROR && "I WILL DELETE YOUR SYSTEM NEXT TIME!)))");

    

    //dis_all_commands(assembler_file);
    return 0;
}
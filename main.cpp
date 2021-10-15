#include "stackfunc.h"
//#include "processor.h"
#include "assembler.h"

extern FILE* calc_file;
extern FILE* assembler_file;

int main()
{

    Stack stk = {};
    StackCtor(&stk, 15);
    
    print_all_commands(calc_file);
    
    
    //for(int i = 0 ; i < 17 ; i++)
    //    StackPush(&stk, 123);
    
    //$StackDump(&stk);
    assembler_file = fopen("assembler.jopa", "rb");
    elem_t mas[40] = {};
    fread(mas, sizeof(elem_t), 25, assembler_file);
    for (int i = 0 ; i < 25 ; i ++)
        printf("%d\n", mas[i]);
    
    struct stat for_read; 

    return 0;
}
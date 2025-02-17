#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if(argc != 2) return -1;

    char *msg = argv[1];
    int len = 0, stack_size = 0; 
    while(msg[len] != '\0') {
        len++;
    }

    int *stack = (int*)malloc(len * sizeof(int));
    char match[5];

    match[('('%5)] = ')';
    match[('{'%5)] = '}';
    match[('['%5)] = ']';

    for(int i=0; i<len; i++) {
        if(msg[i] == '(' || msg[i] == '{' || msg[i] == '[') {
            //push onto stack
            stack[stack_size] = msg[i];
            stack_size++;
        }
        else if(msg[i] == ')' || msg[i] == '}' || msg[i] == ']') {
            if(stack_size == 0 || msg[i] != match[(stack[stack_size-1]%5)]) {
                printf("%d: %c\n", i, msg[i]);
                free(stack);
                return EXIT_FAILURE;
            }
            //pop
            else {
                //stack[stack_size-1] = NULL;
                stack_size--;
            }
        }
        else {
            continue;
        }
        
    }

    if(stack_size != 0) {
        printf("open: ");
        while(stack_size != 0) {
            printf("%c", match[stack[stack_size-1]%5]);
            stack_size--;
        }
        printf("\n");
        free(stack);
        return EXIT_FAILURE;
    }


    free(stack);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct var {
    //name of var
    char *name;
    //0 or 1, current value
    int val;
    struct var *next;
} var;

//each gate has a list of pointers to variables
typedef struct var_ptr {
    struct var *var;
    struct var_ptr *next;
    //for decoder: stores index of output var
    int num;
} var_ptr;

typedef struct gate {
    struct var_ptr *inputs, *outputs;
    //n is for multiplexer and decoder
    int op, n;
    struct gate *next;
} gate;

//num of var currently in list
unsigned int num_var;
var *var_list, *const_0, *const_1;
gate *gate_list = NULL;

//function declarations
var* init_var(char *buf);
var* init_var(char *buf) {
    var *new_var = (var*)malloc(sizeof(var));
    char *name = (char*)malloc(sizeof(char));
    strcpy(name, buf);
    new_var->name = name;
    new_var->val = -1;
    new_var->next = NULL;
    return new_var;
}
void decoder(gate *decoder);
void multiplexer(gate *mux);
void scan_var(FILE *file, int num_i, int num_o, gate *gate);
var* get(char *name, var *list);
void insert_var(var *new_var, var **list);
void insert_ptr(var *new_var, var_ptr **list, int num);
void insert_gate(gate *new_gate, gate **list);

void print_var_list();
void print_var_list() {
    var *ptr = var_list;
    while(ptr != NULL) {
        printf("%s\n", ptr->name);
        ptr = ptr->next;
    }
}
void print_varptr_list(var_ptr *list);
void print_varptr_list(var_ptr *list) {
    var_ptr *ptr = list;
    while(ptr != NULL) {
        printf("%s\n", ptr->var->name);
        ptr = ptr->next;
    }
}
void print_gates_list();
void print_gates_list() {
    gate *ptr = gate_list;
    while(ptr != NULL) {
        printf("op: %d\n", ptr->op);
        ptr = ptr->next;
    }
}

int main(int argc, char **argv) {

    //read a file containing description of a circuit
    if(argc != 2)
        return EXIT_FAILURE;

    FILE *input = fopen(argv[1], "r");
    if(input == NULL)
        return EXIT_FAILURE;
    
    char buffer[17];
    int ret = 0, num_inputs, num_outputs;
    //initialize global var
    var_list = NULL;
    gate_list = NULL;
    const_0 = init_var("0");
    const_0->val = 0;
    const_1 = init_var("1");
    const_1->val = 1;

    //scan inputs
    ret = fscanf(input, "%17s", buffer);
    if(ret != 1 || strcmp("INPUT", buffer) != 0) 
        return EXIT_FAILURE;
    ret = fscanf(input, "%d", &num_inputs);
    if(ret != 1 || num_inputs <= 0) 
        return EXIT_FAILURE;
    ret = fscanf(input, "%17s", buffer);
    if(ret != 1) 
        return EXIT_FAILURE;
    var_list = init_var(buffer);
    var *ptr = var_list;
    for(int i = 1; i < num_inputs; i++) {
        ret = fscanf(input, "%17s", buffer);
        if(ret != 1) 
            return EXIT_FAILURE;
        ptr->next = init_var(buffer);
        ptr = ptr->next;
    }

    //scan outputs
    ret = fscanf(input, "%s", buffer);
    if(ret != 1 || strcmp("OUTPUT", buffer) != 0)
        return EXIT_FAILURE;
    ret = fscanf(input, "%d", &num_outputs);
    if(ret != 1 || num_outputs <= 0) 
        return EXIT_FAILURE;
    //ptr is the last var (last input var) in the list
    for(int i = 0; i < num_outputs; i++) {
        ret = fscanf(input, "%17s", buffer);
        if(ret != 1) 
            return EXIT_FAILURE;
        ptr->next = init_var(buffer);
        ptr = ptr->next;
    }
    
    ret = fscanf(input, "%17s", buffer);
    if(ret != 1)
        return EXIT_FAILURE;
    while(ret != EOF) {
        gate *new_gate = (gate*)malloc(sizeof(gate));
        new_gate->inputs = NULL;
        new_gate->outputs = NULL;
        new_gate->next = NULL;
        if(strcmp(buffer, "NOT") == 0) {
            new_gate->op = 1;
            scan_var(input, 1, 1, new_gate);
        }
        else if(strcmp(buffer, "AND") == 0) {
            new_gate->op = 2;
            scan_var(input, 2, 1, new_gate);
            if(new_gate->inputs == NULL) {
                printf("NULL\n");
            }
            //print_varptr_list(new_gate->inputs);
        }
        else if(strcmp(buffer, "OR") == 0) {
            new_gate->op = 3;
            scan_var(input, 2, 1, new_gate);
        }
        else if(strcmp(buffer, "NOR") == 0) {
            new_gate->op = 4;
            scan_var(input, 2, 1, new_gate);
        }
        else if(strcmp(buffer, "NAND") == 0) {
            new_gate->op = 5;
            scan_var(input, 2, 1, new_gate);
        }
        else if(strcmp(buffer, "XOR") == 0) {
            new_gate->op = 6;
            scan_var(input, 2, 1, new_gate);
        }
        else if(strcmp(buffer, "DECODER") == 0) {
            new_gate->op = 7;
            ret = fscanf(input, "%d", &(new_gate->n));
            if(ret != 1)
                return EXIT_FAILURE;
            scan_var(input, new_gate->n, (1 << new_gate->n), new_gate);
        }
        else if(strcmp(buffer, "MULTIPLEXER") == 0) {
            new_gate->op = 8;
            ret = fscanf(input, "%d", &(new_gate->n));
            if(ret != 1)
                return EXIT_FAILURE;
            scan_var(input, ((new_gate->n) << 1) + new_gate->n, 1, new_gate);
        }
        else if(strcmp(buffer, "PASS") == 0) {
            new_gate->op = 9;
            scan_var(input, 1, 1, new_gate);
        }
        else {
            exit(EXIT_FAILURE);
        }
        //insert gate into gates list
        insert_gate(new_gate, &(gate_list));

        //scan next var
        ret = fscanf(input, "%17s", buffer);
    }

    for(int i = 0; i < (1 << num_inputs); i++) {
        //run through gates linked list
        var *input_var = var_list;
        int mask;
        //set val for all input var
        for(int j = (num_inputs - 1); j >= 0; j--) {
            //create bit mask
            mask = 1 << j;
            input_var->val = (i & mask) >> j;
            printf("%d ", input_var->val);
            input_var = input_var->next;
        }
        printf("|");
        gate *ptr = gate_list;
        while(ptr != NULL) {
            switch(ptr->op) {
                case 1: //NOT
                    ptr->outputs->var->val = (~ptr->inputs->var->val) & 1;
                    break;
                case 2: //AND
                    ptr->outputs->var->val = (ptr->inputs->var->val & ptr->inputs->next->var->val) & 1;
                    break;
                case 3: //OR
                    ptr->outputs->var->val = (ptr->inputs->var->val | ptr->inputs->next->var->val) & 1;
                    break;
                case 4: //NOR
                    ptr->outputs->var->val = (~(ptr->inputs->var->val | ptr->inputs->next->var->val)) & 1;
                    break;
                case 5: //NAND
                    ptr->outputs->var->val = (~(ptr->inputs->var->val & ptr->inputs->next->var->val)) & 1;
                    break;
                case 6: //XOR
                    ptr->outputs->var->val = (ptr->inputs->var->val ^ ptr->inputs->next->var->val) & 1;
                    break;
                case 7: //DECODER
                    decoder(ptr);
                    break;
                case 8: //MULTIPLEXER
                    //printf(" mux");
                    multiplexer(ptr);
                    break;
                case 9: //PASS
                    ptr->outputs->var->val = ptr->inputs->var->val;
                    break;
                default:
                    exit(EXIT_FAILURE);
            }
            ptr = ptr->next;
        }
        //print outputs
        var *output = input_var;
        for(int j = 0; j < num_outputs; j++) {
            printf(" %d", output->val);
            output = output->next;
        }
        printf("\n");
    }
    //free allocated memory
    var *var_prev = var_list, *var_curr = var_list;
    gate *gate_prev = gate_list, *gate_curr = gate_list;
    var_ptr *ptr_prev, *ptr_curr;
    while(var_curr != NULL) {
        var_prev = var_curr;
        var_curr = var_curr->next;
        free(var_prev->name);
        free(var_prev);
    }
    while(gate_curr != NULL) {
        gate_prev = gate_curr;
        gate_curr = gate_curr->next;

        ptr_prev = gate_prev->inputs;
        ptr_curr = gate_prev->inputs;
        while(ptr_curr != NULL) {
            ptr_prev = ptr_curr;
            ptr_curr = ptr_curr->next;
            free(ptr_prev);
        }
        ptr_prev = gate_prev->outputs;
        ptr_curr = gate_prev->outputs;
        while(ptr_curr != NULL) {
            ptr_prev = ptr_curr;
            ptr_curr = ptr_curr->next;
            free(ptr_prev);
        }
        free(gate_prev);
    }
    return EXIT_SUCCESS;
}

void decoder(gate *decoder) {
    unsigned int s = 0;
    var_ptr *ptr = decoder->inputs;
    for(int i = (decoder->n) - 1; i >= 0; i--) {
        s += ((ptr->var->val) << i);
        ptr = ptr->next;
    }
    //printf("s: %d\n", s);
    ptr = decoder->outputs;
    while(ptr != NULL) {
        if(ptr->num == s)
            ptr->var->val = 1;
        else
            ptr->var->val = 0;
        ptr = ptr->next;
    }
}

void multiplexer(gate *mux) {
    unsigned int s = 0;
    var_ptr *ptr = mux->inputs;
    for(int i = (1 << mux->n); i > 0; i--) {
        ptr = ptr->next;
    }
    for(int i = (mux->n) - 1; i >= 0; i--) {
        s += (ptr->var->val) << i;
        ptr = ptr->next;
    }
    ptr = mux->inputs;
    for(int i = 1; i <= s; i++) {
        ptr = ptr->next;
    }
    mux->outputs->var->val = ptr->var->val;
}

//scan a certain number of var from one line of input file
void scan_var(FILE *file, int num_i, int num_o, gate *gate) {
    int ret = 0;
    char buffer[17];
    var *new_var;
    for(int i = 0; i < num_i; i++) {
        ret = fscanf(file, "%17s", buffer);
        if(ret != 1)
            exit(EXIT_FAILURE);
        //check if input is 0 or 1
        if(strcmp(buffer, "0") == 0)
            insert_ptr(const_0, &(gate->inputs), -1);
        else if(strcmp(buffer, "1") == 0)
            insert_ptr(const_1, &(gate->inputs), -1);
        else {
            new_var = get(buffer, var_list);
            if(new_var == NULL) {
                new_var = init_var(buffer);
                insert_var(new_var, &var_list);
            }
            //add to inputs list
            insert_ptr(new_var, &(gate->inputs), -1); 
        } 
    }
    for(int i = 0; i < num_o; i++) {
        ret = fscanf(file, "%17s", buffer);
        if(ret != 1)
            exit(EXIT_FAILURE);
        if(strcmp(buffer, "_") == 0) {
            continue;
        }
        else {
            new_var = get(buffer, var_list);
            if(new_var == NULL) {
                new_var = init_var(buffer);
                insert_var(new_var, &(var_list));
            }
            //if decoder, record the index of the output var
            if(gate->op == 7)
                insert_ptr(new_var, &(gate->outputs), i);
            else
                insert_ptr(new_var, &(gate->outputs), -1);
        }
    }
}

//return var node with given var name, NULL if DNE
var* get(char *name, var *list) {
    var *ptr = list;
    while(ptr != NULL) {
        if(strcmp(name, ptr->name) == 0) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

//insert a given var at the end of var_ptr linked list
void insert_ptr(var *new_var, var_ptr **list, int num) {
    var_ptr *new_node = (var_ptr*)malloc(sizeof(var_ptr));
    new_node->var = new_var;
    new_node->next = NULL;
    new_node->num = num;
    if((*list) == NULL) {
        (*list) = new_node;
        return;
    }
    var_ptr *ptr = (*list);
    while(ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = new_node;
}

//insert a given var at the end of var linked list
void insert_var(var *new_var, var **list) {
    if((*list) == NULL) {
        (*list) = new_var;
        return;
    }
    var *ptr = (*list);
    while(ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = new_var;
}

//insert a given gate at the end of gates linked list
void insert_gate(gate *new_gate, gate **list) {
    if((*list) == NULL) {
        (*list) = new_gate;
        return;
    }
    gate *ptr = (*list);
    while(ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = new_gate;
}
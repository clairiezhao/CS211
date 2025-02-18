#include <stdio.h>
#include <stdlib.h>

struct node {
    int val;
    struct node *next;
};

void insert(struct node **head, int n, int* list_len);
void delete(struct node **head, int n, int* list_len);
void printlist(struct node **head, int list_len);

int main(int argc, char *argv[]) {

    int op_int;
    char op_char;
    int n, success, list_len = 0;
    struct node *head = NULL;

    op_int = getchar();

    while(op_int != EOF) {

        op_char = (char)op_int;
        getchar();
        success = scanf("%d", &n);
        if(success != 1) return EXIT_FAILURE;

        if(op_char == 'i') {
            insert(&head, n, &list_len);
        }
        else if(op_char == 'd') {
            delete(&head, n, &list_len);
        }
        else
            return EXIT_FAILURE;

        //print list
        printlist(&head, list_len);

        //scan newline
        getchar();
        op_int = getchar();
    }


    return 0;
}

void insert(struct node **head, int n, int* list_len) {

    if(*head == NULL) {
        *head = (struct node*)malloc(sizeof(struct node));
        (*head)->val = n;
        (*head)->next = NULL;
        (*list_len)++;
        return;
    }

    if((*head)->val == n) {
        return;
    }

    if((*head)->val > n) {
        struct node *new_node = (struct node*)malloc(sizeof(struct node));
        new_node->val = n;
        new_node->next = *head;
        *head = new_node;
        (*list_len)++;
        return;
    }

    //declare temp ptr
    struct node *ptr = *head;

    //iterate until first val greater than target
    while(ptr->next != NULL && ptr->next->val <= n) {
        if(ptr->next->val == n) {
            return;
        }
        ptr = ptr->next;
    }

    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    new_node->val = n;
    new_node->next = ptr->next;
    ptr->next = new_node;
    (*list_len)++;

}

void delete(struct node **head, int n, int* list_len) {

    if(*head == NULL) {
        return;
    }

    struct node *temp = *head;

    if((*head)->val == n) {
        *head = (*head)->next;
        (*list_len)--;
        free(temp);
        return;
    }

    struct node *ptr = *head;

    while(ptr->next != NULL) {

        if(ptr->next->val == n) {
            temp = ptr->next;
            ptr->next = ptr->next->next;
            (*list_len)--;
            free(temp);
            return;
        }
        ptr = ptr->next;
    }

}

void printlist(struct node **head, int list_len) {

    struct node *ptr = *head;

    printf("%d :", list_len);

    while(ptr != NULL) {
        printf(" %d", ptr->val);
        ptr = ptr->next;
    }
    printf("\n");
}
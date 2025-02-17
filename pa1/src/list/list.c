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

    //scan standard input until session terminates
    char op;
    int n, success, list_len = 0;
    struct node *head = NULL;

    success = scanf("%c", &op);
    if(success != 1) return EXIT_FAILURE;

    while(op != EOF) {
        scanf("%*c");
        success = scanf("%d", &n);
        if(success != 1) return EXIT_FAILURE;

        //printf("op: %c, num: %d, success: %d\n", op, n, success);

        if(op == 'i') {
            insert(&head, n, &list_len);
        }
        else if(op == 'd') {
            delete(&head, n, &list_len);
        }
        else
            return EXIT_FAILURE;

        //print list
        printlist(&head, list_len);

        //scan newline
        scanf("%*c");
        success = scanf("%c", &op);
        if(success != 1) return 0;
    }


    return 0;
}

void insert(struct node **head, int n, int* list_len) {
    //go until first value that is larger

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
    //search for n in list
    //if n exists, set prev to point to next

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
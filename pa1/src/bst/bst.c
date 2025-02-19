#include <stdio.h>
#include <stdlib.h>

struct bstnode {
    int val;
    struct bstnode *left, *right;
};

void search(struct bstnode **node, int n);
struct bstnode* insert(struct bstnode **node, int n);
struct bstnode* delete(struct bstnode **node, int n);
void print(struct bstnode **root);

int main(int argc, char *argv[]) {

    int op_int;
    char op_char;
    int n, success;
    struct bstnode *root = NULL;

    op_int = getchar();

    while(op_int != EOF) {

        op_char = (char)op_int;
        
        if(op_char == 'p') {
            print(&root);
            printf("\n");
            //scan newline
            getchar();
            op_int = getchar();
            continue;
        }

        getchar();
        success = scanf("%d", &n);
        if(success != 1) return EXIT_FAILURE;

        if(op_char == 's') {
            search(&root, n);
        }
        else if(op_char == 'i') {
            root = insert(&root, n);
        }
        else if(op_char == 'd') {
            delete(&root, n);
        }
        else
            return EXIT_FAILURE;

        //scan newline
        getchar();
        op_int = getchar();
    }

    return 0;
}

void search(struct bstnode **node, int n) {
    if((*node) == NULL) {
        puts("absent");
        return;
    }
    if((*node)->val == n) {
        puts("present");
        return;
    }

    if(n < (*node)->val) {
        search(&(*node)->left, n);
    }
    else if(n > (*node)->val) {
        search(&(*node)->right, n);
    }
}

struct bstnode* insert(struct bstnode **node, int n) {
    if(*node == NULL) {
        //allocate new node
        struct bstnode *new_node = (struct bstnode*)malloc(sizeof(struct bstnode));
        new_node->val = n;
        new_node->left = NULL;
        new_node->right = NULL;
        printf("inserted\n");
        return new_node;
    }

    if(n < (*node)->val) {
        (*node)->left = insert(&(*node)->left, n);
    }
    else if(n > (*node)->val) {
        (*node)->right = insert(&(*node)->right, n);
    }
    else{
        printf("not inserted\n");
    }

    return *node;
}

struct bstnode* delete(struct bstnode **node, int n) {
    if((*node) == NULL) {
        printf("absent\n");
        return NULL;
    }

    if(n < (*node)->val) {
        (*node)->left = delete(&(*node)->left, n);
    }
    else if(n > (*node)->val) {
        (*node)->right = delete(&(*node)->right, n);
    }
    else {
        printf("deleted\n");
        struct bstnode *temp = *node;

        if((*node)->left == NULL) {
            //deallocate current node
            *node = (*node)->right;
            free(temp);
            return *node;
        }
        if((*node)->right == NULL) {
            *node = (*node)->right;
            free(temp);
            return *node;
        }
        //else both children are not null
        temp = (*node)->left;
        
        if(temp->right == NULL) {
            (*node)->val = temp->val;
            (*node)->left = temp->left;
            free(temp);
            return *node;
        }

        struct bstnode *temp_prev = (*node);

        while(temp->right != NULL) {
            temp_prev = temp;
            temp = temp->right;
        }
        //temp is now max node in left subtree
        (*node)->val = temp->val;
        
        //delete max
        if(temp->left != NULL)
            temp_prev->right = temp_prev->right->left;
        else
            temp_prev->right = NULL;
            
        free(temp);
    }
    return *node;
}

void print(struct bstnode **root) {
    if((*root) == NULL) {
        return;
    }

    printf("(");
    print(&((*root)->left));
    printf("%d", (*root)->val);
    print(&((*root)->right));
    printf(")");
}
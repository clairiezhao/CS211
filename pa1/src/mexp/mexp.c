#include <stdio.h>
#include <stdlib.h>

void sqmatrixmult(int **matrix_a, int **matrix_b, int ***matrix_prod, int n);

int main(int argc, char *argv[]) {

    FILE *input = fopen(argv[1], "r");
    if(!input) return EXIT_FAILURE;

    int success, n;
    success = fscanf(input, "%d", &n);
    if(success != 1) return EXIT_FAILURE;
    if(n <= 0) return 0;
    //scan newline
    fscanf(input, "%*c");

    //allocate 3 nxn int arr
    int **matrix = (int**)malloc(n * sizeof(int*));
    matrix[0] = malloc(n * n * sizeof(int));

    int **matrix_prod = (int**)malloc(n * sizeof(int*));
    matrix_prod[0] = malloc(n * n * sizeof(int));

    int **copy_matrix_prod = (int**)malloc(n * sizeof(int*));
    copy_matrix_prod[0] = malloc(n * n * sizeof(int));


    for(int i=0; i<n; i++) {
        //set arr indices
        matrix[i] = &matrix[0][i * n];
        matrix_prod[i] = &matrix_prod[0][i * n];
        copy_matrix_prod[i] = &copy_matrix_prod[0][i * n];

        for(int j=0; j<n; j++) {
            success = fscanf(input, "%d", &matrix[i][j]);
            if(success != 1) return -1;

            matrix_prod[i][j] = matrix[i][j];
            copy_matrix_prod[i][j] = matrix[i][j];
            
            fscanf(input, "%*c");
        }
    }

    int exp;
    success = fscanf(input, "%d", &exp);
    if(success != 1) return EXIT_FAILURE;

    if(exp == 0) {
        //matrix_prod is I_n
        for(int i=0; i<n; i++) {
            for(int j=0; j<(n-1); j++) {
                if(i == j) 
                    printf("1 ");
                else
                    printf("0 ");
            }
            if(i == (n-1))
                printf("1\n");
            else
                printf("0\n");
        }
    }
    else {
        for(int i=1; i<exp; i++) {
            sqmatrixmult(matrix, copy_matrix_prod, &matrix_prod, n);
            //set all val of matrix_prod to copy_matrix_prod
            for(int row=0; row<n; row++) {
                for(int col=0; col<n; col++) {
                    copy_matrix_prod[row][col] = matrix_prod[row][col];
                }
            }
        }
        //print
        for(int row=0; row<n; row++) {
            for(int col=0; col<n-1; col++) {
                printf("%d ", matrix_prod[row][col]);
            }
            printf("%d\n", matrix_prod[row][n-1]);
        }
    }

    //deallocate
    free(matrix[0]);
    free(matrix);
    free(matrix_prod[0]);
    free(matrix_prod);
    free(copy_matrix_prod[0]);
    free(copy_matrix_prod);
    
    return EXIT_SUCCESS;
}

void sqmatrixmult(int **matrix_a, int **matrix_b, int ***matrix_prod, int n) {

    int dot_prod;

    for(int row=0; row<n; row++) {
        for(int col=0; col<n; col++) {
            dot_prod = 0;
            for(int i=0; i<n; i++) {
                dot_prod += ((matrix_a[row][i]) * matrix_b[i][col]);
            }
            (*matrix_prod)[row][col] = dot_prod;
        }
    }

}
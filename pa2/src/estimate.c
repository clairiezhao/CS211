#include <stdio.h>
#include <stdlib.h>

int mat_inv(double **mat, int n, double **matinv);
int is_identity(double **mat, int n);
void mat_trans(double **mat, int rows, int cols, double **mattrans);
void mat_mult (double **mat1, int mat1_r, int mat1_c, double **mat2, int mat2_r, int mat2_c, double **matprod);

int main(int argc, char **argv) {

    //scan argv[1], train: matrix houses x attributes + 1, matrix prices houses x 1
    FILE *training_file = fopen(argv[1], "r");
    int ret = fscanf(training_file, "%*s");
    if(ret != 1) return EXIT_FAILURE;

    //scan num houses (rows) and attributes (cols-1)
    int rows = 0, cols = 0;
    ret = fscanf(training_file, "%d", &rows);
    if(ret != 1) return EXIT_FAILURE;
    ret = fscanf(training_file, "%d", &cols);
    if(ret != 1) return EXIT_FAILURE;
    cols += 1;

    //1. houses matrix (houses) x (attr+1)
    double **houses_mat = (double**)malloc(rows * sizeof(double*));
    houses_mat[0] = (double*)malloc(rows * cols * sizeof(double));
    //2. prices matrix (houses) x 1
    double **prices = (double**)malloc(rows * sizeof(double*));
    prices[0] = (double*)malloc(rows * sizeof(double));
    //3. weights matrix (attr+1) x 1
    double **weights = (double**)malloc(cols * sizeof(double*));
    weights[0] = (double*)malloc(cols * 1 * sizeof(double));

    //utility matrices
    //4. houses matrix transpose (attr+1) x (houses)
    double **houses_mat_trans = (double**)malloc(cols * sizeof(double*));
    houses_mat_trans[0] = (double*)malloc(rows * cols * sizeof(double));
    //5. square matrix product (attr+1) x (attr+1)
    double **sq_mat_prod = (double**)malloc(cols * sizeof(double*));
    sq_mat_prod[0] = (double*)malloc(cols * cols * sizeof(double));
    //6. rect matrix product (houses) x (attr+1)
    double **rect_mat_prod = (double**)malloc(rows * sizeof(double*));
    rect_mat_prod[0] = (double*)malloc(rows * cols * sizeof(double));
    //7. square matrix inverse (attr+1) x (attr+1)
    double **sq_mat_inv = (double**)malloc(cols * sizeof(double*));
    sq_mat_inv[0] = (double*)malloc(cols * cols * sizeof(double));

    //set row indices for matrices with num rows = cols
    for(int i = 0; i < cols; i++) {
        weights[i] = &weights[0][i];
        houses_mat_trans[i] = &houses_mat_trans[0][i * cols];
        sq_mat_prod[i] = &sq_mat_prod[0][i * cols];
        sq_mat_inv[i] = &sq_mat_inv[0][i * cols];
    }

    for(int i = 0; i < rows; i++) {
        //set row indices
        houses_mat[i] = &houses_mat[0][i * cols];
        houses_mat[i][0] = 1;
        prices[i] = &prices[0][i];
        
        for(int j = 1; j < (cols-1); j++) {
            ret = fscanf(training_file, "%lf", &houses_mat[i][j]);
            if(ret != 1) return EXIT_FAILURE;
        }

        ret = fscanf(training_file, "%lf", &prices[i][0]);
        if(ret != 1) return EXIT_FAILURE;
    }

    //find weights matrix: W = inv(houses_trans x houses) x houses_trans x prices
    mat_trans(houses_mat, rows, cols, houses_mat_trans);
    mat_mult(houses_mat_trans, cols, rows, houses_mat, rows, cols, sq_mat_prod);
    int success = mat_inv(sq_mat_prod, cols, sq_mat_inv);
    printf("successfully inverted: %d\n", success);
    mat_mult(sq_mat_inv, cols, cols, houses_mat_trans, cols, rows, rect_mat_prod);
    mat_mult(rect_mat_prod, cols, rows, prices, rows, 1, weights);


    //scan argv[2], data file
    FILE *data_file = fopen(argv[2], "r");
    ret = fscanf(data_file, "%*s");
    if(ret != 1) return EXIT_FAILURE;

    int d_rows = 0, d_cols = 0;
    ret = fscanf(training_file, "%d", &d_cols);
    //num attributes should match in both files
    if(ret != 1 || d_cols != cols) return EXIT_FAILURE;
    ret = fscanf(training_file, "%d", &d_rows);
    if(ret != 1) return EXIT_FAILURE;

    //data matrix (houses) x (attr + 1)
    double **data_mat = (double**)malloc(d_rows * sizeof(double*));
    data_mat[0] = (double*)malloc(d_rows * d_cols * sizeof(double));
    //estimated prices matrix (houses) x 1
    double **est_prices = (double**)malloc(d_rows * sizeof(double*));
    est_prices[0] = (double*)malloc(d_rows * sizeof(double));

    for(int i = 0; i < d_rows; i++) {
        data_mat[i] = &data_mat[0][i * cols];
        data_mat[i][0] = 1;
        est_prices[i] = &est_prices[0][i];

        for(int j = 1; j < cols; j++) {
            ret = fscanf(data_file, "%lf", &data_mat[i][j]);
            if(ret != 1) return EXIT_FAILURE;
        }
    }

    mat_mult(data_mat, d_rows, d_cols, weights, d_cols, 1, est_prices);
    //print estimated prices
    for(int i = 0; i < d_rows; i++) {
        printf("%.0f\n", est_prices[i][0]);
    }

    free(houses_mat[0]);
    free(houses_mat);
    free(prices[0]);
    free(prices);
    free(weights[0]);
    free(weights);
    
    free(houses_mat_trans[0]);
    free(houses_mat_trans);
    free(sq_mat_prod[0]);
    free(sq_mat_prod);
    free(rect_mat_prod[0]);
    free(rect_mat_prod);
    free(sq_mat_inv[0]);
    free(sq_mat_inv);

    free(data_mat[0]);
    free(data_mat);
    free(est_prices[0]);
    free(est_prices);
}

int mat_inv(double **mat, int n, double **matinv) {
    //set matinv to I(n)
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i == j)
                matinv[i][j] = 1;
            else
                matinv[i][j] = 0;
        }
    }

    //Gaussian elimination
    int pivot, under;
    for(int i = 0; i < n; i++) {
        //set pivot elem = 1; divide pivot row by pivot element
        pivot = mat[i][i];
        if(pivot == 0) continue;
        for(int j = 0; j < n; j++) {
            mat[i][j] /= pivot;
            matinv[i][j] /= pivot;
        }

        //get all 0s under pivot elem
        for(int j = (i+1); j < n; j++) {
            under = mat[j][i];
            for(int k = 0; k < n; k++) {
                mat[j][k] -= under * mat[i][k];
                matinv[j][k] -= under * matinv[i][k];
            }
        }
    }

    return is_identity(mat, n);
}

int is_identity(double **mat, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i == j) {
                if(mat[i][j] != 1) return 0;
            }
            else
                if(mat[i][j] != 0) return 0; 
        }
    }
    return 1;
}

void mat_trans(double **mat, int rows, int cols, double **mattrans) {
    
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            mattrans[j][i] = mat[i][j];
        }
    }
}

void mat_mult (double **mat1, int mat1_r, int mat1_c, double **mat2, int mat2_r, int mat2_c, double **matprod) {
    double dotprod;
    //1c = 2r
    //matprod is a 1r x 2c matrix

    for(int i = 0; i < mat1_r; i++) {
        for(int j = 0; j < mat2_c; j++) {
            dotprod = 0;
            for(int k = 0; k < mat1_r; k++)
                dotprod += mat1[i][k] * mat2[k][j];
            matprod[i][j] = dotprod;
        }
    }
}
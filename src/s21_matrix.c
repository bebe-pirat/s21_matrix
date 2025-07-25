#include "s21_matrix.h"

// typedef struct matrix_struct {
//     double** matrix;
//     int rows;
//     int columns;
// } matrix_t;

int s21_create_matrix(int rows, int columns, matrix_t *result) {
    int error = 0; 

    if (rows == 0 || columns == 0) error = 1;
    if (!error) {
        result->rows = rows;
        result->columns = columns;

        result->matrix = (double**)calloc(rows, sizeof(double*));
        
        if(result->matrix == NULL) error = 1;

        if (!error) {
             for (int i = 0; i < rows && !error; i++) {
                result->matrix[i] = (double*)calloc(columns, sizeof(double));
                if (result->matrix[i] == NULL) {
                    for (int j = 0; j < i; j++) {
                        free(result->matrix[j]);
                    }
                    free(result->matrix);
                    result->matrix = NULL;
                    error = 1;
                }
            }
        }
    }
    
    return error;
}

void s21_remove_matrix(matrix_t *A) {
    if (A != NULL && A->matrix != NULL) {
        for (int i = 0; i < A->rows; i++) {
            if (A->matrix[i] != NULL) free(A->matrix[i]);
        }

        free(A->matrix);
        A->rows = 0;
        A->columns = 0;
    }
}
#include "s21_matrix.h"

// typedef struct matrix_struct {
//     double** matrix;
//     int rows;
//     int columns;
// } matrix_t;

int s21_create_matrix(int rows, int columns, matrix_t *result) {
    int error = 0; 

    if (result == NULL || rows <= 0 || columns <= 0) error = 1;
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
        A->matrix = NULL;
        A->rows = 0;
        A->columns = 0;
    }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
    int result = SUCCESS;

    if (A == NULL || B == NULL || A->matrix == NULL || B->matrix == NULL) result = FAILURE;
    
    if (result && (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0)) result = FAILURE;
    
    if (result == SUCCESS) {
        if (A->rows != B->rows || A->columns != B->columns) result = FAILURE;
        else {
            for (int i = 0; i < A->rows && result == SUCCESS; i++) {
                for (int j = 0; j < A->columns && result == SUCCESS; j++) {
                  if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPSILON) result = FAILURE;
                }
            }
        }
    } 
    
    return result;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int error = S21_OK;

    if (A == NULL || B == NULL || A->matrix == NULL || B->matrix == NULL) error = S21_INCORRECT_MATRIX;
    if (!error && (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0)) error = S21_INCORRECT_MATRIX;
    
    if (!error && (A->rows != B->rows || A->columns != B->columns)) error = S21_ERROR_CALCULATION;

    int rows = A->rows, columns = A->columns;
    
    int error = s21_create_matrix(rows, columns, result);

    if (!error) {   
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
            }
        }
    }

    return error;
}
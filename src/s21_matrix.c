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

    if (A == NULL || B == NULL || result == NULL || A->matrix == NULL || B->matrix == NULL) error = S21_INCORRECT_MATRIX;
    if (!error && (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0)) error = S21_INCORRECT_MATRIX;
    if (!error && (A->rows != B->rows || A->columns != B->columns)) error = S21_ERROR_CALCULATION;

    if (!error) {
        int rows = A->rows, columns = A->columns;
        error = s21_create_matrix(rows, columns, result);
        if (!error) {   
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
                }
            }
        }
    }

    return error;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int error = S21_OK;

    if (A == NULL || B == NULL || result == NULL || A->matrix == NULL || B->matrix == NULL) error = S21_INCORRECT_MATRIX;
    if (!error && (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0)) error = S21_INCORRECT_MATRIX;
    if (!error && (A->rows != B->rows || A->columns != B->columns)) error = S21_ERROR_CALCULATION;

    if (!error) {
        int rows = A->rows, columns = A->columns;
        error = s21_create_matrix(rows, columns, result);
        if (!error) {   
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
                }
            }
        }
    }

    return error;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
    int error = S21_OK;

    if (A == NULL || result == NULL || A->matrix == NULL) error = S21_INCORRECT_MATRIX;
    if (!error && (A->rows <= 0 || A->columns <= 0)) error = S21_INCORRECT_MATRIX;
    
    if (!error) {
        int rows = A->rows, columns = A->columns;
        error = s21_create_matrix(rows, columns, result);
        if (!error) {   
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    result->matrix[i][j] = A->matrix[i][j] * number;
                }
            }
        }
    }

    return error;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int error = S21_OK;

    if (A == NULL || B == NULL || result == NULL || A->matrix == NULL || B->matrix == NULL) error = S21_INCORRECT_MATRIX;
    if (!error && (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0)) error = S21_INCORRECT_MATRIX;
    if (!error && A->columns != B->rows) error = S21_ERROR_CALCULATION;

    if (!error) {
        int rows = A->rows, columns = A->columns;
        error = s21_create_matrix(rows, columns, result);
        if (!error) {   
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    result->matrix[i][j] = 0;
                    for (int k = 0; k < A->columns; k++) {
                        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
                    }
                }
            }
        }
    }

    return error;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
    int error = S21_OK;

    if (A == NULL || result == NULL || A->matrix == NULL) error = S21_INCORRECT_MATRIX;
    if (!error && (A->rows <= 0 || A->columns <= 0)) error = S21_INCORRECT_MATRIX;
    
    if (!error) {
        int rows = A->rows, columns = A->columns;
        error = s21_create_matrix(columns, rows, result);
        if (!error) {   
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    result->matrix[j][i] = A->matrix[i][j];
                }
            }
        }
    }

    return error;
}

void get_minor(matrix_t *A, matrix_t *minor, int skip_row, int skip_col) {
    int minor_row = 0;
    for (int i = 0; i < A->rows; i++) {
        if (i != skip_row) {
            int minor_col = 0;
            for (int j = 0; j < A->columns; j++) {
                if (j != skip_col) {                    
                    minor->matrix[minor_row][minor_col] = A->matrix[i][j];
                    minor_col++;
                }
            }
            minor_row++;
        }   
    }
}

int s21_determinant(matrix_t *A, double *result) {
    int error = S21_OK;

    if (A == NULL || A->matrix == NULL || result == NULL) error = S21_INCORRECT_MATRIX;

    if (!error && A->columns != A->rows || A->columns <= 0) error = S21_ERROR_CALCULATION;
    
    if (!error) {
        int rows = A->rows;
        if (A->rows == 1) *result = A->matrix[0][0];
        else if (A->rows == 2) *result = A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
        else {
            matrix_t minor;
            error = s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
            if (!error) {
                int sign = 1;
                for (int j = 0; j < A->columns; j++) {
                    get_minor(A, &minor, 0, j); 
                    
                    double minor_det = 0;
                    s21_determinant(&minor, &minor_det);
                    
                    *result += sign * A->matrix[0][j] * minor_det;
                    sign *= -1; 
                }
                s21_remove_matrix(&minor);
            }
        }
    }
    return error;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
    int error = S21_OK;

    if (A == NULL || A->matrix == NULL || result == NULL) error = S21_INCORRECT_MATRIX;
    if (!error && A->columns != A->rows || A->columns <= 0) error = S21_ERROR_CALCULATION;

    if (!error) {
        int rows = A->rows, columns = A->columns;
        error = s21_create_matrix(rows, columns, result);
        matrix_t minor;
        error = s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
        if (!error) {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    get_minor(A, &minor, 0, j); 
                    double minor_det = 0;
                    s21_determinant(&minor, &minor_det);
                    result->matrix[i][j] = pow(-1, j + i) * minor_det;
                }
            }
        }    
        s21_remove_matrix(&minor);
    }
}
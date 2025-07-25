#ifndef S21_MATRIX_H

#define S21_OK 0
#define S21_INCORRECT_MATRIX 1
#define S21_ERROR_CALCULATION

#define SUCCESS 1
#define FAILURE 0

typedef struct matrix_struct {
    double** matrix;
    int rows;
    int columns;
} matrix_t;

// create and remove function
int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);

// equal 
int s21_eq_matrix(matrix_t *A, matrix_t *B);

// arithmetic operations 
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

// транспонирование
int s21_transpose(matrix_t *A, matrix_t *result);

// вычисление определителя матрицы
int s21_determinant(matrix_t *A, double *result);

// обратная матрица
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

// какая-то фигня
int s21_calc_complements(matrix_t *A, matrix_t *result);


#endif
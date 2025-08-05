#include <check.h>
#include <stdlib.h>
#include "../s21_matrix.h"

void s21_fill_matrix(matrix_t *A, double value) {
  if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        A->matrix[i][j] = value;
      }
    }
  }
}

void s21_init_matrix(matrix_t *A, double value, double step) {
    if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        A->matrix[i][j] = value;
        value += step;
      }
    }
  }
}

START_TEST(test_create_matrix_ok) {
    matrix_t mat = {};
    int res = s21_create_matrix(3, 4, &mat);
    ck_assert_int_eq(res, 0); 
    ck_assert_ptr_nonnull(mat.matrix); 
    ck_assert_int_eq(mat.rows, 3); 
    ck_assert_int_eq(mat.columns, 4); 

    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.columns; j++) {
            ck_assert_double_eq(mat.matrix[i][j], 0.0);
        }
    }

    s21_remove_matrix(&mat); 
}
END_TEST

START_TEST(test_create_matrix_error) {
    matrix_t mat = {};
    
    ck_assert_int_eq(s21_create_matrix(-1, 5, &mat), 1);

    ck_assert_int_eq(s21_create_matrix(1, -5, &mat), 1);

    ck_assert_int_eq(s21_create_matrix(-1, -5, &mat), 1);

    ck_assert_int_eq(s21_create_matrix(3, 0, &mat), 1);
    
    ck_assert_int_eq(s21_create_matrix(0, 0, &mat), 1);
    
    ck_assert_int_eq(s21_create_matrix(0, 3, &mat), 1);
    
    ck_assert_int_eq(s21_create_matrix(2, 2, NULL), 1);
}
END_TEST

START_TEST(test_remove_matrix_ok) {
    matrix_t mat = {};
    s21_create_matrix(2, 2, &mat);
    
    mat.matrix[0][0] = 1.0;
    mat.matrix[0][1] = 2.0;
    mat.matrix[1][0] = 3.0;
    mat.matrix[1][1] = 4.0;
    
    s21_remove_matrix(&mat);
    
    ck_assert_ptr_null(mat.matrix);
    ck_assert_int_eq(mat.rows, 0);
    ck_assert_int_eq(mat.columns, 0);
}
END_TEST

Suite *create_and_remove_suite(void) {
    Suite *s;
    TCase *tc_core, *tc_remove;

    s = suite_create("Matrix");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_create_matrix_ok);
    tcase_add_test(tc_core, test_create_matrix_error);
    suite_add_tcase(s, tc_core);

    tc_remove = tcase_create("Remove");
    tcase_add_test(tc_remove, test_remove_matrix_ok);
    suite_add_tcase(s, tc_remove);

    return s;
}

START_TEST(test_eq_matrix_ok1) {
    matrix_t mat1 = {};
    matrix_t mat2 = {};
    s21_create_matrix(2, 2, &mat1);
    s21_create_matrix(2, 2, &mat2);
    
    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 1);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_ok2) {
    matrix_t mat1 = {};
    matrix_t mat2 = {};
    s21_create_matrix(2, 3, &mat1);
    s21_create_matrix(2, 3, &mat2);
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            mat1.matrix[i][j] = i + j;
            mat2.matrix[i][j] = i + j;
        }
    }

    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 1);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_ok3) {
    matrix_t mat1 = {};
    matrix_t mat2 = {};
    s21_create_matrix(2, 3, &mat1);
    s21_create_matrix(2, 3, &mat2);
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            mat1.matrix[i][j] = i + j / 2;
            mat2.matrix[i][j] = i + j / 2;
        }
    }

    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 1);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_false_1) {
    matrix_t mat1 = {}, mat2;
    s21_create_matrix(2, 2, &mat1);
    s21_create_matrix(2, 2, &mat2);

    mat1.matrix[0][0] = 1.0; mat2.matrix[0][0] = 1.0;
    mat1.matrix[0][1] = 2.0; mat2.matrix[0][1] = 2.1; 
    mat1.matrix[1][0] = 3.0; mat2.matrix[1][0] = 3.0;
    mat1.matrix[1][1] = 4.0; mat2.matrix[1][1] = 4.0;

    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 0);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_false_2) {
    matrix_t mat1 = {};
    matrix_t mat2 = {};
    s21_create_matrix(2, 3, &mat1);
    s21_create_matrix(3, 2, &mat2);

    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 0);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_false_3) {
    matrix_t mat1 = {};
    matrix_t mat2 = {};
    s21_create_matrix(2, 2, &mat1);
    s21_create_matrix(3, 2, &mat2);

    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 0);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_false_4) {
    matrix_t mat1 = {};
    matrix_t mat2 = {};
    s21_create_matrix(2, 2, &mat1);
    s21_create_matrix(2, 2, &mat2);

    mat1.matrix[1][1] = 1;

    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 0);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_false_5) {
    matrix_t mat1 = {};
    matrix_t mat2 = {};
    s21_create_matrix(2, 2, &mat1);
    s21_create_matrix(2, 2, &mat2);

    for(int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mat1.matrix[i][j] = i;
            mat1.matrix[i][j] = i + j;
        }
    }
    
    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 0);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_false_6) {
    matrix_t A = {}, B = {};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    ck_assert_int_eq(s21_eq_matrix(NULL, &B), FAILURE);
    ck_assert_int_eq(s21_eq_matrix(&A, NULL), FAILURE);
    ck_assert_int_eq(s21_eq_matrix(NULL, NULL), FAILURE);

    matrix_t C = {NULL, 0, 0};
    ck_assert_int_eq(s21_eq_matrix(&A, &C), FAILURE);

    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

Suite *equal_suite(void) {
    Suite *s;
    TCase *tc_eq;
    
    s = suite_create("Matrix");

    tc_eq = tcase_create("Equal");
    tcase_add_test(tc_eq, test_eq_matrix_ok1);
    tcase_add_test(tc_eq, test_eq_matrix_ok2);
    tcase_add_test(tc_eq, test_eq_matrix_ok3);
    tcase_add_test(tc_eq, test_eq_matrix_false_1);
    tcase_add_test(tc_eq, test_eq_matrix_false_2);
    tcase_add_test(tc_eq, test_eq_matrix_false_3);
    tcase_add_test(tc_eq, test_eq_matrix_false_4);
    tcase_add_test(tc_eq, test_eq_matrix_false_5);
    tcase_add_test(tc_eq, test_eq_matrix_false_6);
    suite_add_tcase(s, tc_eq);
    
    return s;
}

START_TEST(test_sum_matrix_ok_1) {
    matrix_t A = {}, B = {}, result;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    A.matrix[0][0] = 1.0; B.matrix[0][0] = 2.0;
    A.matrix[0][1] = 3.0; B.matrix[0][1] = 4.0;
    A.matrix[1][0] = 5.0; B.matrix[1][0] = 6.0;
    A.matrix[1][1] = 7.0; B.matrix[1][1] = 8.0;

    ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), S21_OK);
    ck_assert_double_eq(result.matrix[0][0], 3.0);
    ck_assert_double_eq(result.matrix[0][1], 7.0);
    ck_assert_double_eq(result.matrix[1][0], 11.0);
    ck_assert_double_eq(result.matrix[1][1], 15.0);

    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_ok_2) {
    matrix_t A = {}, B = {}, result;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    A.matrix[0][0] = 1.4; B.matrix[0][0] = 2.1;
    A.matrix[0][1] = -3.6; B.matrix[0][1] = 4.12;
    A.matrix[1][0] = 5.2; B.matrix[1][0] = 6.45;
    A.matrix[1][1] = 7.0; B.matrix[1][1] = -8.4;

    ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), S21_OK);
    ck_assert_double_eq(result.matrix[0][0], 3.5);
    ck_assert_double_eq(result.matrix[0][1], 0.52);
    ck_assert_double_eq(result.matrix[1][0], 11.65);
    ck_assert_double_le(result.matrix[1][1] - (-1.4), EPSILON);

    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_ok_3) {
    matrix_t A = {}, B = {}, result;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), S21_OK);
    ck_assert_double_eq(result.matrix[0][0], 0);
    ck_assert_double_eq(result.matrix[0][1], 0);
    ck_assert_double_eq(result.matrix[1][0], 0);
    ck_assert_double_le(result.matrix[1][1], 0);

    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_ok_4) {
    matrix_t A = {}, B = {}, result;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    A.matrix[0][0] = 1.4;
    A.matrix[0][1] = -3.6;
    A.matrix[1][0] = 5.2;
    A.matrix[1][1] = 7.0;

    ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), S21_OK);
    ck_assert_double_eq(result.matrix[0][0], 1.4);
    ck_assert_double_eq(result.matrix[0][1], -3.6);
    ck_assert_double_eq(result.matrix[1][0], 5.2);
    ck_assert_double_eq(result.matrix[1][1], 7.0);

    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_error_1) {
    matrix_t A = {}, B = {};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    A.matrix[0][0] = 1.4;
    A.matrix[0][1] = -3.6;
    A.matrix[1][0] = 5.2;
    A.matrix[1][1] = 7.0;

    ck_assert_int_eq(s21_sum_matrix(&A, &B, NULL), S21_INCORRECT_MATRIX);

    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_matrix_error_2) {
    matrix_t A = {}, result = {};
    s21_create_matrix(2, 2, &A);

    ck_assert_int_eq(s21_sum_matrix(&A, NULL, &result), S21_INCORRECT_MATRIX);

    s21_remove_matrix(&A);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_error_3) {
    matrix_t B = {}, result = {};
    s21_create_matrix(2, 2, &B);

    ck_assert_int_eq(s21_sum_matrix(NULL, &B, &result), S21_INCORRECT_MATRIX);

    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_error_4) {
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  B.matrix[3][3] = INFINITY;
  
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), S21_ERROR_CALCULATION);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_error_5) {
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 3, &B);
  
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), S21_INCORRECT_MATRIX);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

Suite *sum_suite(void) {
    Suite *s;
    TCase *tc_eq;
    
    s = suite_create("Matrix");

    tc_eq = tcase_create("Sum");
    tcase_add_test(tc_eq, test_sum_matrix_ok_1);
    tcase_add_test(tc_eq, test_sum_matrix_ok_2);
    tcase_add_test(tc_eq, test_sum_matrix_ok_3);
    tcase_add_test(tc_eq, test_sum_matrix_ok_4);
    tcase_add_test(tc_eq, test_sum_matrix_error_1);
    tcase_add_test(tc_eq, test_sum_matrix_error_2);
    tcase_add_test(tc_eq, test_sum_matrix_error_3);
    tcase_add_test(tc_eq, test_sum_matrix_error_4);
    tcase_add_test(tc_eq, test_sum_matrix_error_5);
    
    suite_add_tcase(s, tc_eq);
    
    return s;
}

START_TEST(test_sub_matrix_ok_1) {
    matrix_t A = {}, B = {}, result;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    A.matrix[0][0] = 1.0; B.matrix[0][0] = 2.0;
    A.matrix[0][1] = 3.0; B.matrix[0][1] = 4.0;
    A.matrix[1][0] = 5.0; B.matrix[1][0] = 6.0;
    A.matrix[1][1] = 7.0; B.matrix[1][1] = 8.0;

    ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), S21_OK);
    ck_assert_double_eq(result.matrix[0][0], -1.0);
    ck_assert_double_eq(result.matrix[0][1], -1.0);
    ck_assert_double_eq(result.matrix[1][0], -1.0);
    ck_assert_double_eq(result.matrix[1][1], -1.0);

    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_ok_2) {
    matrix_t A = {}, B = {}, result;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    A.matrix[0][0] = 1.4; B.matrix[0][0] = 2.1;
    A.matrix[0][1] = -3.6; B.matrix[0][1] = 4.12;
    A.matrix[1][0] = 5.2; B.matrix[1][0] = 6.45;
    A.matrix[1][1] = 7.0; B.matrix[1][1] = -8.4;

    ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), S21_OK);
    ck_assert_double_eq(result.matrix[0][0], -0.7);
    ck_assert_double_eq(result.matrix[0][1], -7.72);
    ck_assert_double_eq(result.matrix[1][0], -2.75);
    ck_assert_double_le(result.matrix[1][1], 15.4);

    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_ok_3) {
    matrix_t A = {}, B = {}, result;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), S21_OK);
    ck_assert_double_eq(result.matrix[0][0], 0);
    ck_assert_double_eq(result.matrix[0][1], 0);
    ck_assert_double_eq(result.matrix[1][0], 0);
    ck_assert_double_le(result.matrix[1][1], 0);

    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_ok_4) {
    matrix_t A = {}, B = {}, result;
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    A.matrix[0][0] = 1.4;
    A.matrix[0][1] = -3.6;
    A.matrix[1][0] = 5.2;
    A.matrix[1][1] = 7.0;

    ck_assert_int_eq(s21_sub_matrix(&B, &A, &result), S21_OK);
    ck_assert_double_eq(result.matrix[0][0], -1.4);
    ck_assert_double_eq(result.matrix[0][1], 3.6);
    ck_assert_double_eq(result.matrix[1][0], -5.2);
    ck_assert_double_le(result.matrix[1][1], -7.0);

    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_error_1) {
    matrix_t A = {}, B = {};
    s21_create_matrix(2, 2, &A);
    s21_create_matrix(2, 2, &B);

    A.matrix[0][0] = 1.4;
    A.matrix[0][1] = -3.6;
    A.matrix[1][0] = 5.2;
    A.matrix[1][1] = 7.0;

    ck_assert_int_eq(s21_sub_matrix(&A, &B, NULL), S21_INCORRECT_MATRIX);

    s21_remove_matrix(&A);
    s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sub_matrix_error_2) {
    matrix_t A = {}, result = {};
    s21_create_matrix(2, 2, &A);

    ck_assert_int_eq(s21_sub_matrix(&A, NULL, &result), S21_INCORRECT_MATRIX);

    s21_remove_matrix(&A);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_error_3) {
    matrix_t B = {}, result = {};
    s21_create_matrix(2, 2, &B);

    ck_assert_int_eq(s21_sub_matrix(NULL, &B, &result), S21_INCORRECT_MATRIX);

    s21_remove_matrix(&B);
    s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_error_4) {
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  B.matrix[3][3] = INFINITY;
  
  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), S21_ERROR_CALCULATION);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_error_5) {
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 3, &B);
  
  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), S21_INCORRECT_MATRIX);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

Suite *sub_suite(void) {
    Suite *s;
    TCase *tc_eq;
    
    s = suite_create("Matrix");

    tc_eq = tcase_create("Sub");
    tcase_add_test(tc_eq, test_sub_matrix_ok_1);
    tcase_add_test(tc_eq, test_sub_matrix_ok_2);
    tcase_add_test(tc_eq, test_sub_matrix_ok_3);
    tcase_add_test(tc_eq, test_sub_matrix_ok_4);
    tcase_add_test(tc_eq, test_sub_matrix_error_1);
    tcase_add_test(tc_eq, test_sub_matrix_error_2);
    tcase_add_test(tc_eq, test_sub_matrix_error_3);
    tcase_add_test(tc_eq, test_sub_matrix_error_4);
    tcase_add_test(tc_eq, test_sub_matrix_error_5);
    
    suite_add_tcase(s, tc_eq);
    
    return s;
}

START_TEST(test_mult_number_error_1) {
  matrix_t A = {};
  double number = 3.14;
  s21_create_matrix(3, 3, &A);

  ck_assert_int_eq(s21_mult_number(&A, number, NULL), S21_INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_mult_number_error_2) {
  matrix_t A = {};
  matrix_t result = {};
  double number = INFINITY;
  s21_create_matrix(3, 3, &A);
  s21_fill_matrix(&A, 1);
  
  ck_assert_int_eq(s21_mult_number(&A, number, &result), S21_ERROR_CALCULATION);
  
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_mult_number_error_3) {
  matrix_t A = {};
  matrix_t result = {};
  double number = 3;
  s21_create_matrix(3, 3, &A);
  s21_fill_matrix(&A, 5);
  A.matrix[2][2] = INFINITY;

  ck_assert_int_eq(s21_mult_number(&A, number, &result), S21_ERROR_CALCULATION);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_ok_1) {
  matrix_t A = {};
  matrix_t result = {};
  matrix_t expected = {};
  double number = 3;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  ck_assert_int_eq(s21_mult_number(&A, number, &result), S21_OK);
  ck_assert_int_eq(s21_expected(&result, &expected), SUCCESS);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_number_ok_2) {
  matrix_t A = {};
  matrix_t result = {};
  matrix_t expected = {};
  double number = 3;
  s21_create_matrix(3, 3, &A);
  s21_fill_matrix(&A, 2);
  s21_create_matrix(3, 3, &expected);
  s21_fill_matrix(&expected, 6);
  
  ck_assert_int_eq(s21_mult_number(&A, number, &result), S21_OK);
  ck_assert_int_eq(s21_expected(&result, &expected), SUCCESS);
 
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_number_ok_3) {
  matrix_t A = {};
  matrix_t result = {};
  matrix_t expected = {};
  double number = 2;
  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1, A.matrix[0][1] = 2, A.matrix[0][2] = 3;
  A.matrix[1][0] = 0, A.matrix[1][1] = 4, A.matrix[1][2] = 2;
  A.matrix[2][0] = 2, A.matrix[2][1] = 3, A.matrix[2][2] = 4;
  s21_create_matrix(3, 3, &expected);
  expected.matrix[0][0] = 2, expected.matrix[0][1] = 4,
  expected.matrix[0][2] = 6;
  expected.matrix[1][0] = 0, expected.matrix[1][1] = 8,
  expected.matrix[1][2] = 4;
  expected.matrix[2][0] = 4, expected.matrix[2][1] = 6,
  expected.matrix[2][2] = 8;
  
  ck_assert_int_eq(s21_mult_number(&A, number, &result), S21_OK);
  ck_assert_int_eq(s21_expected(&result, &expected), SUCCESS);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

Suite *mult_num_suite(void) {
    Suite *s;
    TCase *tc_eq;
    
    s = suite_create("Matrix");

    tc_eq = tcase_create("MultNum");
    tcase_add_test(tc_eq, test_mult_number_ok_1);
    tcase_add_test(tc_eq, test_mult_number_ok_2);
    tcase_add_test(tc_eq, test_mult_number_ok_3);
    tcase_add_test(tc_eq, test_mult_number_error_1);
    tcase_add_test(tc_eq, test_mult_number_error_2);

    suite_add_tcase(s, tc_eq);
    
    return s;
}

START_TEST(test_mult_matrix_error_1) {
  matrix_t A = {};
  s21_create_matrix(3, 3, &A);
  s21_init_matrix(&A, 1, 1);

  ck_assert_int_eq(s21_mult_matrix(&A, NULL, NULL), S21_INCORRECT_MATRIX);
  
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_mult_matrix_error_2) {
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  s21_fill_matrix(&A, 1);
  B.matrix[2][2] = INFINITY;
  
  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), S21_ERROR_CALCULATION);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_error_3) {
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  s21_create_matrix(3, 2, &A);
  s21_create_matrix(3, 2, &B);
  
  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), S21_ERROR_CALCULATION);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_matrix_ok_1) {
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_t expected = {};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  s21_create_matrix(3, 3, &expected);
  
  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), S21_OK);
  ck_assert_int_eq(s21_expected(&result, &expected), SUCCESS);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_matrix_ok_2) {
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_t expected = {};
  s21_create_matrix(3, 2, &A);
  A.matrix[0][0] = 1, A.matrix[0][1] = 4;
  A.matrix[1][0] = 2, A.matrix[1][1] = 5;
  A.matrix[2][0] = 3, A.matrix[2][1] = 6;
  s21_create_matrix(2, 3, &B);
  B.matrix[0][0] = 1, B.matrix[0][1] = -1, B.matrix[0][2] = 1;
  B.matrix[1][0] = 2, B.matrix[1][1] = 3, B.matrix[1][2] = 4;
  s21_create_matrix(3, 3, &expected);
  expected.matrix[0][0] = 9, expected.matrix[0][1] = 11,
  expected.matrix[0][2] = 17;
  expected.matrix[1][0] = 12, expected.matrix[1][1] = 13,
  expected.matrix[1][2] = 22;
  expected.matrix[2][0] = 15, expected.matrix[2][1] = 15,
  expected.matrix[2][2] = 27;
  
  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), S21_OK);
  ck_assert_int_eq(s21_expected(&result, &expected), SUCCESS);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

Suite *mult_matr_suite(void) {
    Suite *s;
    TCase *tc_eq;
    
    s = suite_create("Matrix");

    tc_eq = tcase_create("MultMatr");
    tcase_add_test(tc_eq, test_mult_matrix_ok_1);
    tcase_add_test(tc_eq, test_mult_matrix_ok_2);
    tcase_add_test(tc_eq, test_mult_matrix_error_1);
    tcase_add_test(tc_eq, test_mult_matrix_error_2);
    tcase_add_test(tc_eq, test_mult_matrix_error_3);

    suite_add_tcase(s, tc_eq);
    
    return s;
}

START_TEST(test_transpose_error_1) {
  matrix_t A = {};
  matrix_t result = {};
  ck_assert_int_eq(s21_transpose(&A, &result), S21_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_transpose_ok_1) {
  matrix_t A = {};
  matrix_t result = {};
  s21_create_matrix(3, 2, &A);

  ck_assert_int_eq(s21_transpose(&A, &result), S21_OK);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 3);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_transpose_ok_2) {
  matrix_t A = {};
  matrix_t result = {};
  s21_create_matrix(2, 3, &A);
  
  ck_assert_int_eq(s21_transpose(&A, &result), S21_OK);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 2);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_transpose_ok_3) {
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &eq_matrix);
  s21_init_matrix(&A, 1, 1);
  
  ck_assert_int_eq(s21_transpose(&A, &result), S21_OK);
  eq_matrix.matrix[0][0] = 1, eq_matrix.matrix[0][1] = 4;
  eq_matrix.matrix[1][0] = 2, eq_matrix.matrix[1][1] = 5;
  eq_matrix.matrix[2][0] = 3, eq_matrix.matrix[2][1] = 6;
  ck_assert_int_eq(s21_eq_matrix(&result, &eq_matrix), SUCCESS);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 2);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&eq_matrix);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_transpose_ok_4) {
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  s21_create_matrix(3, 2, &A);
  s21_create_matrix(2, 3, &eq_matrix);
  A.matrix[0][0] = 1, A.matrix[0][1] = 4;
  A.matrix[1][0] = 2, A.matrix[1][1] = 5;
  A.matrix[2][0] = 3, A.matrix[2][1] = 6;
  
  ck_assert_int_eq(s21_transpose(&A, &result), S21_OK);
  eq_matrix.matrix[0][0] = 1, eq_matrix.matrix[0][1] = 2,
  eq_matrix.matrix[0][2] = 3;
  eq_matrix.matrix[1][0] = 4, eq_matrix.matrix[1][1] = 5,
  eq_matrix.matrix[1][2] = 6;
  ck_assert_int_eq(s21_eq_matrix(&result, &eq_matrix), SUCCESS);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 3);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&eq_matrix);
  s21_remove_matrix(&result);
}
END_TEST

Suite *transpose_suite(void) {
  Suite *suite = suite_create("Matrix");
  TCase *tc_core = tcase_create("transpose");
  tcase_add_test(tc_core, test_transpose_error_1);
  tcase_add_test(tc_core, test_transpose_ok_1);
  tcase_add_test(tc_core, test_transpose_ok_2);
  tcase_add_test(tc_core, test_transpose_ok_3);
  tcase_add_test(tc_core, test_transpose_ok_4);
  suite_add_tcase(suite, tc_core);

  return suite;
}

START_TEST(test_calc_complements_error_1) {
  matrix_t A = {};

  s21_create_matrix(3, 3, &A);
  
  ck_assert_int_eq(s21_calc_complements(&A, NULL), S21_INCORRECT_MATRIX);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_error_2) {
  matrix_t A = {};
  matrix_t result = {};
  s21_create_matrix(1, 3, &A);
  s21_init_matrix(&A, 1, 3);
  
  ck_assert_int_eq(s21_calc_complements(&A, &result), S21_ERROR_CALCULATION);
  
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_ok_1) {
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &eq_matrix);
  A.matrix[0][0] = 1, A.matrix[0][1] = 2, A.matrix[0][2] = 3;
  A.matrix[1][0] = 0, A.matrix[1][1] = 4, A.matrix[1][2] = 2;
  A.matrix[2][0] = 5, A.matrix[2][1] = 2, A.matrix[2][2] = 1;
  
  ck_assert_int_eq(s21_calc_complements(&A, &result), S21_OK);
  eq_matrix.matrix[0][0] = 0, eq_matrix.matrix[0][1] = 10,
  eq_matrix.matrix[0][2] = -20;
  eq_matrix.matrix[1][0] = 4, eq_matrix.matrix[1][1] = -14,
  eq_matrix.matrix[1][2] = 8;
  eq_matrix.matrix[2][0] = -8, eq_matrix.matrix[2][1] = -2,
  eq_matrix.matrix[2][2] = 4;
  ck_assert_int_eq(s21_eq_matrix(&result, &eq_matrix), SUCCESS);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&eq_matrix);
}
END_TEST

Suite *calc_comp_suite(void) {
  Suite *suite = suite_create("Matrix");
  TCase *tc_core = tcase_create("calc_comp");
  tcase_add_test(tc_core, test_calc_complements_error_1);
  tcase_add_test(tc_core, test_calc_complements_error_2);
  tcase_add_test(tc_core, test_calc_complements_ok_1);
  suite_add_tcase(suite, tc_core);

  return suite;
}


START_TEST(test_determinant_error_1) {
  matrix_t A = {};
  double det = 0;
  ck_assert_int_eq(s21_determinant(&A, &det), S21_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_determinant_error_2) {
  matrix_t A = {};
  double det = 0;
  s21_create_matrix(3, 2, &A);
  s21_init_matrix(&A, 1, 1);
  
  ck_assert_int_eq(s21_determinant(&A, &det), S21_ERROR_CALCULATION);
  
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_ok_1) {
  matrix_t A = {};
  double det = 0;
  s21_create_matrix(3, 3, &A);
  s21_init_matrix(&A, 1, 1);
  
  ck_assert_int_eq(s21_determinant(&A, &det), S21_OK);
  ck_assert_double_eq(det, 0);
  
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_ok_2) {
  matrix_t A = {};
  double det = 0;
  s21_create_matrix(2, 2, &A);
  s21_init_matrix(&A, 3, 3);

  ck_assert_int_eq(s21_determinant(&A, &det), S21_OK);
  ck_assert_double_eq(det, -18);
  
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_ok_3) {
  matrix_t A = {};
  double det = 0;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 21;
  
  ck_assert_int_eq(s21_determinant(&A, &det), S21_OK);
  ck_assert_double_eq(det, 21);
  
  s21_remove_matrix(&A);
}
END_TEST

Suite *det_suite(void) {
  Suite *suite = suite_create("Matrix");
  TCase *tc_core = tcase_create("det");
  tcase_add_test(tc_core, test_determinant_error_1);
  tcase_add_test(tc_core, test_determinant_error_2);
  tcase_add_test(tc_core, test_determinant_ok_1);
  tcase_add_test(tc_core, test_determinant_ok_2);
  tcase_add_test(tc_core, test_determinant_ok_3);
  suite_add_tcase(suite, tc_core);

  return suite;
}


START_TEST(test_inverse_matrix_error_1) {
  matrix_t A = {};
  matrix_t result = {};
  
  ck_assert_int_eq(s21_inverse_matrix(&A, &result), S21_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_inverse_matrix_error_2) {
  matrix_t A = {};
  matrix_t result = {};
  s21_create_matrix(3, 3, &A);
  s21_init_matrix(&A, 1, 1);
  
  ck_assert_int_eq(s21_inverse_matrix(&A, &result), S21_ERROR_CALCULATION);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix_error_3) {
  matrix_t A = {};
  matrix_t result = {};
  s21_create_matrix(5, 3, &A);
  s21_init_matrix(&A, 1, 1);
  
  ck_assert_int_eq(s21_inverse_matrix(&A, &result), S21_ERROR_CALCULATION);
  
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix_ok_1) {
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &eq_matrix);
  A.matrix[0][0] = 21;
  eq_matrix.matrix[0][0] = 1.0 / 21.0;

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), S21_OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &eq_matrix), SUCCESS);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&eq_matrix);
}
END_TEST

START_TEST(test_inverse_matrix_ok_2) {
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &eq_matrix);
  A.matrix[0][0] = 2, A.matrix[0][1] = 5, A.matrix[0][2] = 7;
  A.matrix[1][0] = 6, A.matrix[1][1] = 3, A.matrix[1][2] = 4;
  A.matrix[2][0] = 5, A.matrix[2][1] = -2, A.matrix[2][2] = -3;
  eq_matrix.matrix[0][0] = 1, eq_matrix.matrix[0][1] = -1,
  eq_matrix.matrix[0][2] = 1;
  eq_matrix.matrix[1][0] = -38, eq_matrix.matrix[1][1] = 41,
  eq_matrix.matrix[1][2] = -34;
  eq_matrix.matrix[2][0] = 27, eq_matrix.matrix[2][1] = -29,
  eq_matrix.matrix[2][2] = 24;
  
  ck_assert_int_eq(s21_inverse_matrix(&A, &result), S21_OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &eq_matrix), SUCCESS);
  
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&eq_matrix);
}
END_TEST

Suite *inv_suite(void) {
  Suite *suite = suite_create("Matrix");
  TCase *tc_core = tcase_create("inverse");
  tcase_add_test(tc_core, test_inverse_matrix_error_1);
  tcase_add_test(tc_core, test_inverse_matrix_error_2);
  tcase_add_test(tc_core, test_inverse_matrix_error_3);
  tcase_add_test(tc_core, test_inverse_matrix_ok_1);
  tcase_add_test(tc_core, test_inverse_matrix_ok_2);
  suite_add_tcase(suite, tc_core);

  return suite;
}

int main() {
  int number_failed;
  Suite *suite = NULL;
  SRunner *runner = srunner_create(suite);
  Suite *suits_list[] = {create_and_remove_suite(),
                         equal_suite(),
                         sum_suite(),
                         sub_suite(),
                         mult_num_suite(),
                         mult_matr_suite(),
                         transpose_suite(),
                         calc_comp_suite(),
                         det_suite(),
                         inv_suite(),
                         NULL};

  for (Suite **current = suits_list; *current != NULL; current++)
    srunner_add_suite(runner, *current);

  srunner_run_all(runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

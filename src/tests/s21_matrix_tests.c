#include <check.h>
#include <stdlib.h>
#include "../s21_matrix.h"

START_TEST(test_create_matrix_ok) {
    matrix_t mat;
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
    matrix_t mat;
    
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
    matrix_t mat;
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
    matrix_t mat1;
    matrix_t mat2;
    s21_create_matrix(2, 2, &mat1);
    s21_create_matrix(2, 2, &mat2);
    
    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 1);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_ok2) {
    matrix_t mat1;
    matrix_t mat2;
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
    matrix_t mat1;
    matrix_t mat2;
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
    matrix_t mat1, mat2;
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
    matrix_t mat1;
    matrix_t mat2;
    s21_create_matrix(2, 3, &mat1);
    s21_create_matrix(3, 2, &mat2);

    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 0);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_false_3) {
    matrix_t mat1;
    matrix_t mat2;
    s21_create_matrix(2, 2, &mat1);
    s21_create_matrix(3, 2, &mat2);

    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 0);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_false_4) {
    matrix_t mat1;
    matrix_t mat2;
    s21_create_matrix(2, 2, &mat1);
    s21_create_matrix(2, 2, &mat2);

    mat1.matrix[1][1] = 1;

    ck_assert_int_eq(s21_eq_matrix(&mat1, &mat2), 0);

    s21_remove_matrix(&mat1);
    s21_remove_matrix(&mat2);
}
END_TEST

START_TEST(test_eq_matrix_false_5) {
    matrix_t mat1;
    matrix_t mat2;
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
    matrix_t A, B;
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

int main() {
  int number_failed;
  Suite *suite = NULL;
  SRunner *runner = srunner_create(suite);
  Suite *suits_list[] = {create_and_remove_suite(),
                         equal_suite(),
                         NULL};

  for (Suite **current = suits_list; *current != NULL; current++)
    srunner_add_suite(runner, *current);

  srunner_run_all(runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

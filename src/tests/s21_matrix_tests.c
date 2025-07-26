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

int main() {
  int number_failed;
  Suite *suite = NULL;
  SRunner *runner = srunner_create(suite);
  Suite *suits_list[] = {create_and_remove_suite,
                         NULL};

  for (Suite **current = suits_list; *current != NULL; current++)
    srunner_add_suite(runner, *current);

  srunner_run_all(runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

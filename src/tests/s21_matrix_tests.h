#ifndef TEST_H
#define TEST_H

#include <check.h>
#include "s21_matrix.h"

Suite *create_and_remove_suite(void);
Suite *equal_suite(void);
Suite *sum_suite(void);
Suite *mult_num_suite(void);
Suite *mult_matr_suite(void);
Suite *transpose_suite(void);
Suite *calc_comp_suite(void);
Suite *det_suite(void);
Suite *inv_suite(void);


#endif
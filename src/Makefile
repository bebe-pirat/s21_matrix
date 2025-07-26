CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
TFLAGS = -lcheck -lsubunit -lm
GFLAGS = -fprofile-arcs -ftest-coverage
LIB_NAME = s21_matrix.a
LIB_SRC = $(wildcard *.c)
LIB_OBJ = $(LIB_SRC:.c=.o)
TEST_SRC = $(wildcard ./tests/*.c)

all: clang $(LIB_NAME)

s21_matrix.a: clean
	$(CC) $(CFLAGS) -c $(LIB_SRC)
	ar rcs $(LIB_NAME) $(LIB_OBJ)
	ranlib $(LIB_NAME)
	rm -f $(LIB_OBJ)

test: $(LIB_NAME)
	$(CC) $(CFLAGS) -g $(TEST_SRC) $(TFLAGS) $(LIB_NAME) -o test
	./test

gcov_report: clean $(LIB_NAME)
	$(CC) $(CFLAGS) $(TEST_SRC) $(TFLAGS) $(GFLAGS) $(LIB_SRC) -o gcov_report
	./gcov_report
	lcov -t "gcovreport" -o gcovreport.info -c -d .
	genhtml -o report gcovreport.info
	rm -rf *.gcda *.gcno
	open ./report/index.html

clang:
	clang-format -i $(TEST_SRC) $(LIB_SRC) *.h ./tests/*.h

valgrind: test
	valgrind --tool=memcheck --leak-check=yes ./test

check: clang valgrind

clean:
	rm -f *.o *.a test gcov_report *.gcno *.gcda *.info
	rm -rf report/

rebuild: clean all

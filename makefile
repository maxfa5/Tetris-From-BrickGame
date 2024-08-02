CC = gcc
CFLAGS = -Wall -Wextra  -std=c11 -Wpedantic
SRC = ./brick_game/tetris/tetris.c
OBJ = s21_*.o
LIB = tetris.a
LIBSFLAGS = -lcheck -lsubunit -lrt -lpthread -lm -lgcov -lncurses 
TEST_SRC = ./brick_game/tetris/tests/test.c
GCOV_FLAGS = -fprofile-arcs -ftest-coverage  --coverage
GCOVR_PATH =~/.local/bin/gcovr
REPORT = ./report/index.html
DIST_DIR = dist
VALGIND_FLAGS = --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all -s

# Default target when no arguments are given to make
.PHONY: all  $(LIB) clang install uninstall gcov_report div clean dist

all: clean install

$(LIB):
	gcc -c $(SRC)
	ar rc $(LIB) *.o
	ranlib $(LIB) 
	rm -rf *.o

all: clean $(LIB)

clang_check:
	clang-format -n   ./brick_game/tetris/*.c ./brick_game/tetris/*.h  ./brick_game/tetris/tests/*.c ./brick_game/tetris/tests/*.h ./gui/cli/main.c

clang_i:
	clang-format -i  ./brick_game/tetris/*.c ./brick_game/tetris/*.h  ./brick_game/tetris/tests/*.c ./brick_game/tetris/tests/*.h ./gui/cli/main.c


install: ${LIB}
	$(CC) $(CFLAGS) -L. ./gui/cli/main.c -o tetris $(LIBSFLAGS) tetris.a
	./tetris

test: clean $(LIB) 
	$(CC) $(CFLAGS) -L. -l:$(LIB) $(GCOV_FLAGS) $(SRC) ${TEST_SRC} -o test_result $(LIBSFLAGS) -g 
	./test_result

gcov_report: uninstall test
	mkdir report
	$(CC) $(CFLAGS) $(LIB) $(GCOV_FLAGS) $(SRC) ${TEST_SRC} -o $(REPORT) $(LIBSFLAGS) -e ${TEST_SRC}

	~/.local/bin/gcovr -r . --html --html-details -o $(REPORT) -e brick_game/tetris/tests/test.c
	xdg-open $(REPORT)

uninstall:
	rm -rf tetris

dvi:
	doxygen

dist: clean install
	mkdir -p $(DIST_DIR)
	gzip tetris  
	mv tetris.gz $(DIST_DIR)/

clean: uninstall
	rm -rf scores.txt
	rm -rf *.o
	rm -rf dist
	rm -rf test_result
	rm -rf test2_result
	rm -rf doxygen
	rm -rf *.a
	rm -rf *.gcda *.gcdo *.gcov *.css *.html *.gcno
	rm -rf report

val2: clean test
	$(CC) $(CFLAGS) -L. -l:$(LIB) $(GCOV_FLAGS) $(SRC) ./brick_game/tetris/tests/test2.c -o test2_result $(LIBSFLAGS) -g 
	valgrind $(VALGIND_FLAGS)  ./test2_result

val: clean install
	$(CC) $(CFLAGS) -L. -l:$(LIB) $(GCOV_FLAGS) $(SRC) $(TEST_SRC) -o test_result $(LIBSFLAGS) -g 
	valgrind $(VALGIND_FLAGS)  ./tetris
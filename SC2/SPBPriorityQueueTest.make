CC = gcc
OBJS = sp_bpqueue_unit_test.o SPBPriorityQueue.o
EXEC = sp_bpqueue_unit_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
sp_bpqueue_unit_test.o: $(TESTS_DIR)/sp_bpqueue_unit_test.c $(TESTS_DIR)/unit_test_util.h SPBPriorityQueue.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
SPBPriorityQueue.o: SPBPriorityQueue.c SPBPriorityQueue.h
	$(CC) $(COMP_FLAG) -c $*.c	
clean:
	rm -f $(OBJS) $(EXEC)

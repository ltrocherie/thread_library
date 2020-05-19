LIBS_DIR = $(OUT_DIR)/lib
OUT_DIR = ../install
SRC_DIR = .
TESTS_DIR = ../tests
LIBS = -lthread -lpthread
CFLAGS = -Wall -g -L $(LIBS_DIR) $(LIBS) -I..

#INTERNAL USE

install_library: thread.o
	ar rcs $(OUT_DIR)/lib/libthread.a thread.o
	rm thread.o

#DO NOT TOUCH THESE

thread.o: thread.c
	gcc $(CFLAGS)  -c $^ -o $@

thread.o_thread: thread.c
	gcc $(CFLAGS) -DUSE_PTHREAD -c $^ -o $@

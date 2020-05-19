#DIRECTORIES
OUT_DIR = ../install
LIBS_DIR = $(OUT_DIR)/lib
SRC_DIR = ../src
TESTS_DIR = .

#LIBRARIES
LIBS = -lthread
LIBS_PTHREAD = -lpthread

#FLAGS
CFLAGS = -Wall -g -L$(LIBS_DIR) $(LIBS) -I$(SRC_DIR)
CFLAGS_PTHREAD = -Wall -g -I$(SRC_DIR) -DUSE_PTHREAD

#FILES
BIN_TO_DO = 01-main\
02-switch\
11-join\
12-join-main\
21-create-many\
22-create-many-recursive\
23-create-many-once\
31-switch-many\
32-switch-many-join\
33-switch-many-cascade\
51-fibonacci\
61-mutex\
62-mutex

BIN_WITHOUT_ARGS = 01-main\
02-switch\
11-join\
12-join-main

THREAD_NB = 5
THREAD_NB_BIS = 20
YIELD_NB = 5
FIBO_NB = 10


install_tests:
	$(foreach f,$(BIN_TO_DO), gcc $(f).c $(CFLAGS) -o $(OUT_DIR)/bin/$(f);)

check:
	$(foreach file,$(BIN_WITHOUT_ARGS),$(OUT_DIR)/bin/$(file);)
	../install/bin/21-create-many $(THREAD_NB)
	../install/bin/22-create-many-recursive $(THREAD_NB)
	../install/bin/23-create-many-once $(THREAD_NB)
	../install/bin/31-switch-many $(THREAD_NB) $(YIELD_NB)
	../install/bin/32-switch-many-join $(THREAD_NB) $(YIELD_NB)
	../install/bin/33-switch-many-cascade $(THREAD_NB_BIS) $(YIELD_NB)
	../install/bin/51-fibonacci $(FIBO_NB)
	../install/bin/61-mutex $(THREAD_NB_BIS)
	../install/bin/62-mutex $(THREAD_NB_BIS)

install_tests_pthread:
	$(foreach f,$(BIN_TO_DO), gcc $(CFLAGS_PTHREAD) -o $(OUT_DIR)/bin/$(f) $(f).c $(LIBS_PTHREAD);)

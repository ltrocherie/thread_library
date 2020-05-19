OUT_DIR = ../install
LIBS_DIR = $(OUT_DIR)/lib
SRC_DIR = .
TESTS_DIR = tests
LIBS = -lthread
CFLAGS = -Wall -g -L$(LIBS_DIR) $(LIBS) -I..

BIN_TO_DO= 01-creation 02-main_exit 03-creation_and_destruction 04-several_threads 05-thread_join 21-large-array-sum 31-preemption

all: $(BIN_TO_DO)

%: %.c
	cc $< $(CFLAGS) -o $(OUT_DIR)/custom_tests_bin/$(basename $<)

check: $(BIN_TO_DO)
	$(foreach f,$^,$(OUT_DIR)/custom_tests_bin/$(f);)

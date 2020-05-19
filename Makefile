SRC_DIR=src
INST_DIR= install
TST_DIR=tests
LIBS_DIR = $(INST_DIR)/lib
TST_BIN_DIR = $(INST_DIR)bin/
GRP_DIR = graphs
CTST_DIR=custom_tests/
VALGRIND_FLAGS = -q

#PUBLIC USE

make: install build

build: install_library install_tests install_customs

install: create_dir

check_customs:
	$(MAKE) check -C $(CTST_DIR)

check: install_tests
	$(MAKE) check -C $(TST_DIR)

valgrind: install
	$(foreach f,$(shell ls install/bin/), valgrind $(VALGRIND_FLAGS) ./install/bin/$f;)

pthreads: install_tests_pthread
	$(MAKE) check -C $(TST_DIR)

graphs: make
	(cd $(GRP_DIR) && ./recuperation.sh)
	(cd $(GRP_DIR) && python3 ./graph.py)

# INTERNAL USE, SHOULD NOT BE CALLED DIRECTLY

create_dir:
		mkdir -p $(INST_DIR) && mkdir -p $(INST_DIR)/bin && mkdir -p $(LIBS_DIR) && mkdir -p $(INST_DIR)/custom_tests_bin

install_library:
	$(MAKE) install_library -C $(SRC_DIR)

install_library_pthread:
	$(MAKE) install_library_pthread -C $(SRC_DIR)

install_tests:
	$(MAKE) install_tests -C $(TST_DIR)

install_customs:
	$(MAKE) -C $(CTST_DIR)

install_tests_pthread:
	$(MAKE) install_tests_pthread -C $(TST_DIR)

clean:
	rm -r $(INST_DIR)*

CPP_FLAGS = -std=c++17 -O2
CURR_DIR = $(shell pwd)
PARENT_DIR = $(abspath $(CURR_DIR)/..)

GK_SRC_PATH = $(CURR_DIR)/gk.cpp
QDIG_SRC_PATH = $(CURR_DIR)/qdigest.cpp

GK_BIN_PATH = $(PARENT_DIR)/bin/gk
QDIG_BIN_PATH = $(PARENT_DIR)/bin/qdig


default:
	cat Makefile

build-gk: 
	mkdir $(PARENT_DIR)/bin; \
	g++ $(CPP_FLAGS) $(GK_SRC_PATH) -o $(GK_BIN_PATH);

build-qdig:
	mkdir $(PARENT_DIR)/bin; \
	g++ $(CPP_FLAGS) $(QDIG_SRC_PATH) -o $(QDIG_BIN_PATH); \

build: 
	mkdir $(PARENT_DIR)/bin; \
	make build-gk; \
	make build-qdig; \

install: build
	sudo cp $(GK_BIN_PATH) /usr/bin/gk; \
	sudo cp $(QDIG_BIN_PATH) /usr/bin/qdig; \

uninstall:
	sudo rm /usr/bin/gk; \
	sudo rm /usr/bin/qdig; \
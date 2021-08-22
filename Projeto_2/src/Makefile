CPP_FLAGS = -std=c++17 -O2
CURR_DIR = $(shell pwd)
PARENT_DIR = $(abspath $(CURR_DIR)/..)

GKS_SRC_PATH = $(CURR_DIR)/gks.cpp
QDIG_SRC_PATH = $(CURR_DIR)/QDigestSketch.cpp

GKS_BIN_PATH = $(PARENT_DIR)/bin/gks
QDIG_BIN_PATH = $(PARENT_DIR)/bin/qdig


default:
	cat Makefile

build-gks:
	g++ $(CPP_FLAGS) $(GKS_SRC_PATH) -o $(GKS_BIN_PATH);

build-qdig:
	g++ $(CPP_FLAGS) $(QDIG_SRC_PATH) -o $(QDIG_BIN_PATH); \

build: 
	mkdir $(PARENT_DIR)/bin ; \
	make build-gks; \
	make build-qdig

install: build
	sudo cp $(GKS_BIN_PATH) /usr/bin/gks; \
	sudo cp $(QDIG_BIN_PATH) /usr/bin/qdig;

uninstall:
	sudo rm /usr/bin/gks; \
	sudo rm /usr/bin/qdig;
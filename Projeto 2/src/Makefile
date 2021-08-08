CPP_FLAGS = -std=c++17 -O2
CURR_DIR = $(shell pwd)
PARENT_DIR = $(abspath $(CURR_DIR)/..)

KLL_SRC_PATH = $(CURR_DIR)/kll.cpp
KMV_SRC_PATH = $(CURR_DIR)/kmv.cpp

KLL_BIN_PATH = $(PARENT_DIR)/bin/kll
KMV_BIN_PATH = $(PARENT_DIR)/bin/kmv

KLL_HELP_PATH = $(CURR_DIR)/helpkll.txt
KMV_HELP_PATH = $(CURR_DIR)/helpkmv.txt


default:
	cat Makefile

build-kll:
	g++ $(CPP_FLAGS) $(KLL_SRC_PATH) -o $(KLL_BIN_PATH)

#build-kmv:
#	g++ $(CPP_FLAGS) $(KMV_SRC_PATH) -o $(KMV_BIN_PATH)

create-bin-folder:
	mkdir $(PARENT_DIR)/bin

build:
	make build-kll
#	make build-kmv
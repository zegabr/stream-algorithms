CPP_FLAGS = -std=c++17 -O2
CURR_DIR = $(shell pwd)
PARENT_DIR = $(abspath $(CURR_DIR)/..)

CMIN_SRC_PATH = $(CURR_DIR)/cmin.cpp
KMV_SRC_PATH = $(CURR_DIR)/kmv.cpp

CMIN_BIN_PATH = $(PARENT_DIR)/bin/cmin
KMV_BIN_PATH = $(PARENT_DIR)/bin/kmv

CMIN_HELP_PATH = $(CURR_DIR)/helpcmin.txt
KMV_HELP_PATH = $(CURR_DIR)/helpkmv.txt


.SILENT:
default:
	cat Makefile

.SILENT:
build-cmin:
	g++ $(CPP_FLAGS) $(CMIN_SRC_PATH) -o $(CMIN_BIN_PATH)

.SILENT:
build-kmv:
	g++ $(CPP_FLAGS) $(KMV_SRC_PATH) -o $(KMV_BIN_PATH)

.SILENT:
build:
	mkdir $(PARENT_DIR)/bin ; \
	make build-cmin; \
	make build-kmv

install: build
	sudo cp $(CMIN_BIN_PATH) /usr/bin/cmin; \
	sudo cp $(KMV_BIN_PATH) /usr/bin/kmv; \
	sudo cp $(CMIN_HELP_PATH) /usr/bin/helpcmin.txt; \
	sudo cp $(KMV_HELP_PATH) /usr/bin/helpkmv.txt;
	
uninstall:
	sudo rm /usr/bin/cmin; \
	sudo rm /usr/bin/kmv; \
	sudo rm /usr/bin/helpcmin.txt; \
	sudo rm /usr/bin/helpkmv.txt;

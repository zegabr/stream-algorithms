CPP_FLAGS = -std=c++17 -O2
COUNT_MIN_PATH = ./src/cmin.cpp
COUNT_MIN_BIN_PATH = ./bin/cmin

default:
	cat Makefile

build:
	g++ $(CPP_FLAGS) $(COUNT_MIN_PATH) -o $(COUNT_MIN_BIN_PATH)

install: build
	# TODO
	

CPP_FLAGS = -std=c++17 -O2
CPP_EXTENDED_FLAGS = -std=c++17 -O2 -Wshadow -Wall -Wno-unused-result -g -fsanitize=address,undefined -D_GLIBCXX_DEBUG -Wno-unused-result -Wno-sign-compare -Wno-char-subscripts
COUNT_MIN_PATH = ./src/cmin.cpp
KMV_PATH = ./src/kmv.cpp
COUNT_MIN_BIN_PATH = ./bin/cmin
KMV_BIN_PATH = ./bin/kmv

default:
	cat Makefile

build-cmin:
	g++ $(CPP_FLAGS) $(COUNT_MIN_PATH) -o $(COUNT_MIN_BIN_PATH)

build-kmv:
	mkdir ./bin ; \
	touch ./bin/kmv ; \
	g++ $(CPP_FLAGS) $(KMV_PATH) -o $(KMV_BIN_PATH)

build-cmin-debug:
	g++ $(CPP_EXTENDED_FLAGS) $(COUNT_MIN_PATH) -o $(COUNT_MIN_BIN_PATH)

build-kmv-debug:
	g++ $(CPP_EXTENDED_FLAGS) $(KMV_PATH) -o $(KMV_BIN_PATH)

build:
	make build-cmin
	make build-kmv

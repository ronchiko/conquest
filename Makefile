
cc = g++

src_files = $(shell find src/ -name *.cpp)

all: $(src_files)
	$(cc) -g $(src_files) -I include -lncurses -o conquest

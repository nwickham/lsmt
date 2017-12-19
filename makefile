CC=g++

DEBUG=-g
WARN=-Wall -W
STD=-std=c++14
CFLAGS=$(WARN) $(DEBUG) $(STD) -c
LFLAGS=$(WARN) $(DEBUG)

BINDIR=./
NAME=main.app
EXE=$(BINDIR)$(NAME)
EXT=cpp
SRCS=$(shell find . -name "*.$(EXT)")
OBJS=$(SRCS:.cpp=.o)

.PHONY: clean

.DEFAULT: all

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $<

clean:
	$(RM) *.o $(EXE)

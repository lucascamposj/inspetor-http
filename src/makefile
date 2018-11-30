IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./obj
LDIR =../lib

LIBS=-lm

_DEPS = server.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = HtppInspector.o server.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: clean HtppInspector

HtppInspector: $(OBJ)
	gcc -std=c99  -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ tradutor

IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./obj
LDIR =../lib

LIBS=-lm

_DEPS = proxy.h parser.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = httpInspector.o proxy.o parser.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: clean httpInspector

httpInspector: $(OBJ)
	gcc -std=c99  -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ httpInspector

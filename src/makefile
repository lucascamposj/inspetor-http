IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./obj
LDIR =../lib

LIBS=-lm

_DEPS = proxy.h parser.h spider.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = aracne.o proxy.o parser.o spider.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: clearscreen clean aracne

aracne: $(OBJ)
	gcc -std=c99  -o $@ $^ $(CFLAGS) $(LIBS)

clearscreen:
	tput reset

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ aracne

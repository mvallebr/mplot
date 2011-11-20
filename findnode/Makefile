CC=colorgcc
PROGNAME=findnode
LDFLAGS=-L ../fconv -lfconv
CFLAGS=-Wall -O2 -march=i586 -mcpu=i586 -I. -I../fconv
OBJS= main.o 
LD_LIBRARY_PATH=../fconv

all: $(OBJS)
	$(CC) $(LDFLAGS)  $(OBJS) -o $(PROGNAME)

clean:
	rm -f *~ $(OBJS) $(PROGNAME)

test: all
	./$(PROGNAME) entrada.txt saida.txt



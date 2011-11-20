CC=colorgcc
PROGNAME=geraelipse
LDFLAGS=-L -lm
CFLAGS=-Wall -O2 -march=i586 -mcpu=i586 -I. 
OBJS= main.o 
LD_LIBRARY_PATH=

all: $(OBJS)
	$(CC) $(LDFLAGS)  $(OBJS) -o $(PROGNAME)

clean:
	rm -f *~ $(OBJS) $(PROGNAME)

test: all
	./$(PROGNAME) saida.txt



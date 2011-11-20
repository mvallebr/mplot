CC=colorgcc
PROGNAME=mesh2col
LDFLAGS=-L ../fconv -lfconv -L../mplotwin -lmplot $$(sdl-config --libs)
CFLAGS=-Wall -O2 -march=i586 -mcpu=i586 -I. -I../fconv -I../mplotwin $$(sdl-config --cflags)
OBJS= main.o 
LD_LIBRARY_PATH=../fconv:../mplotwin

all: $(OBJS)
	$(CC) $(LDFLAGS)  $(OBJS) -o $(PROGNAME)

clean:
	rm -f *~ $(OBJS) $(PROGNAME)

test0: all
	./$(PROGNAME) entrada.txt entrada.hm saida.txt 0

test1: all
	./$(PROGNAME) entrada.txt entrada.hm saida.txt 1
	gnuplot plot.gnu && display saida.png


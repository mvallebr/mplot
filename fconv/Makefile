CC=colorgcc
DLLNAME=libfconv.so
LDFLAGS=-lm
CFLAGS=-Wall -O2 -march=i586 -mcpu=i586 -I.
OBJS=  cdwrite.o hm.o fconv.o


all: lib

lib: $(OBJS)
	$(CC) $(LDFLAGS)  -shared -Wl,-soname,$(DLLNAME).0 -o $(DLLNAME).0.1 $(OBJS) 
	ln -sf $(DLLNAME).0.1 $(DLLNAME).0
	ln -sf $(DLLNAME).0 $(DLLNAME)

clean:
	rm -f *~ $(OBJS) $(DLLNAME)*



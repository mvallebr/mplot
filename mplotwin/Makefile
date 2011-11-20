CC=gcc
CFLAGS=-g -Wall -O2 -I. $$(sdl-config --cflags)
LDFLAGS_WINDOWS=$$(sdl-config --libs) -lSDL_image
OUTPUT_WINDOWS="mtest"
OBJS=main.o

DLLNAME_WINDOWS="libmplot.so"
DLL_LDFLAGS_WINDOWS=$(DFLAGS_WINDOWS)
LIBOBJS=mplot.o 
EXTRAOBJS= SFont.o

DEFFILE=mplot.def
ANAME_WINDOWS=libmplot.a

all: lib $(OBJS) 
	$(CC) $(OBJS) $(LDFLAGS_WINDOWS)  -L. -lmplot -o $(OUTPUT_WINDOWS) 

lib: $(LIBOBJS) $(EXTRAOBJS)
	$(CC) $(LDFLAGS_WINDOWS)  -shared -Wl,-soname,$(DLLNAME_WINDOWS).0 -o $(DLLNAME_WINDOWS).0.1 $(LIBOBJS) $(EXTRAOBJS)
	ln -sf $(DLLNAME_WINDOWS).0 $(DLLNAME_WINDOWS)
	#dllwrap --export-all --output-def $(DEFFILE) --implib $(ANAME_WINDOWS) \
	#--driver-name gcc -o $(DLLNAME_WINDOWS) $(LIBOBJS) $(EXTRAOBJS) \
	#$(DLL_LDFLAGS_WINDOWS)

clean:
	rm -f $(OBJS) $(LIBOBJS) *~ $(ANAME_WINDOWS) $(DLLNAME_WINDOWS)\
	 $(OUTPUT_WINDOWS) $(DEFFILE) 

static: $(LIBOBJS) $(OBJS) $(EXTRAOBJS)
	$(CC) $(OBJS) $(LIBOBJS) $(EXTRAOBJS) $(LDFLAGS_WINDOWS) -o $(OUTPUT_WINDOWS)

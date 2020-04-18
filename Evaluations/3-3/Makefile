MAIN=main
UTILS=utils
TRANS=transforms
MAT4=mat4
SHPERE=sphere
IDIR=..\..\Utils
CC=gcc
CFLAGS=-I$(IDIR)
DLLDIR=..\..\Libraries
BDIR=bin
ODIR=obj

LIBS=-lglew32 -lfreeglut -lopengl32

_DEPS = $(UTILS).h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_UOBJ = $(UTILS).o $(TRANS).o $(MAT4).o $(SHPERE).o
UOBJ = $(patsubst %,$(ODIR)/%,$(_UOBJ))

_OBJ = $(MAIN).o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
	if not exist $(ODIR) mkdir $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.c $(DEPS)
	if not exist $(ODIR) mkdir $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(MAIN): $(OBJ) $(UOBJ)
	if not exist $(BDIR) mkdir $(BDIR)
	if not exist $(BDIR)/*.dll copy $(DLLDIR) $(BDIR)
	gcc -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	if exist $(ODIR)\*.o del /f $(ODIR)\*.o
	if exist $(BDIR)\*.exe del /f $(BDIR)\*.exe

# If code changes you must make again and then make run.
run:
	if not exist $(BDIR)/$(MAIN).exe make
	$(BDIR)/$(MAIN)

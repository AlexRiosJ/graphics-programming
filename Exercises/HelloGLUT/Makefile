MAIN=main
UTILS=utils
IDIR=..\..\Utils
CC=gcc
CFLAGS=-I$(IDIR)
DLLDIR=..\..\Libraries
BDIR=bin
ODIR=obj

LIBS=-lglew32 -lfreeglut -lopengl32

_DEPS = $(UTILS).h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_UOBJ = $(UTILS).o
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
	if exist $(ODIR)\del del /f $(ODIR)\del
	if not exist $(BDIR)/*.dll copy $(DLLDIR) $(BDIR)
	gcc -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	if exist $(ODIR)\*.o del /f $(ODIR)\*.o
	if exist $(BDIR)\*.exe del /f $(BDIR)\*.exe

run:
	if not exist $(BDIR)/$(MAIN).exe make
	$(BDIR)/$(MAIN)

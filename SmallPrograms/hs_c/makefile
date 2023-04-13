SRCDIR = .
BINDIR = ./build
EXE = out

GHC_RUNTIME_PATH = /usr/lib/ghc-9.0.2/rts
GHC_INCLUDE_PATH = $(GHC_RUNTIME_PATH)/../include/
GHC_CFLAGS = -dynamic -shared -fforce-recomp -O2 -fPIC

all: prework $(BINDIR)/$(EXE)

prework:
	mkdir $(BINDIR)

$(BINDIR)/$(EXE): $(BINDIR)/lib.so $(BINDIR)/$(EXE).o
	gcc -o $@ $^ $(GHC_RUNTIME_PATH)/libHSrts-ghc9.0.2.so

$(BINDIR)/lib.so: $(SRCDIR)/fs.hs
	ghc -o $@ $< -outputdir $(BINDIR) $(GHC_CFLAGS)

$(BINDIR)/$(EXE).o: $(SRCDIR)/libtest.c $(BINDIR)/lib.so
	gcc -o $@ $< -I $(GHC_INCLUDE_PATH) -I $(BINDIR) -c

run:
	LD_LIBRARY_PATH=$(BINDIR):$(GHC_RUNTIME_PATH):${LD_LIBRARY_PATH} $(BINDIR)/$(EXE)

clean:
	rm -r $(BINDIR)

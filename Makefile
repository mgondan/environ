SOBJ=   $(PACKSODIR)/environ.$(SOEXT)

all:    $(SOBJ)

OBJ=	c/environ.o

$(SOBJ): $(OBJ)
	mkdir -p $(PACKSODIR)
	$(LD) $(LDSOFLAGS) -o $@ $(OBJ) $(SWISOLIB)

check::
install::
clean:
	rm -f $(OBJ)
distclean: clean
	rm -f $(SOBJ)

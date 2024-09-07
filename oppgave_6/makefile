# Modifisert makefil

INCLDIR = ./include
OBJDIR = ./obj

CC = gcc
CFLAGS = -O2 -Wall -Werror -Wextra
CFLAGS += -I$(INCLDIR)

LINKEXTRA = -lpthread

# Header filer 
_DEPS = ex6.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

# Source filer
_SRCS = ex6.c
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(_SRCS))

# Endre filnavn her
all: ex6

# Opprett 'obj'
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c $(DEPS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

ex6: $(OBJDIR) $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LINKEXTRA)


.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~


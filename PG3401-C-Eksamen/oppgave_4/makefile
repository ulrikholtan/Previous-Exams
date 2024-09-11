# Modifisert makefil

INCLDIR = ./include
OBJDIR = ./obj

CC = gcc
CFLAGS = -O2 -Wall -Werror -Wextra
CFLAGS += -I$(INCLDIR)
LINKEXTRA = -lpthread

# Header filer 
_DEPS = ex4.h sha1.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))
# Source filer
_SRCS = ex4.c sha1.c
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(_SRCS))

# Endre filvan her
all: ex4

# Opprett 'obj'
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c $(DEPS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

ex4: $(OBJDIR) $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LINKEXTRA)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~


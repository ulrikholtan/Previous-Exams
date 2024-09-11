# Modifisert makefil

INCLDIR = ./include
OBJDIR = ./obj

CC = gcc
CFLAGS = -O2 -Wall -Werror -Wextra
CFLAGS += -I$(INCLDIR)

LINKEXTRA = -lpthread

# Header filer 
_DEPS = server.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

# Source filer
_SRCS = server.c client.c
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(_SRCS))

# Endre filnavn her
all: server client

# Opprett 'obj'
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c $(DEPS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJDIR)/server.o
	$(CC) -o $@ $< $(CFLAGS) $(LINKEXTRA)


client: $(OBJDIR)/client.o
	$(CC) -o $@ $< $(CFLAGS) $(LINKEXTRA)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~


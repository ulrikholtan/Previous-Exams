# Modifisert makefil

INCLDIR = ./include
OBJDIR = ./obj

CC = gcc
CFLAGS = -O2 -Wall -Werror -Wextra
CFLAGS += -I$(INCLDIR)
LINKEXTRA = -lpthread

# Header filer 
_DEPS = ex2.h palindrome.h heterogram.h all_upper.h all_lower.h anagram.h disjoint.h sanitizer.h task2_word_metadata.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))
# Source filer
_SRCS = ex2.c palindrome.c heterogram.c all_upper.c all_lower.c anagram.c disjoint.c sanitizer.c task2_word_metadata.c
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(_SRCS))

# Endre filvan her
all: ex2

# Opprett 'obj'
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c $(DEPS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

ex2: $(OBJDIR) $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LINKEXTRA)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~



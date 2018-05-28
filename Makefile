ASIPLIB  = libasip.a

CC       = gcc
CFLAGS   = -Wall -I.
LINKER   = gcc
LFLAGS   = -Wall -I. -lm -lpthread

SRCDIR   = src
OBJDIR   = obj
INCDIR   = includes
EXADIR   = examples
LIBDIR   = lib
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXAMPLES := $(wildcard $(EXADIR)/*.c)
rm       = rm -f

all: $(OBJECTS) $(ASIPLIB) $(EXAMPLES:.c=)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) -I $(INCDIR) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully"

$(ASIPLIB): $(OBJECTS) 
	@mkdir -p $(LIBDIR)
	ar rcs $(LIBDIR)/$(ASIPLIB) $(OBJECTS)
	@echo "Static library complete"

$(EXAMPLES:.c=): $(OBJECTS) $(ASIPLIB)
	@mkdir -p $(BINDIR)
	@$(LINKER) -I $(INCDIR) $@.c -o $(BINDIR)/$(notdir $@) $(LIBDIR)/$(ASIPLIB) $(LFLAGS) 
	@echo "Done $(BINDIR)/$(notdir $@)"
	
.PHONY: clean distclean
clean:
	@$(rm) $(OBJECTS) $(LIBDIR)/$(ASIPLIB) $(BINDIR)/$(EXAMPLES:.c=)


distclean: clean
	rm -rf $(BINDIR) $(OBJDIR) $(LIBDIR)

# Takes the name of the current directory as a the executable and "pr" name
TARGET  := stnd
# Compiler
CC      := gcc
# Warning levels
WARN    :=
# Optimisation
OFLAGS  := -std=c99
# Aditionnal libraries to link
LDFLAGS := 

# Link Libraries
LDLIBS 	:= 

# Source directory
SRCDIR	:= src
# Include directory
INCDIR	:= include
# Object directory
OBJDIR	:= obj

CFLAGS	:= -I $(INCDIR)

# setting the vpath (additionnal search path for make)
VPATH	:= $(INCDIR) $(SRCDIR) $(OBJDIR)

CPP_SRCS    = $(wildcard src/*.c)
OBJ_FILES   = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(CPP_SRCS))

all: objdir $(TARGET)

$(TARGET): $(OBJ_FILES)
	@echo "Linking..."
	$(CC) $(WARN) $(OFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJ_FILES) -o $@ $(LDLIBS) 
	@echo "Done."

obj/%.o: %.c
	@echo "Compiling "$<"..."
	$(CC) -c $(WARN) $(OFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@

objdir:
	@echo "Creating object directory..."
	mkdir -p $(OBJDIR)

clean: 
	@echo "Cleaning..."
	rm -rf $(OBJDIR)/*.o

mrproper: clean
	rm -rf ${TARGET}


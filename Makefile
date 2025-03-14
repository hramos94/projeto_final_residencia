CC = gcc
CFLAGS = -Wall -Wextra 
TARGET = reb
BINFOLDER = bin/
SRCFOLDER = src/
OBJFOLDER = obj/
INCLUD = $(shell find ./inc -type d | sed 's/^/-I/')
LINK = -lGL -lm -lGLU -lGLEW -lglfw 


SRCFILES := $(shell find $(SRCFOLDER) -type f -name '*.c')

OBJFILES := $(patsubst $(SRCFOLDER)%.c,$(OBJFOLDER)%.o,$(SRCFILES))

all: $(OBJFILES) | $(BINFOLDER) 
	$(CC) $(CFLAGS) $(OBJFILES) -o $(BINFOLDER)$(TARGET) $(LINK)

$(OBJFOLDER)%.o: $(SRCFOLDER)%.c | $(OBJFOLDER)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< $(INCLUD) -o $@ 

$(BINFOLDER):
	mkdir -p $@

$(OBJFOLDER):
	mkdir -p $@

run:
	$(BINFOLDER)$(TARGET)

clean:
	rm -rf $(BINFOLDER)
	rm -rf $(OBJFOLDER)



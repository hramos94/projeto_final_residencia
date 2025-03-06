CC = gcc
CFLAGS = -Wall -Wextra 
TARGET = reb
BINFOLDER = bin/
SRCFOLDER = src/
OBJFOLDER = obj/
INCLUD = -I ./inc

SRCFILES := $(wildcard $(SRCFOLDER)*.c)

all: $(SRCFILES:src/%.c=obj/%.o) | $(BINFOLDER) 
	$(CC) $(CFLAGS) $(OBJFOLDER)*.o -o $(BINFOLDER)$(TARGET) 

obj/%.o: src/%.c | $(OBJFOLDER)
	$(CC) $(CFLAGS) -c $< $(INCLUD) -o $@ 

$(BINFOLDER):
	mkdir -p $@

$(OBJFOLDER):
	mkdir -p $@

run:
	$(BINFOLDER)$(TARGET)

clean:
	rm -f $(BINFOLDER)*
	rm -f $(OBJFOLDER)*



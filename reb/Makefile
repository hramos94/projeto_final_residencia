CC = gcc
CFLAGS = -Wall -Wextra 
TARGET = reb
BINFOLDER = bin/
SRCFOLDER = src/
BSWFOLDER = ../bsw/src/
OBJFOLDER = obj/
INCLUD = $(shell find ./inc -type d | sed 's/^/-I/')
INCLUD += $(shell find ../bsw/inc -type d | sed 's/^/-I/')
LINK = -lGL -lm -lGLU -lGLEW -lglfw 

# Separa fontes do projeto (reb/src) e BSW (bsw/src)
REB_SRCFILES := $(shell find $(SRCFOLDER) -type f -name '*.c')
BSW_SRCFILES := $(shell find $(BSWFOLDER) -type f -name '*.c')

# Mapeia fontes para objetos (reb/src/*.c -> obj/*.o e bsw/src/*.c -> obj/*.o)
OBJFILES := $(patsubst $(SRCFOLDER)%.c, $(OBJFOLDER)%.o, $(REB_SRCFILES))
OBJFILES += $(patsubst $(BSWFOLDER)%.c, $(OBJFOLDER)%.o, $(BSW_SRCFILES))

all: $(OBJFILES) | $(BINFOLDER) 
	$(CC) $(CFLAGS) $(OBJFILES) -o $(BINFOLDER)$(TARGET) $(LINK)

# Regra para compilar fontes do projeto (reb/src)
$(OBJFOLDER)%.o: $(SRCFOLDER)%.c | $(OBJFOLDER)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< $(INCLUD) -o $@

# Regra para compilar fontes BSW (bsw/src)
$(OBJFOLDER)%.o: $(BSWFOLDER)%.c | $(OBJFOLDER)
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


CC=nvcc
#CFLAGS= -`sdl2-config --cflags --libs` -lm #-c `sdl-config --cflags`
#LDFLAGS=  -lSDL2 -lSDL2_ttf 

#SRC=$(wildcard src/*.c, src/*.cu)
#OBJ=$(patsubst src/%.c, src/%.o, $(SRC))
#BIN=automata

all: 
	$(CC) src/main.cu src/render.cu src/util.c 

clean:
	$(RM) -r $(BIN) $(OBJ)





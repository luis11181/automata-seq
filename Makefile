CC=nvcc

#CFLAGS= -Wall `sdl2-config --cflags --libs` -fopenmp -lm -D_DEFAULT_SOURCE  -D_BSD_SOURCE#-c `sdl-config --cflags`
#LDFLAGS=  -lSDL2 -lSDL2_ttf 

#SRC=$(wildcard src/*.c)
#OBJ=$(patsubst src/%.c, src/%.o, $(SRC))
#BIN=automata

#all: $(BIN)
#$(BIN): $(OBJ)
#	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)
#src/%.o: src/%.c
#	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

all: 
	$(CC) -Xcompiler -sdl2-config  src/main.cu src/render.cu src/util.c  

clean:
	$(RM) -r $(BIN) $(OBJ)





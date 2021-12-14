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


#nvcc -O3 simpleMultiGPU.cu -o simpleMultiGPU
all: 
	$(CC) -Xcompiler -sdl2-config  src/main.cu src/render.cu src/util.c  

clean:
	$(RM) -r $(BIN) $(OBJ)




#! #############################################################################
# makefile from book
# SRCS := $(wildcard *.c)
# OBJS := $(patsubst %.c, %.o, $(SRCS))
# CUDA_SRCS := $(wildcard *.cu)
# CUDA_OBJS := $(patsubst %.cu, %.o, $(CUDA_SRCS))
# CUDA_PATH := /usr/local/cuda-6.0# specify your CUDA root path
# NVCC := $(CUDA_PATH)/bin/nvcc
# CC := icc
# LD := icc -openmp
# CUDA_LIB := -L$(CUDA_PATH)/lib64 -lcublas -lcufft -lcudart
# CUDA_INC += -I$(CUDA_PATH)/include
# CFLAGS += -std=c99
# INCLUDES := # specify include path for host code
# GPU_CARD := -arch=sm_35 # specify your device compute capability
# NVCC_FLAGS += -O3 -dc # separate compilation
# NVCC_FLAGS += -Xcompiler -fopenmp
# CUDA_LINK_FLAGS := -dlink # device linker option
# EXEC := test # specify your executable name
# CUDA_LINK_OBJ := cuLink.o
# all: $(EXEC)
# $(EXEC): $(OBJS) $(CUDA_OBJS)
#  $(NVCC) $(GPU_CARD) $(CUDA_LINK_FLAGS) -o $(CUDA_LINK_OBJ) $(CUDA_OBJS)
#  $(LD) -o $@ $(OBJS) $(CUDA_OBJS) $(CUDA_LINK_OBJ) $(CUDA_LIB)
# %.o : %.c
#  $(CC) -o $@ -c $(CFLAGS) $(INCLUDES) $<
# %.o : %.cu
#  $(NVCC) $(GPU_CARD) $(NVCC_FLAGS) -o $@ -c $< $(CUDA_INC)
# clean:
#  rm -f $(OBJS) $(EXEC) *.o a.out


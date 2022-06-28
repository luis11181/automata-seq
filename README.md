# parallel sand/substances simulator
Implementation of a material simulator in C, user can add material and see how they interact over time.

## Dependencies
* GCC
* GNU Make
* SDL2
* CUDA
* MPI
* OPENMP
* must be on linux for the <sys/time.h>, SDL and TTF typography 
* 
## Demo
Graphical interface of the simulation

<img width="364" alt="sandsim" src="https://user-images.githubusercontent.com/80784724/153083285-99e0e162-d3ab-4130-b7ca-eee35bf88c18.png">

Top menu with the frames per second 

<img width="374" alt="menu" src="https://user-images.githubusercontent.com/80784724/153083517-486992f2-19fe-4f26-8cb8-1f14716f0cc1.png">

## Sequential version

Sequential version is based on a found SDL implementation of a cellular automata https://github.com/joaquin-rossi/automata; Then the implemented logic for the sandsimulation, the element rules, rendering and parallelism from scratch

run the makefile to compile

# OpenMp parallel version

optimized parallel version with OpenMp.

<img width="374" alt="sandsim2openmp" src="https://user-images.githubusercontent.com/80784724/153085864-5ebfd712-e215-4387-9a31-8a63dcef4dd8.png">


# Cuda paralell version

Functional Cuda paraller version, result are slower for this application

# MPI paralell version

OpenMPI paraller version of the logic, console simplified app that calculate the matrices but does not display, implementation was done in a 8 computers Aws cluster

to run: after all the configuration it is compiled and run with 8 nodes

mpicc sandsim.c -o sandsim
mpirun -np 8 -host nodo0,nodo1,nodo2,nodo3,nodo4,nodo5,nodo6,nodo7 ./sandsim

## Extra info

* Press spacebar to toggle between paused and unpaused.
* Press the mpuse to draw with the elements
* Press the keys to change the elemnts.
F= fire, R= rock, A= air, W= water, S= sand, H= smoke/humo...





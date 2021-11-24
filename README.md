# CELULLAR AUTOMATA SAND SIMULATION OPTIMIZED WITH DIFFERENT LIBRARIES AND PARADIGMS FOR PARALLELISM 
Implementation of a  celullar automata simulation with SDL2 in C. Developed and tested on Linux.

### Dependencies
* GCC
* GNU Make
* SDL2
* libsdl2-dev
* ttf-ubuntu-font-family

### Installation
- sudo apt-get update
- sudo apt-get install libsdl2-dev
- sudo apt-get install libsdl2-ttf-dev
- sudo apt-get install ttf-ubuntu-font-family
- make clean
- make all

### Game info
* Press spacebar to toggle between paused and unpaused.
* click and move the mouse to draw  in the game, putting an element in the simulation
* Press the different letters to change the element to draw in the game with the mouse f=fire, s=sand, r=rock, w=water, a=air, o=oil
* press + or - to change the brush size

## OpenMP parallel implementation

![PARALLEL SPEEDUP](https://user-images.githubusercontent.com/80784724/143155717-ba5ceb6b-340f-4b94-a1ce-940d7e506cdf.jpg)

![PARALLEL COST](https://user-images.githubusercontent.com/80784724/143155731-2fa46800-c5c5-44af-8cf8-aa3f68737970.jpg)


## Demo

![programa](https://user-images.githubusercontent.com/80784724/143155782-f1c3a149-0b24-4153-846e-80c2f92ceefd.jpg)



#include <SDL2/SDL.h>
#include <stdbool.h> 
#include <stdlib.h>

#include "logic.h"
#include "render.h"

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

//!! define color of the automate, already listed in logic properties

const SDL_Color BLACK_CELL_COLOR = { .r = 0, .g = 0, .b = 0 };
const SDL_Color WHITE_CELL_COLOR = { .r = 255, .g = 255, .b = 255 };
const SDL_Color BLUE_CELL_COLOR = { .r = 0, .g = 0, .b = 255 };
const SDL_Color RED_CELL_COLOR = { .r = 255, .g = 0, .b = 0 };
const SDL_Color GRAY_CELL_COLOR = { .r = 128, .g = 128, .b = 128 };
const SDL_Color YELLOW_CELL_COLOR = { .r = 255, .g = 255, .b = 0 };
const SDL_Color PURPLE_CELL_COLOR = { .r = 119, .g = 0, .b = 199 };
const SDL_Color GREEN_CELL_COLOR = { .r = 0, .g = 255, .b = 0 };
const SDL_Color WHITEBLUE_CELL_COLOR = { .r = 176, .g = 241, .b = 247 };
const SDL_Color GRAYSMOKE_CELL_COLOR = { .r = 176, .g = 176, .b = 176 };
const SDL_Color STRUCTURE_CELL_COLOR = { .r = 67, .g = 59, .b = 27 };


const SDL_Color ANT_COLOR = { .r = 255, .g = 50, .b = 50 };

void render_grid(SDL_Renderer *renderer, const state_t *state)
{
    for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++) {
            SDL_Rect rect = {
                .x = x * CELL_WIDTH,
                .y = y * CELL_HEIGHT,
                .w = CELL_WIDTH,
                .h = CELL_HEIGHT
            };

            switch(state->board[x][y]) {
                case BLACK:
                    SDL_SetRenderDrawColor(renderer, BLACK_CELL_COLOR.r, BLACK_CELL_COLOR.g, BLACK_CELL_COLOR.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;

                case BLUE:
                    SDL_SetRenderDrawColor(renderer, BLUE_CELL_COLOR.r, BLUE_CELL_COLOR.g, BLUE_CELL_COLOR.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;

                case RED:
                    SDL_SetRenderDrawColor(renderer, RED_CELL_COLOR.r, RED_CELL_COLOR.g, RED_CELL_COLOR.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case GRAY:
                    SDL_SetRenderDrawColor(renderer, GRAY_CELL_COLOR.r, GRAY_CELL_COLOR.g, GRAY_CELL_COLOR.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case YELLOW:
                    SDL_SetRenderDrawColor(renderer, YELLOW_CELL_COLOR.r, YELLOW_CELL_COLOR.g, YELLOW_CELL_COLOR.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case WHITEBLUE:
                    SDL_SetRenderDrawColor(renderer, WHITEBLUE_CELL_COLOR.r, WHITEBLUE_CELL_COLOR.g, WHITEBLUE_CELL_COLOR.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case GREEN:
                    SDL_SetRenderDrawColor(renderer, GREEN_CELL_COLOR.r, GREEN_CELL_COLOR.g, GREEN_CELL_COLOR.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                   
                case PURPLE:
                    SDL_SetRenderDrawColor(renderer, PURPLE_CELL_COLOR.r, PURPLE_CELL_COLOR.g, PURPLE_CELL_COLOR.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case GRAYSMOKE:
                    SDL_SetRenderDrawColor(renderer, GRAYSMOKE_CELL_COLOR.r, GRAYSMOKE_CELL_COLOR.g, GRAYSMOKE_CELL_COLOR.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case STRUCTURE:
                    SDL_SetRenderDrawColor(renderer, STRUCTURE_CELL_COLOR.r, STRUCTURE_CELL_COLOR.g, STRUCTURE_CELL_COLOR.b, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                default: {}
            }
        }

}

void langtons_ant(SDL_Renderer *renderer, state_t *state)
{
    // RENDER ANT
    SDL_SetRenderDrawColor(renderer, ANT_COLOR.r, ANT_COLOR.g, ANT_COLOR.b, 255);
    SDL_Rect ant_rect = {
        .x = state->ant.x * CELL_WIDTH ,
        .y = state->ant.y * CELL_HEIGHT,
        .w = CELL_WIDTH,
        .h = CELL_HEIGHT
    };
    SDL_RenderFillRect(renderer, &ant_rect);

    if (state->mode == RUNNING_MODE)
    for (int i = 0; i < MOVES_PER_FRAME; i++) {
        int current = state->board[state->ant.x][state->ant.y];

        // TURN 90º
        switch (current) {
            case WHITE:
                state->ant.dir = mod(state->ant.dir + 1, 4);
                break;
            case BLACK:
                state->ant.dir = mod(state->ant.dir - 1, 4);
                break;
        }

        // FLIP THE COLOR OF THE SQUARE
        state->board[state->ant.x][state->ant.y] = BLACK + WHITE - current;

        // MOVE FORWARD ONE UNIT
        switch (state->ant.dir) {
            case UP:
                state->ant.y = mod(state->ant.y - 1, N);
                break;
            case RIGHT:
                state->ant.x = mod(state->ant.x + 1, N);
                break;
            case DOWN:
                state->ant.y = mod(state->ant.y + 1, N);
                break;
            case LEFT:
                state->ant.x = mod(state->ant.x - 1, N);
                break;
        }
    }
}

void game_of_life(SDL_Renderer *renderer, state_t *state)
{
    if (state->mode == RUNNING_MODE)
    for (int i = 0; i < MOVES_PER_FRAME; i++) {
        int new_board[N][N];

        for (int x = 0; x < N; x++)
            for (int y = 0; y < N; y++) {
                int n_neigh = 
                    state->board[mod((x - 1), N)][mod((y - 1), N)] +
                    state->board[mod((x    ), N)][mod((y - 1), N)] +
                    state->board[mod((x + 1), N)][mod((y - 1), N)] +
                    state->board[mod((x - 1), N)][mod((y    ), N)] +
                    state->board[mod((x + 1), N)][mod((y    ), N)] +
                    state->board[mod((x - 1), N)][mod((y + 1), N)] +
                    state->board[mod((x    ), N)][mod((y + 1), N)] +
                    state->board[mod((x + 1), N)][mod((y + 1), N)];

                if (state->board[x][y] == ALIVE && (n_neigh == 2 || n_neigh == 3))
                    new_board[x][y] = ALIVE;
                else if (state->board[x][y] == DEAD && n_neigh == 3)
                    new_board[x][y] = ALIVE;
                else
                    new_board[x][y] = DEAD;
            }

        for (int x = 0; x < N; x++)
            for (int y = 0; y < N; y++)
                state->board[x][y] = new_board[x][y];
    }
}

void brians_brain(SDL_Renderer *renderer, state_t *state)
{
    if (state->mode == RUNNING_MODE)
    for (int i = 0; i < MOVES_PER_FRAME; i++) {
        int new_board[N][N];

        for (int x = 0; x < N; x++)
            for (int y = 0; y < N; y++) {
                int n_neigh =
                    (state->board[mod((x - 1), N)][mod((y - 1), N)] == ON) +
                    (state->board[mod((x    ), N)][mod((y - 1), N)] == ON) +
                    (state->board[mod((x + 1), N)][mod((y - 1), N)] == ON) +
                    (state->board[mod((x - 1), N)][mod((y    ), N)] == ON) +
                    (state->board[mod((x + 1), N)][mod((y    ), N)] == ON) +
                    (state->board[mod((x - 1), N)][mod((y + 1), N)] == ON) +
                    (state->board[mod((x    ), N)][mod((y + 1), N)] == ON) +
                    (state->board[mod((x + 1), N)][mod((y + 1), N)] == ON);

                if (state->board[x][y] == OFF && n_neigh == 2)
                    new_board[x][y] = ON;
                else if (state->board[x][y] == ON)
                    new_board[x][y] = DYING;
                else
                    new_board[x][y] = OFF;
            }

        for (int x = 0; x < N; x++)
            for (int y = 0; y < N; y++)
                state->board[x][y] = new_board[x][y];
    }
}

void wireworld(SDL_Renderer *renderer, state_t *state)
{
    if (state->mode == RUNNING_MODE)
    for (int i = 0; i < MOVES_PER_FRAME; i++) {
        int new_board[N][N];

        for (int x = 0; x < N; x++)
            for (int y = 0; y < N; y++) {
                int n_neigh =
                    (state->board[mod((x - 1), N)][mod((y - 1), N)] == ELECTRON_HEAD) +
                    (state->board[mod((x    ), N)][mod((y - 1), N)] == ELECTRON_HEAD) +
                    (state->board[mod((x + 1), N)][mod((y - 1), N)] == ELECTRON_HEAD) +
                    (state->board[mod((x - 1), N)][mod((y    ), N)] == ELECTRON_HEAD) +
                    (state->board[mod((x + 1), N)][mod((y    ), N)] == ELECTRON_HEAD) +
                    (state->board[mod((x - 1), N)][mod((y + 1), N)] == ELECTRON_HEAD) +
                    (state->board[mod((x    ), N)][mod((y + 1), N)] == ELECTRON_HEAD) +
                    (state->board[mod((x + 1), N)][mod((y + 1), N)] == ELECTRON_HEAD);

                /* RULES
                   - empty -> empty
                   - electron head -> electron tail
                   - electron tail -> conductor
                   - conductor -> n_neigh = 1 or 2 ? electron head : conductor
                 */

                if (state->board[x][y] == EMPTY)
                    new_board[x][y] = EMPTY;
                else if (state->board[x][y] == ELECTRON_HEAD)
                    new_board[x][y] = ELECTRON_TAIL;
                else if (state->board[x][y] == ELECTRON_TAIL)
                    new_board[x][y] = CONDUCTOR;
                else if (state->board[x][y] == CONDUCTOR && (n_neigh == 1 || n_neigh == 2))
                    new_board[x][y] = ELECTRON_HEAD;
                else
                    new_board[x][y] = CONDUCTOR;
            }

        for (int x = 0; x < N; x++)
            for (int y = 0; y < N; y++)
                state->board[x][y] = new_board[x][y];
    }
}

////////////////////////////////////////////////////////////////////////////////!!
//SAND SIMULATION FUNCTIONS

//world_sim_puede_moverse define si un elemento se puede mover o interactura dada la presencia de otro elemento en su posicion destino
bool world_sim_puede_moverse(state_t *state, short sustancia, int x, int y){
    // Si las coordenadas se salen de los límites, no se puede mover por defecto
    if(x < 0 || x > N-1 || y < 0 || y > N-1){ return false;}
    switch (sustancia)
    {
    case SAND:
      if(state->board[x][y] == AIR){ return true;} 
      else if (state->board[x][y] == SAND){ return false;}
      else if (state->board[x][y] == WATER && drand48() < 0.5){ return true;}
      else if (state->board[x][y] == ROCK){ return false;}
      else if (state->board[x][y] == OIL){ return true;}
      else if (state->board[x][y] == HUMO){ return true;}
      
      break;

    case WATER:
      if(state->board[x][y] == AIR){ return true;} 
      else if (state->board[x][y] == SAND){ return false;}
      else if (state->board[x][y] == WATER){ return false;}
      else if (state->board[x][y] == ROCK){ return false;}
      else if (state->board[x][y] == OIL && drand48() < 0.5){ return true;}
      else if (state->board[x][y] == HUMO){ return true;}

      break;
  
    case ROCK:
      if(state->board[x][y] == AIR){ return true;} 
      else if (state->board[x][y] == SAND && drand48() < 0.2 ){ return true;}
      else if (state->board[x][y] == WATER && drand48() < 0.9 ){ return true;}
      else if (state->board[x][y] == ROCK){ return false;}
      else if (state->board[x][y] == OIL && drand48() < 0.9){ return true;}
      else if (state->board[x][y] == HUMO){ return true;}

      break;

    // No es necesario porque el aire nunca se mueve por si mismo
    /*case AIR: 
      if(state->board[x][y] == AIR){ return true;}
      break;*/

    case FIRE:
      if(state->board[x][y] == AIR){ return true;} 
      else if (state->board[x][y] == SAND){ return false;}
      else if (state->board[x][y] == WATER){ return true;}
      else if (state->board[x][y] == ROCK){ return false;}
      else if (state->board[x][y] == FIRE){ return false;}
      else if (state->board[x][y] == OIL){ return true;}
      else if (state->board[x][y] == HUMO){ return true;}
      break;

    case OIL:
      if(state->board[x][y] == AIR){ return true;} 
      else if (state->board[x][y] == SAND){ return false;}
      else if (state->board[x][y] == WATER){ return false;}
      else if (state->board[x][y] == ROCK){ return false;}
      else if (state->board[x][y] == OIL){ return false;}
      else if (state->board[x][y] == HUMO){ return true;}

      break;
    
    case HUMO:
      if(state->board[x][y] == AIR){ return true;} 
      
      break;
    
    default:
      return false;
      break;
    }

    return false;
}

//* all _sim_mover , define cuales seran las regls e interacciones que genera cada elemento al moverse

//La mejor forma seria importar la direccion a todo el arreglo de flags y trabajar con el, pero no pude hacerlo asi
void sand_sim_mover(state_t *state, bool seHaMovidoFlags[N][N], int fromX, int fromY, int toX, int toY){
    short sustancia = state->board[fromX][fromY];
    short otraSustancia = state->board[toX][toY];
    //Switch para las interacciones especiales
    switch (sustancia)
    {
    case FIRE:
        if(otraSustancia == OIL){
            state->board[fromX][fromY] =FIRE;
            state->board[toX][toY] = FIRE;
        } else if(otraSustancia == WATER){
            state->board[fromX][fromY] = HUMO;
            state->board[toX][toY] = WATER;
        } else{
            bool seDescompone= drand48() < 0.003;
            if(seDescompone){
                state->board[fromX][fromY] = HUMO;
                state->board[toX][toY] = otraSustancia;
            } else{
                state->board[fromX][fromY] = otraSustancia;
                state->board[toX][toY] = FIRE;
            }
  
        }
        break;

    case HUMO:
        
        if (true)
        {
            bool seDescompone= drand48() < 0.05;
            if(seDescompone){
            state->board[fromX][fromY] = AIR;
            state->board[toX][toY] = otraSustancia; } 
            else{
            state->board[fromX][fromY] = otraSustancia;
            state->board[toX][toY] = sustancia;
        }}

        break;

    default:
        state->board[fromX][fromY] = otraSustancia;
        state->board[toX][toY] = sustancia;
        break;
    }
    seHaMovidoFlags[fromX][fromY] = true;
    seHaMovidoFlags[toX][toY] = true;
}

bool sand_sim_mover_abajo(state_t *state, short sustancia, bool seHaMovidoFlags[N][N],int x, int y){
    if(world_sim_puede_moverse(state, sustancia, x, y + 1)){ //Mover abajo
        sand_sim_mover(state, seHaMovidoFlags, x, y, x, y + 1);
        return true;
    }
    return false;
}

bool sand_sim_mover_izq_der(state_t *state, short sustancia, bool seHaMovidoFlags[N][N],int x, int y){
    //random number to define if it should go left or right
    bool primeroIzquierda = drand48() < 0.5;
    if(primeroIzquierda){
        if(world_sim_puede_moverse(state, sustancia, x - 1, y)){ //Mover a la izquierda
            sand_sim_mover(state, seHaMovidoFlags, x, y, x - 1, y);
            return true;
        } else if(world_sim_puede_moverse(state, sustancia, x + 1, y)){ //Mover a la derecha
            sand_sim_mover(state, seHaMovidoFlags, x, y, x + 1, y);
            return true;
        }
    } else {
        if(world_sim_puede_moverse(state, sustancia, x + 1, y)){ //Mover a la derecha
            sand_sim_mover(state, seHaMovidoFlags, x, y, x + 1, y);
            return true;
        } else if(world_sim_puede_moverse(state, sustancia, x - 1, y)){ //Mover a la izquierda
            sand_sim_mover(state, seHaMovidoFlags, x, y, x - 1, y);
            return true;
        }
    }
    return false;
}

bool sand_sim_mover_abajo_diagonal(state_t *state, short sustancia, bool seHaMovidoFlags[N][N],int x, int y){
    //random number to define if it should go left or right
    bool primeroIzquierda = drand48() < 0.5;
        if(primeroIzquierda){
            if(world_sim_puede_moverse(state, sustancia, x - 1, y + 1)){ //Mover a la izquierda
                sand_sim_mover(state, seHaMovidoFlags, x, y, x-1, y+1);
                return true;
            } else if (world_sim_puede_moverse(state, sustancia, x + 1, y + 1)){ //Mover a la derecha
                sand_sim_mover(state, seHaMovidoFlags, x, y, x+1, y+1);
                return true;
            }
        } else {
            if(world_sim_puede_moverse(state, sustancia, x + 1, y + 1)){ //Mover a la derecha
                sand_sim_mover(state, seHaMovidoFlags, x, y, x+1, y+1);
                return true;
            } else if (world_sim_puede_moverse(state, sustancia, x - 1, y + 1)){ //Mover a la izquierda
                sand_sim_mover(state, seHaMovidoFlags, x, y, x-1, y+1);
                return true;
            }
        }
    return false;
}


// x, y: Posición de la partícula que se mueve abajo o a los lados
bool sand_sim_mover_abajo_y_lados(state_t *state, short sustancia, bool seHaMovidoFlags[N][N],int x, int y){
    
    //Si no se puede mover hacia abajo
    if(!sand_sim_mover_abajo(state, sustancia, seHaMovidoFlags, x, y)){                 
        //Se moverá en diagonal hacia abajo
        if(sand_sim_mover_abajo_diagonal(state, sustancia, seHaMovidoFlags, x, y)){
            //Si se mueve en diagonal, retornar true
            return true;
        }
    }
    return false; 
}


// x, y: Posición de la partícula que se mueve arriba o a los lados
bool sand_sim_mover_arriba_y_lados(state_t *state, short sustancia, bool seHaMovidoFlags[N][N],int x, int y){
    
    if(world_sim_puede_moverse(state, sustancia, x, y - 1)){ //Mover arriba
        sand_sim_mover(state, seHaMovidoFlags, x, y, x, y - 1);
        return true;
    }

    bool primeroIzquierda = drand48() < 0.5;

    if(primeroIzquierda){
        if(world_sim_puede_moverse(state, sustancia, x - 1, y - 1)){ //Mover a la izquierda
            sand_sim_mover(state, seHaMovidoFlags, x, y, x-1, y-1);
            return true;
        } else if (world_sim_puede_moverse(state, sustancia, x + 1, y - 1)){ //Mover a la derecha
            sand_sim_mover(state, seHaMovidoFlags, x, y, x+1, y-1);
            return true;
        }
    } else {
        if(world_sim_puede_moverse(state, sustancia, x + 1, y - 1)){ //Mover a la derecha
            sand_sim_mover(state, seHaMovidoFlags, x, y, x+1, y-1);
            return true;
        } else if (world_sim_puede_moverse(state, sustancia, x - 1, y - 1)){ //Mover a la izquierda
            sand_sim_mover(state, seHaMovidoFlags, x, y, x-1, y-1);
            return true;
        }
    }
    
    return false; 
}


//***** world_sand_sim() RUNS THE SIMULATION logic for all elements of the world
void world_sand_sim(SDL_Renderer *renderer, state_t *state)
{
    if (state->mode == RUNNING_MODE){
    for (int i = 0; i < MOVES_PER_FRAME; i++) {
        //int new_board[N][N] = {state->board};
        bool seHaMovidoFlags[N][N] = {false};

        
        for (int y = N-1; y >= 0; y--){
            for (int x = 0; x < N; x++) {
                
                //Saltar si ya se ha movido esta posicion
                if(seHaMovidoFlags[x][y]) continue;
                
                //*g  rules and functions for sand
                if(state->board[x][y] == SAND){
                    sand_sim_mover_abajo_y_lados(state, SAND, seHaMovidoFlags, x, y);                  
                }

                //*g rules and functions for rock
                if(state->board[x][y] == ROCK){
                    sand_sim_mover_abajo(state, ROCK, seHaMovidoFlags, x, y);
                }

               //*g  rules and functions for water
                if(state->board[x][y] == WATER){
                    
                    //Si el agua no se puede mover abajo o a los lados
                    if(!sand_sim_mover_abajo_y_lados(state, WATER, seHaMovidoFlags, x, y)){
                        //Se mueve a la izquierda o derecha
                        sand_sim_mover_izq_der(state, WATER, seHaMovidoFlags, x, y);
                    }   
                }

              //*g  rules and functions for oil
                if(state->board[x][y] == OIL){
                    //Si el agua no se puede mover abajo o a los lados
                    if(!sand_sim_mover_abajo_y_lados(state, OIL, seHaMovidoFlags, x, y)){
                        //Se mueve a la izquierda o derecha
                        sand_sim_mover_izq_der(state, OIL, seHaMovidoFlags, x, y);
                    }   
                }

                //*g rules and functions for fire
                if(state->board[x][y] == FIRE){
                    if (!sand_sim_mover_abajo_y_lados(state, FIRE, seHaMovidoFlags, x, y))
                    {
                      bool seDescompone= drand48() < 0.2;
                      if(seDescompone){
                          state->board[x][y] = HUMO;
                      } 

                    }
                    
                }

                //*g rules and functions for humo
                if(state->board[x][y] == HUMO){
                  //Si el humo no puede moverse arriba o diagonal va para los lados
                    if(!sand_sim_mover_arriba_y_lados(state, HUMO, seHaMovidoFlags, x, y)){
                        //Se mueve a la izquierda o derecha
                        sand_sim_mover_izq_der(state, HUMO, seHaMovidoFlags, x, y);
                    }   
                    
                }

            }
        }        
    }
    }
}

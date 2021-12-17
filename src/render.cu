#include <SDL2/SDL.h>
#include "/usr/include/SDL2/SDL_ttf.h"
#include <stdbool.h> 
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <ctime>
#include "cuda_runtime.h"
#include "common/book.h"

#include "logic.h"
#include "render.h"
#include "util.h"


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

//NUMBER OF THREADS WE ARE USING IN SPECIFIC MOMENT
int threads=1;
int s_cambio_threads = 10; //Segundos para aumentar las thread

//Variables benchmarking Render Grid
int render_grid_framecnt = 0;
int fps_render_grid = 0;
double avg_time_render_grid = 0;


void render_grid(SDL_Renderer *renderer, const state_t *state)
{
    // //*calculate time to render the grid
    // struct timeval tval_before, tval_after, tval_result;
    
    // gettimeofday(&tval_before, NULL);


    //* Change thread number every 5 seconds and check if threads are less than the maximum of threads, so we can see the FPS for each number of threads
    ++render_grid_framecnt; //Ir sumando los frames

    resetTimer(TVAL_THREAD_2); //Actualizar timer
    
    if(( getTimerS(TVAL_THREAD_2)-getTimerS(TVAL_THREAD_1) >= s_cambio_threads) && (threads <= THREADS)){
        
        double avg_fps = render_grid_framecnt/s_cambio_threads;
        long double avg_time = ((avg_time_render_grid/avg_fps)/s_cambio_threads)*1000000;
        fps_render_grid = avg_fps;
        SDL_Log("[RENDER] Threads: %d, #de FPS promedio de los anteriores %d segundos: %0.1f, Tiempo promedio (ms): %0.1Lf", 
        threads, 
        s_cambio_threads, 
        avg_fps,
        avg_time);
        threads ++; //aumenta # threads
        resetTimer(TVAL_THREAD_2); //Actualizar timer
        resetTimer(TVAL_THREAD_1); //Actualizar timer
        render_grid_framecnt = 0; //Reiniciar la cuenta
        avg_time_render_grid = 0; //Reniciar el promedio de tiempo en X segundos
       
    }

    
    //#pragma omp parallel num_threads(threads)  
    //{
    //#pragma omp for
    for (int x = 0; x < N; x++){  
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
          }}


        // //*calculate time to render the grid
        // gettimeofday(&tval_after, NULL);
        // timersub(&tval_after, &tval_before, &tval_result);
        


        //* calculate total TIME to run the whole program
        resetTimer(TVAL_TOTAL_2);

        long double  d = ((getTimerS(TVAL_TOTAL_2)*1000000+(getTimerMS(TVAL_TOTAL_2))) -(getTimerS(TVAL_TOTAL_1)*1000000+(getTimerMS(TVAL_TOTAL_1)) ));

        avg_time_render_grid += d/1000000;

        char str[128];
        sprintf(str, "Total time to loop the whole program (ms): %0.1Lf", 
            d
           );
        renderFormattedText(renderer, str, 0 , 20);

        char str2[128];
        sprintf(str2, "Threads: %d, AVG_FPS(%d s): %d", threads, s_cambio_threads, fps_render_grid);
        renderFormattedText(renderer, str2, 250 , 0);

        //* calculate total time to run the whole program
        resetTimer(TVAL_TOTAL_1); 
    //}

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

//Variables benchmarking Función SandSim
int sandsim_framecnt = 0;
int thread_sandsim_cnt = 1;
int fps_sandsim = 0;
//double time_sandsim_acum = 0;
double avg_time_sandsim = 0;
double avg_FPS_sandsim = 0;


//* /////////////////////////////////
//* cuda function
__global__ void cudaFunction(int *d_Matrix, int *d_Values, int dim) {

  // map from threadIdx/BlockIdx to pixel position
  int x = threadIdx.x + blockIdx.x * blockDim.x;// thread column
  int y = threadIdx.y + blockIdx.y * blockDim.y;//thread row

  *d_Matrix = {{1}};

  if (x<dim && y<dim) {
    int offset = x + y * blockDim.x * gridDim.x;
    // write array contINING all the offset variable values
    *d_Values[offset] = offset;
  }
  
  
}
//* /////////////////////////////////


//***** world_sand_sim() RUNS THE SIMULATION logic for all elements of the world
void world_sand_sim(SDL_Renderer *renderer, state_t *state)
{

  //*/////////////////////////////////////
  //CUDA CODE

  int grid_dim;
  int block_dim;

  if (N < 1024) {
      grid_dim = N;
      block_dim = N;
  }
  else {
      grid_dim = 1024;
      block_dim = 1024;
  }

  // malloc device memory
  int Matrix[N][N]={{0}};
  int Values[N*N]={{0}};
  int *d_Matrix;
  int *d_Values;

  HANDLE_ERROR( cudaMalloc((void **)&d_Matrix, N * N * sizeof(int)));
  HANDLE_ERROR( cudaMalloc((void **)&d_Values, N * N * sizeof(int)));

  // transfer data from host to device
  //HANDLE_ERROR( cudaMemcpy(d_Matrix, &Matrix, cudaMemcpyHostToDevice));

  // invoke the kernel
  cudaFunction<<< grid_dim, block_dim >>>(d_Matrix, d_Values, N);
  cudaDeviceSynchronize();


  HANDLE_ERROR( cudaMemcpy( &Matrix, d_Matrix, N * N * sizeof(int) , cudaMemcpyDeviceToHost ) );
  HANDLE_ERROR( cudaMemcpy( &Values, d_Values, N * N * sizeof(int) , cudaMemcpyDeviceToHost ) );

  printf( Matrix );
  printf( Values );

  // free host and devide memory
  HANDLE_ERROR( cudaFree(d_MatA));
  free(h_A);


  //*/////////////////////////////////////



//   ++sandsim_framecnt; //Sumando los fps de sandsim

//   if (state->mode == RUNNING_MODE){

//       //CÁLCULO DE EL TIEMPO PROMEDIO PARA CADA NUMERO DE THREADS
//       if(threads != thread_sandsim_cnt){
//           double avg_fps = sandsim_framecnt/s_cambio_threads;
//           long double avg_time = (avg_time_sandsim /avg_fps) / s_cambio_threads; 
//           SDL_Log("[SANDSIM] Tiempo promedio para %d threads (ms): %0.1Lf",thread_sandsim_cnt,avg_time);
//           avg_time_sandsim = 0; //Reinciar el conteo del promedio acumulado en X segundos
//           sandsim_framecnt = 0;
//           thread_sandsim_cnt = threads; //Se actualiza la variable para el contador interno de sandsim
//       }

      
//     for (int i = 0; i < MOVES_PER_FRAME; i++) {
//       //int new_board[N][N] = {state->board};
//       bool seHaMovidoFlags[N][N] = {false};

//       //*calculate time to render the grid
//       struct timeval tval_before_sandsim, tval_after_sandsim, tval_result_sandsim;
//       gettimeofday(&tval_before_sandsim, NULL);
      
      
//       //#pragma omp parallel num_threads(threads) 
//       //{
        
//         for (int y = N-1; y >= 0; y--){
//             //#pragma omp for //collapse(2)
//             for (int x = 0; x < N; x++) {
                
//                 //Saltar si ya se ha movido esta posicion
//                 if(seHaMovidoFlags[x][y]) continue;
                
//                 //*g  rules and functions for sand
//                 if(state->board[x][y] == SAND){
//                     sand_sim_mover_abajo_y_lados(state, SAND, seHaMovidoFlags, x, y);                  
//                 }

//                 //*g rules and functions for rock
//                 if(state->board[x][y] == ROCK){
//                     sand_sim_mover_abajo(state, ROCK, seHaMovidoFlags, x, y);
//                 }

//               //*g  rules and functions for water
//                 if(state->board[x][y] == WATER){
                    
//                     //Si el agua no se puede mover abajo o a los lados
//                     if(!sand_sim_mover_abajo_y_lados(state, WATER, seHaMovidoFlags, x, y)){
//                         //Se mueve a la izquierda o derecha
//                         sand_sim_mover_izq_der(state, WATER, seHaMovidoFlags, x, y);
//                     }   
//                 }

//               //*g  rules and functions for oil
//                 if(state->board[x][y] == OIL){
//                     //Si el agua no se puede mover abajo o a los lados
//                     if(!sand_sim_mover_abajo_y_lados(state, OIL, seHaMovidoFlags, x, y)){
//                         //Se mueve a la izquierda o derecha
//                         sand_sim_mover_izq_der(state, OIL, seHaMovidoFlags, x, y);
//                     }   
//                 }

//                 //*g rules and functions for fire
//                 if(state->board[x][y] == FIRE){
//                     if (!sand_sim_mover_abajo_y_lados(state, FIRE, seHaMovidoFlags, x, y))
//                     {
//                       bool seDescompone= drand48() < 0.2;
//                       if(seDescompone){
//                           state->board[x][y] = HUMO;
//                       } 

//                     }
                    
//                 }

//                 //*g rules and functions for humo
//                 if(state->board[x][y] == HUMO){
//                   //Si el humo no puede moverse arriba o diagonal va para los lados
//                     if(!sand_sim_mover_arriba_y_lados(state, HUMO, seHaMovidoFlags, x, y)){
//                         //Se mueve a la izquierda o derecha
//                         sand_sim_mover_izq_der(state, HUMO, seHaMovidoFlags, x, y);
//                     }   
                    
//                 }

//             }
//         }  

          
//         //*calculate time to render the grid
//         gettimeofday(&tval_after_sandsim, NULL);

//         timersub(&tval_after_sandsim, &tval_before_sandsim, &tval_result_sandsim);


//       //  #pragma omp single nowait
//        // {
//           //Calculo FPS
//           //Si ha pasado un segundo desde la ultima medicion
//         /*  if((tval_after_sandsim.tv_sec - getTimerS(TVAL_SANDSIM)) != 0){
//               fps_sandsim = fps_sandsim_cnt; //Capturar cuantas veces se ha ejecutado esta funcion (FPS)
//               fps_sandsim_cnt = 0; //Reiniciar la cuenta de FPS
//               avg_time_sandsim += time_sandsim_acum / (long int)fps_sandsim; //Tiempos de cada frame/ FPS = prom. tiempo ejecución por segundo
//               //SDL_Log("%lf",avg_time_sandsim);
//               time_sandsim_acum = 0; //Reiniciar el acumulador del tiempo
//               resetTimer(TVAL_SANDSIM); //Actualizar timer

//           } else{  //Si no ha pasado el segundo
//               ++fps_sandsim_cnt; //Ir sumando los frames
//               time_sandsim_acum += tval_result_sandsim.tv_usec; //Va acumulando todos los tiempos de cada frame
//           }
//         */

//           avg_time_sandsim += tval_result_sandsim.tv_usec;

//         char str[128];
//         sprintf(str, "Total time to execute function world_sand_sim (ms): %ld", 
//             (long int)tval_result_sandsim.tv_usec);
//         renderFormattedText(renderer, str, 0 , 40);
//         }
//       //}

//   }
//  // }  
    
}

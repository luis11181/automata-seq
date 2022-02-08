// gcc console_sandsim.c -o console_sandsim

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <mpi.h>

// N  o el tamaño del grid debe ser multiplo de 7
#define N 28 // 14 // Si es menor o igual a 80, se muestra en consola
//! reescribir variable nodos de abajo, dependiendo numero de procesos a correr
#define FRAMES 1000 // Número de veces que se ejecuta la función sandsim

#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

enum CELL
{
  BLACK, // 0
  WHITE,
  BLUE,
  RED,
  GRAY,
  YELLOW,
  PURPLE,
  GREEN,
  WHITEBLUE, // 176, 241, 247
  GRAYSMOKE,
  STRUCTURE
};

// Falling sand simulator - materiales básicos
#define AIR BLACK
#define ROCK GRAY
#define SAND YELLOW
#define WATER BLUE
#define OIL PURPLE
#define FIRE RED
#define CLOUD WHITEBLUE
#define HUMO GRAYSMOKE
#define ESTATICO STRUCTURE

//=================== CPU FUNCTIONS =======================//

void printBoard(char *name, u_int8_t *board, int n)
{
  char str[50] = "\n";
  strcat(str, name);
  strcat(str, "\n\n");
  printf("%s", str);
  if (n <= 80)
  {
    for (int y = 0; y < n; y++)
    {
      for (int x = 0; x < n; x++)
      {
        int idx = (y * n) + x;
        printf("%d,", board[idx]);
      }
      printf("\n");
    }
  }
  else
  {
    printf("Board too large to be shown in console\n");
  }
}

double cpuSecond()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}

bool sandsim_puede_moverse(u_int8_t *board, short sustancia, int x, int y, const int n)
{
  // Si las coordenadas se salen de los límites, no se puede mover por defecto
  time_t t;
  srand((unsigned int)time(&t));
  if (x < 0 || x > n - 1 || y < 0 || y > n - 1)
  {
    return false;
  }
  int idx = (y * n) + x;
  switch (sustancia)
  {
  case SAND:
    if (board[idx] == AIR)
    {
      return true;
    }
    else if (board[idx] == SAND)
    {
      return false;
    }
    else if (board[idx] == WATER && drand48() < 0.5)
    {
      return true;
    }
    else if (board[idx] == ROCK)
    {
      return false;
    }
    else if (board[idx] == OIL)
    {
      return true;
    }
    else if (board[idx] == HUMO)
    {
      return true;
    }

    break;

  case WATER:
    if (board[idx] == AIR)
    {
      return true;
    }
    else if (board[idx] == SAND)
    {
      return false;
    }
    else if (board[idx] == WATER)
    {
      return false;
    }
    else if (board[idx] == ROCK)
    {
      return false;
    }
    else if (board[idx] == OIL && drand48() < 0.5)
    {
      return true;
    }
    else if (board[idx] == HUMO)
    {
      return true;
    }

    break;

  case ROCK:
    if (board[idx] == AIR)
    {
      return true;
    }
    else if (board[idx] == SAND && drand48() < 0.2)
    {
      return true;
    }
    else if (board[idx] == WATER && drand48() < 0.9)
    {
      return true;
    }
    else if (board[idx] == ROCK)
    {
      return false;
    }
    else if (board[idx] == OIL && drand48() < 0.9)
    {
      return true;
    }
    else if (board[idx] == HUMO)
    {
      return true;
    }

    break;

    // No es necesario porque el aire nunca se mueve por si mismo
    /*case AIR:
      if(board[idx] == AIR){ return true;}
      break;*/

  case FIRE:
    if (board[idx] == AIR)
    {
      return true;
    }
    else if (board[idx] == SAND)
    {
      return false;
    }
    else if (board[idx] == WATER)
    {
      return true;
    }
    else if (board[idx] == ROCK)
    {
      return false;
    }
    else if (board[idx] == FIRE)
    {
      return false;
    }
    else if (board[idx] == OIL)
    {
      return true;
    }
    else if (board[idx] == HUMO)
    {
      return true;
    }
    break;

  case OIL:
    if (board[idx] == AIR)
    {
      return true;
    }
    else if (board[idx] == SAND)
    {
      return false;
    }
    else if (board[idx] == WATER)
    {
      return false;
    }
    else if (board[idx] == ROCK)
    {
      return false;
    }
    else if (board[idx] == OIL)
    {
      return false;
    }
    else if (board[idx] == HUMO)
    {
      return true;
    }

    break;

  case HUMO:
    if (board[idx] == AIR)
    {
      return true;
    }

    break;

  default:
    return false;
    break;
  }

  return false;
}

void sandsim_mover(u_int8_t *board, bool *has_moved, int fromX, int fromY, int toX, int toY, const int n)
{
  int idxFrom = (fromY * n) + fromX;
  int idxTo = (toY * n) + toX;
  short sustancia = board[idxFrom];
  short otraSustancia = board[idxTo];
  // Switch para las interacciones especiales
  switch (sustancia)
  {

  case FIRE:
    if (otraSustancia == OIL)
    {
      board[idxFrom] = FIRE;
      board[idxTo] = FIRE;
    }
    else if (otraSustancia == WATER)
    {
      board[idxFrom] = HUMO;
      board[idxTo] = WATER;
    }
    else
    {
      bool seDescompone = drand48() < 0.003;
      if (seDescompone)
      {
        board[idxFrom] = HUMO;
        board[idxTo] = otraSustancia;
      }
      else
      {
        board[idxFrom] = otraSustancia;
        board[idxTo] = FIRE;
      }
    }
    break;

  case HUMO:

    if (true)
    {
      bool seDescompone = drand48() < 0.05;
      if (seDescompone)
      {
        board[idxFrom] = AIR;
        board[idxTo] = otraSustancia;
      }
      else
      {
        board[idxFrom] = otraSustancia;
        board[idxTo] = sustancia;
      }
    }

    break;

  default:
    board[idxFrom] = otraSustancia;
    board[idxTo] = sustancia;
    break;
  }
  has_moved[idxFrom] = true;
  has_moved[idxTo] = true;
}

bool sandsim_mover_abajo(u_int8_t *board, u_int8_t sustancia, bool *has_moved, int x, int y, const int n)
{
  if (sandsim_puede_moverse(board, sustancia, x, y + 1, n))
  { // Mover abajo
    sandsim_mover(board, has_moved, x, y, x, y + 1, n);
    return true;
  }
  return false;
}

bool sandsim_mover_izq_der(u_int8_t *board, u_int8_t sustancia, bool *has_moved, int x, int y, const int n)
{
  // random number to define if it should go left or right
  bool primeroIzquierda = drand48() < 0.5;
  if (primeroIzquierda)
  {
    if (sandsim_puede_moverse(board, sustancia, x - 1, y, n))
    { // Mover a la izquierda
      sandsim_mover(board, has_moved, x, y, x - 1, y, n);
      return true;
    }
    else if (sandsim_puede_moverse(board, sustancia, x + 1, y, n))
    { // Mover a la derecha
      sandsim_mover(board, has_moved, x, y, x + 1, y, n);
      return true;
    }
  }
  else
  {
    if (sandsim_puede_moverse(board, sustancia, x + 1, y, n))
    { // Mover a la derecha
      sandsim_mover(board, has_moved, x, y, x + 1, y, n);
      return true;
    }
    else if (sandsim_puede_moverse(board, sustancia, x - 1, y, n))
    { // Mover a la izquierda
      sandsim_mover(board, has_moved, x, y, x - 1, y, n);
      return true;
    }
  }
  return false;
}

bool sandsim_mover_abajo_diagonal(u_int8_t *board, u_int8_t sustancia, bool *has_moved, int x, int y, const int n)
{
  // random number to define if it should go left or right
  bool primeroIzquierda = drand48() < 0.5;
  if (primeroIzquierda)
  {
    if (sandsim_puede_moverse(board, sustancia, x - 1, y + 1, n))
    { // Mover a la izquierda
      sandsim_mover(board, has_moved, x, y, x - 1, y + 1, n);
      return true;
    }
    else if (sandsim_puede_moverse(board, sustancia, x + 1, y + 1, n))
    { // Mover a la derecha
      sandsim_mover(board, has_moved, x, y, x + 1, y + 1, n);
      return true;
    }
  }
  else
  {
    if (sandsim_puede_moverse(board, sustancia, x + 1, y + 1, n))
    { // Mover a la derecha
      sandsim_mover(board, has_moved, x, y, x + 1, y + 1, n);
      return true;
    }
    else if (sandsim_puede_moverse(board, sustancia, x - 1, y + 1, n))
    { // Mover a la izquierda
      sandsim_mover(board, has_moved, x, y, x - 1, y + 1, n);
      return true;
    }
  }
  return false;
}

// x, y: Posición de la partícula que se mueve abajo o a los lados
bool sandsim_mover_abajo_y_lados(u_int8_t *board, u_int8_t sustancia, bool *has_moved, int x, int y, const int n)
{

  // Si no se puede mover hacia abajo
  if (!sandsim_mover_abajo(board, sustancia, has_moved, x, y, n))
  {
    // Se moverá en diagonal hacia abajo
    if (sandsim_mover_abajo_diagonal(board, sustancia, has_moved, x, y, n))
    {
      // Si se mueve en diagonal, retornar true
      return true;
    }
  }
  return false;
}

// x, y: Posición de la partícula que se mueve arriba o a los lados
bool sandsim_mover_arriba_y_lados(u_int8_t *board, u_int8_t sustancia, bool *has_moved, int x, int y, const int n)
{

  if (sandsim_puede_moverse(board, sustancia, x, y - 1, n))
  { // Mover arriba
    sandsim_mover(board, has_moved, x, y, x, y - 1, n);
    return true;
  }

  bool primeroIzquierda = drand48() < 0.5;

  if (primeroIzquierda)
  {
    if (sandsim_puede_moverse(board, sustancia, x - 1, y - 1, n))
    { // Mover a la izquierda
      sandsim_mover(board, has_moved, x, y, x - 1, y - 1, n);
      return true;
    }
    else if (sandsim_puede_moverse(board, sustancia, x + 1, y - 1, n))
    { // Mover a la derecha
      sandsim_mover(board, has_moved, x, y, x + 1, y - 1, n);
      return true;
    }
  }
  else
  {
    if (sandsim_puede_moverse(board, sustancia, x + 1, y - 1, n))
    { // Mover a la derecha
      sandsim_mover(board, has_moved, x, y, x + 1, y - 1, n);
      return true;
    }
    else if (sandsim_puede_moverse(board, sustancia, x - 1, y - 1, n))
    { // Mover a la izquierda
      sandsim_mover(board, has_moved, x, y, x - 1, y - 1, n);
      return true;
    }
  }

  return false;
}

void sandsim(u_int8_t *board, bool *has_moved, const int n, const int y0, const int y1)
{
  // for (int y = n - 1; y >= 0; y--)
  // sleep(1); // carga computacional de prueba
  for (int y = y0; y < y1; y++)
  {
    // se le puede poner un sleep() para agregar carga  computacional falsa
    for (int x = 0; x < n; x++)
    {
      int idx = (y * n) + x;
      if (!has_moved[idx])
      {
        if (board[idx] == SAND)
        {
          sandsim_mover_abajo_y_lados(board, SAND, has_moved, x, y, n);
        }

        if (board[idx] == ROCK)
        {
          sandsim_mover_abajo(board, ROCK, has_moved, x, y, n);
        }

        if (board[idx] == WATER)
        {

          // Si el agua no se puede mover abajo o a los lados
          if (!sandsim_mover_abajo_y_lados(board, WATER, has_moved, x, y, n))
          {
            // Se mueve a la izquierda o derecha
            sandsim_mover_izq_der(board, WATER, has_moved, x, y, n);
          }
        }

        if (board[idx] == OIL)
        {

          // Si el agua no se puede mover abajo o a los lados
          if (!sandsim_mover_abajo_y_lados(board, OIL, has_moved, x, y, n))
          {
            // Se mueve a la izquierda o derecha
            sandsim_mover_izq_der(board, OIL, has_moved, x, y, n);
          }
        }

        if (board[idx] == FIRE)
        {
          if (!sandsim_mover_abajo_y_lados(board, FIRE, has_moved, x, y, n))
          {
            bool seDescompone = drand48() < 0.2;
            if (seDescompone)
            {
              board[idx] = HUMO;
            }
          }
        }

        if (board[idx] == HUMO)
        {
          // Si el humo no puede moverse arriba o diagonal va para los lados
          if (!sandsim_mover_arriba_y_lados(board, HUMO, has_moved, x, y, n))
          {
            // Se mueve a la izquierda o derecha
            sandsim_mover_izq_der(board, HUMO, has_moved, x, y, n);
          }
        }
      }
    }
  }

  // printBoard((char *)"Trasformed board", board, n);
  // printBoard((char *)"Has moved array",(u_int8_t *)has_moved, n);
}

void initData(u_int8_t *board, int n)
{

  for (int x = 0; x < N; x++)
  {
    for (int y = 0; y < N; y++)
    {
      int idx = (y * n) + x;
      board[idx] = AIR;

      // make rock is the sum is small
      if (y > (N - (N / 2.1)))
      { // make the sea
        board[idx] = WATER;
      }
      else if (y > (N - (N / 1.05)))
      { // make sand
        board[idx] = SAND;
      }
    }
  }

  // printBoard((char *)"Initial board", board, n);
}

void resetBoolData(bool *has_moved, int n)
{

  for (int i = 0; i < n * n; i++)
  {
    has_moved[i] = false;
  }
}

int main(int argc, char **argv)
{

  //! INITIALIZATION MPI

  MPI_Init(&argc, &argv);

  int tag = 1, tasks, iam;

  // char message[N];

  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &tasks);

  MPI_Comm_rank(MPI_COMM_WORLD, &iam);

  //!!!!!!!!!!!!!!!!!!!END INITIALIZATION MPI
  printf("%s Starting...\n", argv[0]);

  // set up data size of board
  int nElem = N * N;
  printf("Board size (n * n): %d\n", nElem);
  size_t nBytesBoards = nElem * sizeof(u_int8_t);
  size_t nBytesBool = nElem * sizeof(bool);
  u_int8_t *h_board;
  bool *h_has_moved;
  h_board = (u_int8_t *)malloc(nBytesBoards);
  h_has_moved = (bool *)malloc(nBytesBool);
  // Inicializar los timers
  double iStart, iElaps, iElapsHost, iElapsGPU;
  // initialize data at host side
  initData(h_board, N);
  memset(h_has_moved, 0, nBytesBool);
  // add board on host for result checks
  iStart = cpuSecond();
  // sandsim(h_board, h_has_moved, N);

  //! definir que porcion del loop hara cada uno de los nodos  mpi

  int y0, y1;
  // int valor0, valor1;
  int nodos = 7;
  // u_int8_t *h_board_piece = malloc(nElem / nodos * sizeof(u_int8_t));

  if (iam != 0)
  {
    // N  o el tamaño del grid debe ser multiplo de 7
    y0 = N / nodos + (N / nodos * (iam - 2)); // loos nodos 1,2,3,4,5,6,7 tendrn una porcion del grid
    y1 = N / nodos + (N / nodos * (iam - 1));
    // indices de los valores que se van a enviar a cada nodo esclavo
    // valor0 = nElem / nodos + (nElem / nodos * (iam - 2)); // valores del array general que corresponderan a cada nodo escalvo
    // valor1 = nElem / nodos + (nElem / nodos * (iam - 1)) - 1;
    // se copia el valor del array en las matrices pequeñas de cada nodo
    // memcpy(h_board_piece, h_board + valor0, nElem / 7 * sizeof(u_int8_t));
  }

  // se definene los arrays vacios en los que el nodo maestro recibe los resultados de cada esclavo
  // int *result_h_board_piece1 = malloc(nElem / nodos * sizeof(u_int8_t));
  // int *result_h_board_piece2 = malloc(nElem / nodos * sizeof(u_int8_t));
  // int *result_h_board_piece3 = malloc(nElem / nodos * sizeof(u_int8_t));
  // int *result_h_board_piece4 = malloc(nElem / nodos * sizeof(u_int8_t));
  // int *result_h_board_piece5 = malloc(nElem / nodos * sizeof(u_int8_t));
  // int *result_h_board_piece6 = malloc(nElem / nodos * sizeof(u_int8_t));
  // int *result_h_board_piece7 = malloc(nElem / nodos * sizeof(u_int8_t));

  for (int frames = 0; frames < FRAMES; frames++)
  {
    // printf("\e[1;1H\e[2J"); //clear screen
    // system("clear"); /*clear output screen*/

    // if (iam == 0)
    // {
    //   // gotoxy(0, 0);
    //   // printf("=================FRAME %d=================                                            \n", frames);
    // }

    resetBoolData(h_has_moved, N);

    if (iam != 0)
    {
      // se copia el valor del array en las matrices pequeñas de cada nodo
      // memcpy(h_board_piece, h_board + valor0, nElem / nodos * sizeof(u_int8_t));
      // printf("\n \n-------- llamada a sansim de los nodos----- %d-----\n", iam);
      sandsim(h_board, h_has_moved, N, y0, y1);
      //! enviar el resultado de la operacion al nodo 0 maestro
      // MPI_Send( void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator)
      //  printf("\n \n-------- mpi send de los nodos---- %d-----\n", iam);
      MPI_Send(h_board + ((nElem / nodos) * (iam - 1)), nElem / nodos, MPI_INT, 0, tag, MPI_COMM_WORLD);
      //  printf("\n \n-------- mpi barrier de los nodos al ya enviar por primera vez el resultado parcial----- %d-----\n", iam);
      MPI_Barrier(MPI_COMM_WORLD);
    }
    else
    {
      // for (int i = 1; i < tasks; i++)

      // MPI_Recv( void* data,  int count,  MPI_Datatype datatype,  int source,  int tag,  MPI_Comm communicator, MPI_Status* status)
      //! una opcion es copiar directamente los resultados a la larte del array que corresponde
      // MPI_Recv(h_board, nElem / 7, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
      // MPI_Recv(h_board + 700, nElem / 7, MPI_INT, 2, tag, MPI_COMM_WORLD, &status);
      //! otra opcion es pegar en una variable intermedia y luego alocar en el array general
      //   printf("\n \n-------- mpi barrier del master antes de recibir por primera vez el los resultados parciales---- %d-----\n", iam);
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Recv(h_board, nElem / nodos, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(h_board + (nElem / nodos), nElem / nodos, MPI_INT, 2, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(h_board + (nElem / nodos * 2), nElem / nodos, MPI_INT, 3, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(h_board + (nElem / nodos * 3), nElem / nodos, MPI_INT, 4, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(h_board + (nElem / nodos * 4), nElem / nodos, MPI_INT, 5, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(h_board + (nElem / nodos * 5), nElem / nodos, MPI_INT, 6, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(h_board + (nElem / nodos * 6), nElem / nodos, MPI_INT, 7, tag, MPI_COMM_WORLD, &status);

      //    printf("\n \n-------- mpi receive del master, acabo de recibir todos---- %d-----\n", iam);

      // //! copy the result of the small arrays into the big array h_board
      // memcpy(h_board, result_h_board_piece1, nElem / nodos * sizeof(u_int8_t));
      // memcpy(h_board + nElem / nodos, result_h_board_piece2, nElem / nodos * sizeof(u_int8_t));
      // memcpy(h_board + nElem / nodos * 2, result_h_board_piece3, nElem / nodos * sizeof(u_int8_t));
      // memcpy(h_board + nElem / nodos * 3, result_h_board_piece4, nElem / nodos * sizeof(u_int8_t));
      // memcpy(h_board + nElem / nodos * 4, result_h_board_piece5, nElem / nodos * sizeof(u_int8_t));
      // memcpy(h_board + nElem / nodos * 5, result_h_board_piece6, nElem / nodos * sizeof(u_int8_t));
      // memcpy(h_board + nElem / nodos * 6, result_h_board_piece7, nElem / nodos * sizeof(u_int8_t));
    }

    // barrera para sincronizar todos los nodos luego de que maetro recibe la informacion
    //  printf("\n \n--------primera mpi barrier general, luego de ya enviar y recibir la informacion parcial---- %d-----\n", iam);
    //! MPI_Barrier(MPI_COMM_WORLD);

    if (iam == 0)
    {
      //    printf("\n \n-------- mpi send del master a los nodos con el h_board resultante---- %d-----\n", iam);
      for (int i = 1; i < tasks; i++)
        MPI_Send(h_board, nElem, MPI_INT, i, tag, MPI_COMM_WORLD);
      //    printf("\n \n-------- mpi barrier del master al enviar el resultado global a cada sub matrix----- %d-----\n", iam);
      MPI_Barrier(MPI_COMM_WORLD);
    }

    if (iam != 0)
    {
      //    printf("\n -------- mpi barrier de nodos, al leer el hboard resultante---- %d-----\n", iam);
      MPI_Barrier(MPI_COMM_WORLD);
      // printf("\n \n-------- mpi recv de los nodos, leyendo el hboard resultante---- %d-----\n", iam);
      MPI_Recv(h_board, nElem, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
      //    printf("\n -------- nodos, acabarond de leer hboard resultante---- %d-----\n", iam);
    }

    // barrera para sincronizar todos los nodos luego de que todos los nodos quedan con la misma informacion
    //  printf("\n-------- segunda mpi barrier general, al acabar el loop----- %d-----\n", iam);
    //! MPI_Barrier(MPI_COMM_WORLD);

    // send result of the sandsim to master

    // if (iam == 0)
    // {
    //   // gotoxy(10, 0);
    //   //printBoard((char *)"Result board", h_board, N);

    //   // memcpy(h_board + valor0, h_board_piece, nElem / 7 * sizeof(u_int8_t));
    // }
    //   printf("\n--------sleep de 1 seg----- %d-----\n", iam);
    // sleep(1); //! Sólo para revisar el movimiento, quitar para los benchmarks
  }

  printf("\n \n--------  get the time  %d-----\n", iam);

  iElaps = cpuSecond() - iStart;
  iElapsHost = iElaps;

  if (iam == 0)
  {
    printf("\n \n \n------------------------------- \n \n Time elapsed on Host %f sec\n \n \n \n \n ------------------------------------------------\n", iElaps);
  }

  printf("\n \n--------  final libera memoria  %d-----\n", iam);
  free(h_board);
  free(h_has_moved);
  // free(h_board_piece);
  // free(result_h_board_piece1);
  // free(result_h_board_piece2);
  // free(result_h_board_piece3);
  // free(result_h_board_piece4);
  // free(result_h_board_piece5);
  // free(result_h_board_piece6);
  // free(result_h_board_piece7);

  MPI_Finalize();
  return (0);
}
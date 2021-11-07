#ifndef LOGIC_H_
#define LOGIC_H_

#define N 50
#define SCREEN_WIDTH 800.0
#define SCREEN_HEIGHT 800.0
#define CELL_WIDTH (SCREEN_WIDTH / N)
#define CELL_HEIGHT (SCREEN_HEIGHT / N)
#define MOVES_PER_FRAME 1
#define MOVES_PER_SECOND 20

enum AUTOMATA {
    LANGTONS_ANT,
    GAME_OF_LIFE,
    BRIANS_BRAIN,
    WIREWORLD,
    FALLING_SAND_SIM,
};

enum MODE {
    RUNNING_MODE,
    PAUSED_MODE,
    QUIT_MODE
};

enum CELL {
    BLACK,
    WHITE,
    BLUE,
    RED,
    GRAY,
    YELLOW,
    PURPLE,
    GREEN,
    WHITEBLUE, // 176, 241, 247
};

/* CELL NAME MACROS */

// langtons un solo punto se mueve de forma aleatoria y va creando estructuras

// The Game of Life se mueve automatico toda la estructura de forma aleatoria
#define DEAD BLACK
#define ALIVE WHITE

//  Brian's brain poner nuestros cuadritos impresos donde se hace click
#define OFF BLACK
#define ON WHITE
#define DYING BLUE

// Wireworldd poner nuestros cuadritos impresos donde se hace click
#define EMPTY BLACK
#define CONDUCTOR WHITE
#define ELECTRON_HEAD BLUE
#define ELECTRON_TAIL RED

// Falling sand simulator - materiales básicos
#define AIR BLACK
#define ROCK GRAY
#define SAND YELLOW
#define WATER BLUE
#define OIL PURPLE
#define FIRE RED
#define CLOUD WHITEBLUE


typedef struct {
    int x;
    int y;
    int dir;
} position;

typedef struct {
    int board[N][N];
    int mode;
    position ant;
} state_t;

enum ORIENTATION {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

#endif // LOGIC_H_

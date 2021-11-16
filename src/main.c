#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h> 
#include <SDL2/SDL.h>
//#include "SDL_ttf.h"
#include "/usr/include/SDL2/SDL_ttf.h"

#include "util.h"
#include "logic.h"
#include "render.h"

#define SECONDS_TO_MICROSECONDS 1000000

void print_usage()
{
    printf("Usage: ./automata AUTOMATA\n");
    printf("     Langton's ant           -> langton\n");
    printf("     Conway's Game of Life   -> gameoflife\n");
    printf("     Falling Sand Simulator  -> sandsim\n");
}

int main(int argc, char **argv)
{
    int automata;//! automata defines the simulation we are running (see logic.h)
    char running_title[64] = {'\0'};
    char paused_title[64] = {'\0'};

    if (argc < 2) {
        print_usage();
        return EXIT_FAILURE;
    } else if (strcmp(argv[1], "langton") == 0) {
        automata = LANGTONS_ANT;
        strncat(running_title, "LANGTONS_ANT", 48);
        strncat(paused_title, "LANGTONS_ANT", 48);
    } else if (strcmp(argv[1], "gameoflife") == 0) {
        automata = GAME_OF_LIFE;
        strncat(running_title, "THE GAME OF LIFE", 48);
        strncat(paused_title, "THE GAME OF LIFE", 48);
    } else if (strcmp(argv[1], "sandsim") == 0) {
        automata = FALLING_SAND_SIM;
        strncat(running_title, "FALLING SAND SIMULATOR", 48);
        strncat(paused_title, "FALLING SAND SIMULATOR", 48);
    } else {
        fprintf(stderr, "No such automata.\n");
        print_usage();
        return EXIT_FAILURE;
    }

    strncat(running_title, " - RUNNING", 48);
    strncat(paused_title, " - PAUSED", 48);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_INIT Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    //inicia ttf para los mensajes
    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    	
    // load font.ttf at size 16 into font
    TTF_Font *font;
    font=TTF_OpenFont("/usr/share/fonts/truetype/ubuntu/Ubuntu-M.ttf", 16);
    if(!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }

    SDL_Window *window = SDL_CreateWindow(running_title, SDL_WINDOWPOS_UNDEFINED,
                                                                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                                                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    state_t state = {.mode = RUNNING_MODE};

     //! INIT BOARD AWITCHES THE GAME ACORDING TO THE AUTOMATA VARIABLES THAT WAS SELECTRED AT THE START
    // INIT BOARD
    switch (automata) {
        case LANGTONS_ANT:
            state.ant.x = N / 2;
            state.ant.y = N / 2;
            state.ant.dir = LEFT;
            break;

        case GAME_OF_LIFE:
            for (int x = 0; x < N; x++)
                for (int y = 0; y < N; y++)
                    state.board[x][y] = BLACK;

            // GLIDER
            state.board[N / 2][N / 2] = WHITE;
            state.board[N / 2 + 1][N / 2] = WHITE;
            state.board[N / 2 + 2][N / 2] = WHITE;
            state.board[N / 2 + 1][N / 2 - 2] = WHITE;
            state.board[N / 2 + 2][N / 2 - 1] = WHITE;
            break;

         //* initial state of the world
        case FALLING_SAND_SIM:
            for (int x = 0; x < N; x++){
                for (int y = 0; y < N; y++){
              
                    //make rock is the sum is small
                    if (y > (N-(N/10))) {
                        state.board[x][y] = ROCK;
                    } else if (y > (N-(N/5))) { //make the sea
                        state.board[x][y] = BLUE;
                    } else if (y < N/10) { //make sand
                        state.board[x][y] = SAND;
                    } else { //everithing else is air
                        state.board[x][y] = AIR;
                    }
                    
                    if (y < (N-(N*0.75))&& y > (N-(N*0.8)) && x < (N-(N*0.25)) && x > (N-(N*0.3) )) { //make sand
                        state.board[x][y] = ESTATICO;
                    } 
                }
            } 
                      
            // print the matrix state,board in the console to test
            for (int x = 0; x < N; x++){
                for (int y = 0; y < N; y++){
                    //printf("%d", state.board[x][y]);
                }
                //printf("\n");
            }
          break;

        default:
            for (int x = 0; x < N; x++)
                for (int y = 0; y < N; y++)
                    state.board[x][y] = AIR;

            break;
    }

     //! event infinite loop, to switch events
    // is a event is fired it executes the corresponding function
    SDL_Event event;
    bool draw;
    int drawing_element = FIRE;
    int brushSize = 2;

    char dest[200]= "Fire";
       

    while (state.mode != QUIT_MODE) {
      //! while loop to search for events and handle them doing an action dependong on the game
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                
                case SDL_QUIT:
                    state.mode = QUIT_MODE;
                    break;

                case SDL_MOUSEBUTTONDOWN:

                  draw= true;

                    if (automata!=FALLING_SAND_SIM)
                    {
                      state.mode = PAUSED_MODE;
                      SDL_SetWindowTitle(window, paused_title);
                    }
                
                    int x = event.button.x / CELL_WIDTH;
                    int y = event.button.y / CELL_HEIGHT;
                    

                    // TOGGLES BETWEEN EACH ELEMENT TYPE WITH EACH CLICK
                    switch (automata) {
                        case GAME_OF_LIFE:
                            state.board[x][y] = (state.board[x][y] + 1) % 2;
                            break;
                        // USE MODULE 9 TO ONLY GET A NUMBER BETWEEN 0 AND 9 THAT ARE HE NUMBER OF COLORS
                        case FALLING_SAND_SIM:
                        // alter teh state of pixel with each click
                          if (draw)
                          {
                            int mouseix = event.motion.x;
                            int mouseiy = event.motion.y;
                            int mousex = mouseix / CELL_WIDTH;
                            int mousey = mouseiy / CELL_HEIGHT;
                           
                             for(int y = max(0,mousey-brushSize); y < min(N-1, mousey+brushSize); ++y){
                              for(int x = max(0,mousex-brushSize); x < min(N-1, mousex+brushSize); ++x){
                                state.board[x][y] = drawing_element;
                              }
                           }}
                            
                        break;
                    }
                  break;

                // if the click is pressed and there is movement the mouse will draw any picture
                case SDL_MOUSEMOTION:
                    if (draw)
                    {
                        int mouseix = event.motion.x;
                        int mouseiy = event.motion.y;
                        int mousex = mouseix / CELL_WIDTH;
                        int mousey = mouseiy / CELL_HEIGHT;
                        
                        
                        for(int y = max(0,mousey-brushSize); y < min(N-1, mousey+brushSize); ++y){
                              for(int x = max(0,mousex-brushSize); x < min(N-1, mousex+brushSize); ++x){
                                 state.board[x][y] = drawing_element;
                              }
                            }

                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    draw=false;
                    break;
               
                //event if left is used
                case SDLK_LEFT:
                        //do something with left arrow
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == ' ') {
                        state.mode = RUNNING_MODE + PAUSED_MODE - state.mode;
                        SDL_SetWindowTitle(window, state.mode ? paused_title : running_title);
                    }  //*makes a meteorite effect if m or M are pressed 
                    else if (event.key.keysym.sym == 'm' || event.key.keysym.sym == 'M') {
                      int random = rand() % 19;
                        for (int x = N/20*random; x < N/20*random + N/20; x++)
                            for (int y = 0; y < N/20; y++)
                                state.board[x][y] = (rand() % 2) ? ROCK : FIRE;
                    }           
                    else if (event.key.keysym.sym == 'f' || event.key.keysym.sym == 'F') {
                      drawing_element = FIRE;
                      
                      char aa[]= "Fire";
                      strcpy(dest,aa);
                 
                    }                                       
                    else if (event.key.keysym.sym == 's' || event.key.keysym.sym == 'S') {
                      drawing_element = SAND;
                      char bb[]= "Sand";
                      strcpy(dest,bb);
                    }  
                    else if (event.key.keysym.sym == 'a' || event.key.keysym.sym == 'A') {
                      drawing_element = AIR;
                      char cc[]= "Air";
                      strcpy(dest,cc);
                    }                              
                    else if (event.key.keysym.sym == 'r' || event.key.keysym.sym == 'R') {
                      drawing_element = ROCK;
                      char dd[]= "Rock";
                      strcpy(dest,dd);
                    }
                    else if (event.key.keysym.sym == 'w' || event.key.keysym.sym == 'W') {
                      drawing_element = WATER; 
                      char ee[]= "Water";
                      strcpy(dest,ee);
                  } 

                    else if (event.key.keysym.sym == 'h' || event.key.keysym.sym == 'H') {
                      drawing_element = HUMO;
                      char ff[]= "Humo";
                      strcpy(dest,ff);
                   } 
                    else if (event.key.keysym.sym == 'e' || event.key.keysym.sym == 'E') {
                      drawing_element = ESTATICO; 
                      char gg[]= "Estatico";
                      strcpy(dest,gg);} 
                    else if (event.key.keysym.sym == 'o' || event.key.keysym.sym == 'O') {
                      drawing_element = OIL;
                      char hh[]= "Oil";
                      strcpy(dest,hh); } 
                    else if (event.key.keysym.sym == '+' ) {
                      //print brush size
                      if (brushSize<N/5)
                      {
                       brushSize =  brushSize + 1;
                      }
                      
                    }
                    else if (event.key.keysym.sym == '-') {
                      if (brushSize>1)
                      {
                       brushSize =  brushSize - 1;
                      }
                       }
                      
                break;}
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        render_grid(renderer, &state);

        if (state.mode == RUNNING_MODE)
            usleep((1.0 / MOVES_PER_SECOND) * SECONDS_TO_MICROSECONDS);

        switch (automata) {
            case LANGTONS_ANT:
                langtons_ant(renderer, &state);
                break;

            case GAME_OF_LIFE:
                game_of_life(renderer, &state);
                break;

            case FALLING_SAND_SIM:
                world_sand_sim(renderer, &state);
                break;
        }



         
        SDL_Color font_color = { .r = 6, .g = 150, .b = 78 };
        //render text on screen with SDL with the element that is being drawn 
        SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, dest, font_color); //
        SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); // now you can convert it into a texture
        SDL_Rect Message_rect;
        Message_rect.x = 0;
        Message_rect.y = 0;
        Message_rect.w = surfaceMessage->w;
        Message_rect.h = surfaceMessage->h;
        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
        //SDL_FreeSurface(surfaceMessage);
        //SDL_DestroyTexture(Message);

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyWindow(window);
    TTF_Quit();
    // you could SDL_Quit(); here...or not.
    SDL_Quit();

    return EXIT_SUCCESS;
}

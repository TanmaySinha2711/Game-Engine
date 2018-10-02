#include "Game.h"
#include <iostream>
#include<string>
#include<sstream>
#include "SDL.h"
#include "Game.h"
#include "breakout.h"
#include "scene.h"
#include "SDL_mixer.h"
#include "fps.h"
#include<stdio.h>
#include "menu.h"
using namespace std;

int lan=0;

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

Game::Game()
{
    window = NULL;
    renderer = NULL;
    surface = NULL;
    mainMenu = menu();
    font = NULL;
}

Game::~Game()
{

}

bool Game::Init()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    if(SDL_Init(SDL_INIT_VIDEO) <0 || TTF_Init() < 0)
    {
        std::cout <<"SDL could not be initialized" << std::endl;
        return 0;
    }
    window = SDL_CreateWindow("Breakout",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED |SDL_RENDERER_PRESENTVSYNC);
    // Surface of window
    surface = SDL_GetWindowSurface(window);

    // Loading the font from the file
    font = TTF_OpenFont("./assets/Font2.ttf", 35);

    return true;
}

void Game::NewGame()
{
    Mix_Chunk *music=NULL;
    Mix_Chunk *collision=NULL;
    Scene scene(renderer);

    int i = mainMenu.displayMenu(window,surface,font,lan);

    if(i == 0) {
        //initialize audio/video
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
            std::cout << "SDL_Init Error: " << std::endl;
            return;
        }


        cerr<<"\nIntializing Window\n";

        if (window == nullptr) {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return;
        }
        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
           printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
           SDL_Quit();
        }

        cerr<<"\nInitializing Renderer\n";

        if (renderer == nullptr) {
            SDL_DestroyWindow(window);
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return;
        }

        //Load music
        music = Mix_LoadWAV( "./assets/music.mp3" );
        if( music == NULL ){
            printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
            SDL_Quit();
        }

        collision=Mix_LoadWAV( "./assets/collision.mp3" );
        if( collision == NULL ){
            printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        }

        SDL_Event e;
        bool quit = false;
        bool paused=false;
        int vol1,vol2;
        fps f;
        int del = 10;
        vol1=Mix_PlayChannel(-1,music,-1);


        if(Mix_PausedMusic()==1)
            Mix_ResumeMusic();

            Uint32 time;
        while (!quit) {
            Mix_Volume(vol1,32);
            time = SDL_GetTicks();
            if(scene.collide){
               vol2=Mix_PlayChannel(-1,collision,0);
                Mix_Volume(vol2,128);
            }

            while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || scene.life<1) {
                    cerr<<"\nGame Over!!\n";
                    scene.paddle_reset();
                    quit = true;
                    break;
            }

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        quit = true;
                        break;
                    case SDLK_LEFT:
                        scene.move_paddle_keyboard(-KEY_MOVE_STEP);
                        break;
                    case SDLK_a:
                        scene.move_paddle_keyboard(-KEY_MOVE_STEP);
                        break;
                    case SDLK_d:
                        scene.move_paddle_keyboard(KEY_MOVE_STEP);
                        break;
                    case SDLK_RIGHT:
                        scene.move_paddle_keyboard(KEY_MOVE_STEP);
                        break;
                    case SDLK_p:
                        if(paused==false){
                            paused=true;
                            cerr<<"game paused"<<endl;
                        }
                        else{
                            paused=false;
                        }
                        break;
                    }
                }
                if (e.type == SDL_MOUSEMOTION) {
                    scene.move_paddle(e.motion.x - PAD_WIDTH / 2);
                }

            }

            if(!paused){

                const char *c;
                c = "Breakout - Dungeon_master ";
                SDL_SetWindowTitle(window, c);

                if(scene.life>0){
                    scene.render(font,lan);
                }
                int k = 0;
                if(scene.score%((BRICK_COLS*(BRICK_ROWS+scene.level))+k)==0 && scene.score!=0){
                    //this->NewGame();
                    if(scene.level>3){

                        mainMenu.youLost(window,surface,font,1,lan);
                        SDL_Delay(2000);
                        this->NewGame();
                    }
                    k += BRICK_COLS;
                    mainMenu.youLost(window,surface,font,3,lan);
                    SDL_Delay(2000);
                    SDL_FillRect(surface,&surface->clip_rect,SDL_MapRGB(surface->format, 0x00,0x00,0x00));
                    scene.bricks.clear();
                    scene.level++;
                    scene.CreateLevel();
                    scene.paddle_reset();
                    scene.launch_ball();
                    scene.render(font,lan);

                }
                if(scene.life==0){

                    mainMenu.youLost(window,surface,font,0,lan);
                    SDL_Delay(2000);
                    this->NewGame();
                }
            }

        }

        std::cout << "Quitting" << std::endl;
        SDL_DestroyRenderer(renderer);
        renderer=NULL;
        Mix_FreeChunk( music );
        music = NULL;
        Mix_FreeChunk(collision);
        collision=NULL;
        Mix_Quit();
        SDL_DestroyWindow(window);
        window=NULL;
        SDL_Quit();
    }

    else if(i == 1){
        int j;
        j =  mainMenu.displaySettingsMenu(window,surface,font,lan);
        if(j==0){
            lan=0;

            this->NewGame();
        }
        else{
            lan=1;

            this->NewGame();
        }

    }

    else if(i == 2){

        SDL_Quit();
    }

}

void Game::delay(Uint32 time)
{
    if(1000/60 > (SDL_GetTicks() - time))
        {
            SDL_Delay(1000/60 - (SDL_GetTicks() - time));
        }
}


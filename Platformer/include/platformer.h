#include<iostream>
#include<sstream>
#include<string>
#include "SDL.h"
#include "scene.h"
#include "SDL_mixer.h"
#include<stdio.h>
#include "menu.h"
#include "SDL_ttf.h"
#include<weather.h>


#define SCREEN_X 100
#define SCREEN_Y 50
#define SCREEN_DIM 23*32
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

class Platformer
{
    Mix_Music *music = NULL;
    Mix_Chunk *jump_sound = NULL;
    Mix_Chunk *fire_sound = NULL;
    Mix_Chunk *death = NULL;
    menu* mainMenu;
    WindowResolution res;
    bool weatherOption[5] = {false, false, false, false, false};
public:
    SDL_Window* window;
    SDL_Renderer *renderer;
    SDL_Surface* surface;
    TTF_Font* font;
    bool Init();
    void load();
    void runGame();
    void menuDisplay();
    Scene* scene;
    Weather* weather;
    float w_speed;
    void annihilate();
};

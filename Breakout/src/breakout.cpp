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
using namespace std;

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

int main(int argc, char **argv) {

    Game* game = new Game();

    if(game->Init())
    {
        game->NewGame();
    }

    delete game;
    return 0;


}

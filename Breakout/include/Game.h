#ifndef GAME_H
#define GAME_H
#include "SDL.h"
#include "SDL_ttf.h"
#include "menu.h"
#include "SDL_mixer.h"
class Game
{
    public:
        Game();
        virtual ~Game();
        SDL_Window* window;
        SDL_Surface* surface;
        SDL_Renderer* renderer;
        //Scene scene;
        TTF_Font* font;
        menu mainMenu = menu();

        // INITIALIZE Game
        bool Init();

        void NewGame();

        //void Play (Mix_Music *music,Mix_Chunk *collision,Scene scene);

        void delay(Uint32 time);


    protected:

    private:
};

#endif // GAME_H

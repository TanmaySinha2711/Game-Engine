#ifndef MENU_H
#define MENU_H

#include "SDL_ttf.h"
class menu
{
    public:
        menu();
        virtual ~menu();
        int displayMenu(SDL_Window* window,SDL_Surface* surface, TTF_Font* font,int lan);
        int displaySettingsMenu(SDL_Window* window, SDL_Surface* surface, TTF_Font* font,int lan);
        int getMenuOptions(SDL_Window* window, SDL_Surface* surface, TTF_Font* font, const int OPTIONS,int lan);
        void youLost(SDL_Window* window, SDL_Surface* surface, TTF_Font* font, int result,int lan);
        void delay(Uint32 time);
    protected:

    private:
};


#endif // MENU_H

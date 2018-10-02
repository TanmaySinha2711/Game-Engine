#include "menu.h"
#include "SDL.h"
#include "string"
#include <iostream>
#include<SDL_ttf.h>
#include <iostream>
#include<sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

menu::menu()
{

}

menu::~menu()
{
    //dtor
}

int menu::displayMenu(SDL_Window* window, SDL_Surface* surface, TTF_Font* font,int lan)
{
    const int OPTIONS = 3;
    int i = getMenuOptions(window, surface,font,OPTIONS,lan);
    return i;
}

int menu::displaySettingsMenu(SDL_Window* window, SDL_Surface* surface,TTF_Font* font,int lan)
{
    const int OPTIONS = 2;
    int i = getMenuOptions(window,surface,font,OPTIONS,lan);
    return i;
}

int menu::getMenuOptions(SDL_Window* window, SDL_Surface* surface, TTF_Font* font, const int OPTIONS,int lan)
{
    Uint32 time;
    int x,y;
    SDL_Color color[2] = {{255,0,0},{0,255,0}};
    SDL_Rect position[OPTIONS];
    SDL_Surface* MENU_ITEMS[OPTIONS];
    bool isSelected[OPTIONS];
    const char* MENU_OPTIONS[OPTIONS];
    if(OPTIONS==3)
    {
        if(lan==0){
            MENU_OPTIONS[0] = "Play";
            MENU_OPTIONS[1] = "Language";
            MENU_OPTIONS[2] = "Exit";
        }
        else{
            MENU_OPTIONS[0] = "Jugar";
            MENU_OPTIONS[1] = "Idioma";
            MENU_OPTIONS[2] = "Salida";
        }
        for(int i =0; i < OPTIONS; ++i)
        {
            isSelected[i] = 0;
            position[i].x = 200;
            MENU_ITEMS[i] = TTF_RenderText_Solid(font,MENU_OPTIONS[i],color[0]);
        }
        position[0].y = 250;
        position[1].y = 300;
        position[2].y = 350;
    }

    else if(OPTIONS==4)
    {
        MENU_OPTIONS[0] = "Play";
        MENU_OPTIONS[1] = "Language";
        MENU_OPTIONS[2] = "Exit";
        MENU_OPTIONS[3] = "Resume";
        for(int i =0; i < OPTIONS; ++i)
        {
            isSelected[i] = 0;
            position[i].x = 200;
            MENU_ITEMS[i] = TTF_RenderText_Solid(font,MENU_OPTIONS[i],color[0]);
        }
        position[0].y = 250;
        position[1].y = 300;
        position[2].y = 350;
        position[3].y = 200;
    }

    else if (OPTIONS == 2)
    {
        if(lan==0){
            MENU_OPTIONS[0] = "English";
            MENU_OPTIONS[1] = "Spanish";
        }
        else{
            MENU_OPTIONS[0] = "Inglés";
            MENU_OPTIONS[1] = "Español";
        }
        for(int i =0; i < OPTIONS; ++i)
        {
            isSelected[i] = 0;
            position[i].x = 200;
            MENU_ITEMS[i] = TTF_RenderText_Solid(font,MENU_OPTIONS[i],color[0]);
        }
        position[0].y = 300;
        position[1].y = 350;
    }

    SDL_FillRect(surface,&surface->clip_rect,SDL_MapRGB(surface->format, 0x00,0x00,0x00));
    SDL_Event e;

    while(1)
    {
        time = SDL_GetTicks();
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    for(int i = 0; i< OPTIONS;++i)
                    {
                        SDL_FreeSurface(MENU_ITEMS[i]);
                    }
                return 2;

                case SDL_MOUSEMOTION:
                    x = e.motion.x;
                    y = e.motion.y;
                    for(int i = 0; i < OPTIONS; ++i)
                    {
                        if(x >= position[i].x && x <= position[i].x + position[i].w
                        && y >= position[i].y && y <= position[i].y + position[i].h)
                        {
                            if(!isSelected[i])
                            {
                                isSelected[i] = 1;
                                SDL_FreeSurface(MENU_ITEMS[i]);
                                MENU_ITEMS[i] = TTF_RenderText_Solid(font,MENU_OPTIONS[i], color[1]);
                            }
                        }
                        else
                        {
                            if(isSelected[i])
                            {
                                isSelected[i] = 0;
                                SDL_FreeSurface(MENU_ITEMS[i]);
                                MENU_ITEMS[i] = TTF_RenderText_Solid(font,MENU_OPTIONS[i], color[0]);
                            }
                        }
                    }
                break;
                case SDL_MOUSEBUTTONDOWN:
                    x = e.button.x;
                    y = e.button.y;
                    for(int i = 0; i < OPTIONS; i++)
                    {
                        if(x >= position[i].x && x <= position[i].x + position[i].w
                        && y >= position[i].y && y <= position[i].y + position[i].h)
                        {
                            for(int j = 0; j < OPTIONS;++j)
                            {
                                SDL_FreeSurface(MENU_ITEMS[j]);
                            }
                            return i;
                        }
                    }
                break;
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym==SDLK_ESCAPE || e.key.keysym.sym==SDLK_q)
                    {
                        for(int i = 0; i < OPTIONS;++i)
                        {
                            SDL_FreeSurface(MENU_ITEMS[i]);
                        }
                        return 2;
                    }
            }
        }
        for(int i = 0; i < OPTIONS;++i)
        {
            SDL_BlitSurface(MENU_ITEMS[i], NULL, surface, &position[i]);
        }
        SDL_UpdateWindowSurface(window);

    }
    return 0;
}

void menu::youLost(SDL_Window* window, SDL_Surface* surface, TTF_Font* font, int result,int lan)
{
    int x, y;
    SDL_Color color = {255, 0 ,0};
    SDL_Surface* lose;
    const char* lost;
    SDL_Rect rect;
    if(result == 0)
    {
        if(lan==0){
            lost = "You lose";
        }
        else
            lost="Tú pierdes";
    }
    else if(result == 1)
    {
        if(lan==0){
            lost = "You win";
        }
        else
            lost="Tú ganas";
    }

    else if(result==3)
    {
        static int i=2;
        lost = ("Level " + patch::to_string(i)).c_str();
        i++;

    }

    SDL_FillRect(surface,&surface->clip_rect,SDL_MapRGB(surface->format, 0x00,0x00,0x00));
    rect.x = 150;
    rect.y = 300;

    lose = TTF_RenderText_Solid(font, lost, color);
    SDL_BlitSurface(lose,NULL,surface,&rect);
    SDL_UpdateWindowSurface(window);

}

void menu::delay(Uint32 time)
{
    if(1000/30 > (SDL_GetTicks() - time))
        {
            SDL_Delay(1000/30 - (SDL_GetTicks() - time));
        }
}


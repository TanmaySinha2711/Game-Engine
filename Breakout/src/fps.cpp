#include "fps.h"
#include "SDL.h"

fps::fps()
{
    //ctor
    tt = SDL_GetTicks();
    ctr = 0;
    avg = 0;
    lastTime = 0;
}

void fps::update_timer()
{
    tt = SDL_GetTicks()/1000;
}

void fps::update_ctr()
{
    currentTime = SDL_GetTicks();
  if (currentTime > lastTime + 1000)
    {
        avg = ctr;
        ctr = 0;
        lastTime = currentTime;
    }
    else ctr++;

}

float fps::avgfps()
{
    return avg;
}

void fps::delay_time(int ms)
{
    SDL_Delay(ms);
}

fps::~fps()
{
    //dtor
}

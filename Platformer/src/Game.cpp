#include "Game.h"
#include "platformer.h"

int main(int argc, char **argv)
{
    Platformer* platformer = new Platformer();
    if(platformer->Init())
    {
        platformer->menuDisplay();
    }
    return 0;
}

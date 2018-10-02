#include "SDL.h"
#include "breakout.h"

typedef enum Color {
    RED,
    LIGHTBLUE,
    BLUE,
} Color;

typedef enum BounceDirection {
    None,
    Top,
    Bottom,
    Left,
    Right
} BounceDirection;

class Brick {
public:

    Brick(SDL_Renderer *r, int x, int y, Color c, int stren);
    ~Brick();

    BounceDirection collide(int x1, int y1, int x2, int y2);
    void destroy();
    void render();

    Color color;
    int strength;
private:
    SDL_Renderer *renderer;

    SDL_Rect paddle;

    bool visible;
};

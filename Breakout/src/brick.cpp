#include "brick.h"

Brick::Brick(SDL_Renderer *r, int x, int y, Color c, int stren) {
    renderer = r;
    paddle.x = x;
    paddle.y = y+50;
    paddle.w = BRICK_WIDTH;
    paddle.h = BRICK_HEIGHT;
    color = c;
    visible = true;
    strength = stren;
}

Brick::~Brick() {}

BounceDirection Brick::collide(int x1, int y1, int x2, int y2) {
    if(strength < 0)
        return None;

    if (!(
        visible &&
        paddle.x <= x2 &&
        paddle.x + BRICK_WIDTH >= x1 &&
        paddle.y <= y2 &&
        paddle.y + BRICK_HEIGHT >= y1
    )) {
        return None;
    }

    if (y1 <= paddle.y - BRICK_HEIGHT/2) {
        return Bottom;
    } else if (y1 >= paddle.y + (BRICK_HEIGHT/2)) {
        return Top;
    } else if (x1 < paddle.x) {
        return Left;
    } else if (x1 > paddle.x) {
        return Right;
    } else {
        return None;
    }
}

void Brick::destroy() {
    visible = false;
}

void Brick::render() {
    if (!visible || strength < 0) {
        return;
    }

    switch (color) {
    case RED:
        SDL_SetRenderDrawColor(renderer, 255, 51, 51, 255);
        break;
    case LIGHTBLUE:
        SDL_SetRenderDrawColor(renderer, 51, 255, 255, 255);
        break;
    default:
        SDL_SetRenderDrawColor(renderer, 51, 153, 255, 255);
        break;
    }
    SDL_RenderFillRect(renderer, &paddle);
}

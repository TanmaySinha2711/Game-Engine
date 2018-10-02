#include <vector>
#include "SDL.h"
#include<bits/stdc++.h>
#include "SDL_ttf.h"
#include "weather.h"

struct WindowResolution
{
    int height;
    int width;
};

static SDL_DisplayMode display_mode;
static WindowResolution display;
WindowResolution getResolution();

class Scene {
public:
    Scene(SDL_Renderer *r,WindowResolution res, Weather* wt);
    ~Scene();

    void render();
    void moveScene();
    void moveRight(float w_speed);
    void moveLeft(float w_speed);

    void playerJump(float myGravity,
    float maxFallSpeed,
    float *myJumpForce,
    float *curJumpForce,
    float deltaTime,
    bool *jump);

    void playerDown();
    bool hitObject();
    int getRestingPosition();
    bool enemyKillsPlayer();
    void createBullet();
    int fire;
    void gravityAction(float myGravity, float *curGravForce);
    bool bottomHit(float *curVelocity);
    bool playerAlive;
    int moveIndex;
    int score;
private:
    int** arr;
    SDL_Rect player;
    SDL_Rect enemy;
    SDL_Rect bullet;
    SDL_Renderer *renderer;
    SDL_Texture *background_texture;
    SDL_Surface *image;
    SDL_Texture *map_textures[7];
    SDL_Texture *run[12];
    SDL_Texture *bullet_texture;
    SDL_Texture *enemy_left;
    SDL_Texture *enemy_right;
    WindowResolution resolution;
    int nextFrame;
    int flag;
    bool enemyAlive;
    int end_point;
    int ex, ey;
    bool enemyDisplayed;
    TTF_Font* font;

    SDL_Rect wind;
    bool wind_moving;
    SDL_Texture *weatherTextures[3];
    Weather* weather;

};

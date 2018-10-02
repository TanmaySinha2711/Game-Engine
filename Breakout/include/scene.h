#include <vector>
#include "SDL.h"
#include "breakout.h"
#include "brick.h"
#include "SDL_ttf.h"

class Scene {
public:
    Scene(SDL_Renderer *r);
    ~Scene();

    int score;
    int life;
    int level;
    void render(TTF_Font *font,int lan);
    void move_paddle(int x);
    void move_paddle_keyboard(int delta);
    void launch_ball();
    void paddle_reset();
    void updateScore(SDL_Renderer* renderer, TTF_Font* font, int lan);
    void CreateLevel();
    SDL_Surface* ballSurface;
    SDL_Texture* ballTexture;
    bool collide;
    std::vector<Brick> bricks;
private:
    void move_ball();

    SDL_Renderer *renderer;

    bool ball_moving;
    int ball_velx;
    int ball_vely;
    SDL_Rect ball;
    SDL_Rect paddle;

};

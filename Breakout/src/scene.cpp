#include "scene.h"
#include<SDL_mixer.h>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#include "SDL_ttf.h"
#include "SDL_image.h"

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

Scene::Scene(SDL_Renderer *r) {
    cerr<<"\nInitializing Scene\n";
    renderer = r;
    score = 0;
    life = 3;
    level = 1;
    paddle.x = (SCREEN_WIDTH - PAD_WIDTH) / 2;
    paddle.y = SCREEN_HEIGHT - PAD_HEIGHT;
    paddle.w = PAD_WIDTH;
    paddle.h = PAD_HEIGHT;
    ballSurface = IMG_Load("./assets/ball.png");
    if(!ballSurface) SDL_Quit();
    ballTexture = SDL_CreateTextureFromSurface(r,ballSurface);
    SDL_FreeSurface(ballSurface);
    ball.x=(SCREEN_WIDTH - BALL_RADIUS) / 2;
    ball.y = paddle.y - BALL_RADIUS;
    ball.w = BALL_RADIUS * 2;
    ball.h = BALL_RADIUS * 2;

    ball_moving = true;
    ball_velx = 0;
    ball_vely = 0;
    paddle_reset();
    launch_ball();
    collide = false;
    CreateLevel();
}

void Scene::CreateLevel(){
    int random;
    for (int i = 0; i < BRICK_ROWS+level; i++) {
        for (int j = 0; j < BRICK_COLS; j++) {
                srand(time(NULL));
                random = rand()%3;
                srand(random);
                bricks.push_back(Brick(renderer, j * BRICK_WIDTH + j, i * BRICK_HEIGHT + i, (Color)random, random));
        }
    }

}

Scene::~Scene() {}

void Scene::updateScore(SDL_Renderer* renderer, TTF_Font* font, int lan)
{
    SDL_Surface* scoreSurface[3];
    SDL_Color color = {255,255,255};
    const char* current[3];
    if(lan==0){
        current[0] = ("Level : " + patch::to_string(level)).c_str();
        current[1] = ("Score : " + patch::to_string(score)).c_str();
        current[2] = ("Lives : " + patch::to_string(life)).c_str();
    }
    else{
        current[0] = ("Nivel : " + patch::to_string(level)).c_str();
        current[1] = ("Puntuación : " + patch::to_string(score)).c_str();
        current[2] = ("Vive : " + patch::to_string(life)).c_str();
    }
    SDL_Texture* scoreTexture[3];
    SDL_Rect position[3];
    position[0].x = 10;
    position[1].x = 150;
    position[2].x = 300;
    for(int i = 0; i < 3; ++i)
    {
        position[i].y = 10;
        position[i].h = 20;
        position[i].w = 100;
        scoreSurface[i] = TTF_RenderText_Solid(font,current[i],color);
        scoreTexture[i] = SDL_CreateTextureFromSurface(renderer,scoreSurface[i]);
        SDL_RenderCopy(renderer,scoreTexture[i],NULL,&position[i]);
    }
}

void Scene::render(TTF_Font *font,int lan) {
    // Draw background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw paddle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle);

    // Draw bricks
    for (int i = 0; i < (BRICK_COLS*(BRICK_ROWS+level)); i++) {
        if(bricks[i].strength >= 0)
            bricks[i].render();
    }

    // Draw ball
    move_ball();
    SDL_RenderCopy(renderer,ballTexture,NULL,&ball);
    SDL_RenderPresent(renderer);

    updateScore(renderer,font,lan);
    SDL_RenderPresent(renderer);
}

void Scene::move_paddle(int x) {
    paddle.x = x;

    if (paddle.x < 0) {
        paddle.x = 0;
    } else if (paddle.x > SCREEN_WIDTH - PAD_WIDTH) {
        paddle.x = SCREEN_WIDTH - PAD_WIDTH;
    }
}

void Scene::move_paddle_keyboard(int delta) {
    move_paddle(paddle.x + delta);
}

void Scene::launch_ball() {
    ball.x = paddle.x - BALL_RADIUS + paddle.w/2;
    ball.y = paddle.y - BALL_RADIUS - paddle.h/2 - 5;
    ball_moving = true;

    ball_velx = 3;
    ball_vely = -5;

    if (rand() > RAND_MAX/2) {
        ball_velx = -ball_velx;
    }
}

void Scene::paddle_reset(){
    paddle.x = (SCREEN_WIDTH - PAD_WIDTH) / 2;
    paddle.y = SCREEN_HEIGHT - PAD_HEIGHT;
    paddle.w = PAD_WIDTH;
    paddle.h = PAD_HEIGHT;
}

void Scene::move_ball() {

    if (!ball_moving) {
        return;
    }

    ball.x += ball_velx;
    ball.y += ball_vely;

    // Collisions

    // Left
    if (ball.x <= 0) {
        ball_velx = -ball_velx;
    }
    // Right
    if (ball.x >= SCREEN_WIDTH - 2*BALL_RADIUS) {
        ball_velx = -ball_velx;
    }
    // Top
    if (ball.y <= 0) {
        ball_vely = -ball_vely;
    }
    // Bottom
    if (ball.y > paddle.y) {
        paddle_reset();
        launch_ball();
        life--;
        cerr<<endl<<3-life<<" lives lost\n";
    }
    // paddle
    if (ball.x >= paddle.x && ball.x <= paddle.x + PAD_WIDTH && ball.y + 2*BALL_RADIUS >= paddle.y) {
        ball_vely = -ball_vely;
        collide = true;
        }
        else
            collide = false;
    // Bricks
    for (int i = (BRICK_COLS*(BRICK_ROWS+level)); i >= 0; --i) {
        int x1 = ball.x;
        int y1 = ball.y;
        int x2 = ball.x + 2*BALL_RADIUS;
        int y2 = ball.y + 2*BALL_RADIUS;

        BounceDirection dir = bricks[i].collide(x1, y1, x2, y2);
        switch (dir) {
        case None:
            continue;
        case Top:
        case Bottom:
            ball_vely = -ball_vely;
            break;
        case Left:
        case Right:
            ball_velx = -ball_velx;
            break;
        }


        if(bricks[i].strength <= 0)
        {
            bricks[i].strength--;
            bricks[i].destroy();
            score = score + 1;

        }

        else {
                bricks[i].strength--;
            bricks[i].color = (Color)bricks[i].strength;
        }

    }
}

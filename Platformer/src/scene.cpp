#include "scene.h"
#include<SDL_mixer.h>
#include<stdio.h>
#include "SDL_image.h"
#include<fstream>
#include "SDL_ttf.h"

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

Scene::Scene(SDL_Renderer *r,WindowResolution res, Weather* wt) {
    resolution = res;

    arr = new int*[24];
    for(int i = 0; i < 24; ++i)
        arr[i] = new int[48];

    ifstream game_map;

    game_map.open("Maps/Map1.dat");
    if (!game_map) {
        cout << "Unable to open file";
        SDL_Quit(); // terminate with error
    }

    weather = wt;

    //import array from tile editor
    int i,j;

    for(i=0;i<24;i++){
        for(j=0;j<24;j++){
            game_map>>arr[i][j];
        }
    }

    game_map.close();

    game_map.open("Maps/Map2.dat");
    if (!game_map) {
        cout << "Unable to open file";
        SDL_Quit(); // terminate with error
    }
    for(i=0;i<24;i++){
        for(j=24;j<48;j++){
            game_map>>arr[i][j];
        }
    }
    game_map.close();

    cerr<<"\nInitializing Scene\n";
    renderer = r;

    font = TTF_OpenFont("./assets/Font2.ttf", 35);

    playerAlive = true;
    enemyAlive = true;
    flag = 1;
    nextFrame = 1;
    fire = 0;
    moveIndex = 0;
    enemyDisplayed = true;
    score = 0;

    //player
    player.w = 32;
    player.h = 32;
    player.x = 0;
    player.y = resolution.height - 32 - (player.w);

    //load background image
    image = IMG_Load("assets/image.bmp");
    if(!image)
        SDL_Quit();
    background_texture = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    //load tile textures
    image= IMG_Load("assets/Grass_Tile_1.png");
    if(!image)
        SDL_Quit();
    map_textures[0] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Grass_Tile_2.png");
    if(!image)
        SDL_Quit();
    map_textures[1] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Grass_Tile_3.png");
    if(!image)
        SDL_Quit();
    map_textures[2] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Box_4.png");
    if(!image)
        SDL_Quit();
    map_textures[3] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Box_5.png");
    if(!image)
        SDL_Quit();
    map_textures[4] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Grass_Tile_6.png");
    if(!image)
        SDL_Quit();
    map_textures[5] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Grass_Tile_7.png");
    if(!image)
        SDL_Quit();
    map_textures[6] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    //load bullet texture
    image = IMG_Load("assets/bullet.png");
    if(!image)
        SDL_Quit();
    bullet_texture = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    //load enemy texture while moving left
    image = IMG_Load("assets/Enemy_9.png");
    if(!image)
        SDL_Quit();
    enemy_left = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    //load enemy texture while moving right
    image = IMG_Load("assets/Enemy_8.png");
    if(!image)
        SDL_Quit();
    enemy_right = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    //load player textures
    //idle
    image = IMG_Load("assets/Idle.png");
    if(!image)
        SDL_Quit();
    run[0] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    //run
    image = IMG_Load("assets/Run_1.png");
    if(!image)
        SDL_Quit();
    run[1] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Run_2.png");
    if(!image)
        SDL_Quit();
    run[2] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Run_3.png");
    if(!image)
        SDL_Quit();
    run[3] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Run_4.png");
    if(!image)
        SDL_Quit();
    run[4] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Run_5.png");
    if(!image)
        SDL_Quit();
    run[5] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Run_6.png");
    if(!image)
        SDL_Quit();
    run[6] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Run_7.png");
    if(!image)
        SDL_Quit();
    run[7] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Run_8.png");
    if(!image)
        SDL_Quit();
    run[8] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Run_9.png");
    if(!image)
        SDL_Quit();
    run[9] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    image = IMG_Load("assets/Run_10.png");
    if(!image)
        SDL_Quit();
    run[10] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    //player jump
    image = IMG_Load("assets/jump.png");
    if(!image)
        SDL_Quit();
    run[11] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    //load wind texture
    image = IMG_Load("assets/wind.png");
    if(!image)
        SDL_Quit();
    weatherTextures[0] = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);

    wind_moving = false;
    srand(time(NULL));
    wind.h = 50;
    wind.w = 50;
    wind.x = -8;
    wind.y = rand()%(500-100 + 1) + 100;

    //set enemy coordinates
    enemy.w = 32;
    enemy.h = 32;
}

Scene::~Scene() {}

void Scene::render() {
    // Draw background
    SDL_RenderCopy(renderer,background_texture,NULL,NULL);

    //draw map
    int i,j;
    SDL_Rect rect;
    rect.w = 32;
    rect.h = 32;
    rect.y = 0;
    for(i=0;i<24;i++){
        for(j=0;j<24;j++){
            if(arr[i][j]!=0){
                if (arr[i][j]==8){
                    if(enemyDisplayed && j == 23){
                        ex = 23 * 32;
                        ey = (i - 1) * 32;
                        enemy.x = ex;
                        enemy.y = ey;
                        enemyDisplayed = false;
                    }
                }
                else{
                    rect.x = j*32;
                    SDL_RenderCopy(renderer,map_textures[arr[i][j]-1],NULL,&rect);
                }

            }
        }
        rect.y = i*32;
    }

    weather->updateWeather(renderer);

    for(i=0;i<24;i++)
        for(j=0;j<47;j++)
            if(arr[i][j]==8 && j==24){
                enemyDisplayed = true;
                enemyAlive = true;
            }

    if(enemyKillsPlayer()){
        playerAlive = false;
    }

    //draw player
    if(playerAlive){
            player.x += weather->windDisplacementX(40, 50);
        SDL_RenderCopy(renderer,run[moveIndex],NULL,&player);
    }
    //draw enemy
    if(enemyAlive){
        if(flag == 1){
                SDL_RenderCopy(renderer,enemy_right,NULL,&enemy);
                enemy.x+=1;
                if(enemy.x >= ex + 64)
                    flag = 0;
        }
        else if(flag == 0){
                SDL_RenderCopy(renderer,enemy_left,NULL,&enemy);
                enemy.x-=1;
                if(enemy.x <= ex - 64)
                    flag = 1;
        }
    }


    //draw bullet
    if(fire == 1){

        SDL_RenderCopy(renderer,bullet_texture,NULL,&bullet);
        bullet.x+=4;
        if(bullet.x >= end_point)
            fire = 0;
        if(bullet.x >= enemy.x && bullet.x <= (enemy.x + enemy.w) && enemyAlive){
            fire = 0;
            enemyAlive = false;
            enemyDisplayed = true;
            score += 10;
        }
    }

    SDL_RenderPresent(renderer);

    if(nextFrame >=60)
                nextFrame = 1;
            else
                nextFrame*=2;



}

void Scene::createBullet(){
    bullet.x = player.x + player.w;
    end_point = player.x + 128;
    bullet.y = player.y + player.w/2.5f;
    bullet.w = player.w/2;
    bullet.h = player.h/2;
}

bool Scene::enemyKillsPlayer(){
    if(((player.x + player.w >= enemy.x) && (player.x <= enemy.x + enemy.w)) && abs(player.y - enemy.y) < 32 && enemyAlive)
        return true;
    return false;
}

void Scene::moveScene() {
    int i,j;
    int temp;
    for(i=0;i<47;i++)
        for(j=0;j<24;j++){
            temp = arr[j][i];
            arr[j][i] = arr[j][i+1];
            arr[j][i+1] = temp;
        }
    enemy.x -= 32;
    ex -= 32;
}

void Scene::moveRight(float w_speed) {

    int w = 32;

    for(int i=0; i<24; i++)
    for(int j=0; j<24; j++){
        if((abs(player.x - j*w +w) < 8 ) && arr[i][j]!=0 && arr[i][j]!=8){
                if(abs(player.y - i*w + w) < 32 && arr[i][j]!=0){
                    return;
                }
        }
    }

    if(player.x >= 18*32)
       {
        if(nextFrame%32 < 6)
        moveScene();
        }
    else
        player.x+=8;

    SDL_RenderCopy(renderer,run[moveIndex++],NULL,&player);
    if(moveIndex >= 11)
        moveIndex = 0;
}

void Scene::moveLeft(float w_speed) {

    int w = 32;
    for(int i=0; i<24; i++)
    for(int j=0; j<24; j++){
        if((abs(player.x - j*w -w) < 8 ) && arr[i][j]!=0 && arr[i][j]!=8){
                if(abs(player.y - i*w + w) < 32 && arr[i][j]!=0){
                    return;
                }
        }
    }

    if(player.x > player.w/2 + 10)
        player.x-=8;

    SDL_RenderCopy(renderer,run[moveIndex--],NULL,&player);
    if(moveIndex < 0)
        moveIndex = 10;

}

bool Scene::hitObject(){
    int i,j;
    int w = 32;

    for(i=0; i<24; i++)
    for(j=0; j<24; j++){
        if(arr[i][j]!=8){
            if((abs(player.x - j*w) < 32 ) && arr[i][j]!=0 ){
                    if(abs(player.y - i*w + w) < 32 && arr[i][j]!=0){
                        return true;
                    }
                    if(abs(player.y - i*w + w) < 32 && arr[i][j]!=0){
                        return true;
                    }
            }
        }
    }
    return false;
}

int Scene::getRestingPosition(){
    int i,j;
    int w = 32;

    for(i=0; i<24; i++)
    for(j=0; j<24; j++){
        if((abs(player.x - j*w) < 32 ) && arr[i][j]!=0 && arr[i][j]!=8){
                if(abs(player.y - i*w + w) < 32 && arr[i][j]!=0){
                    return (i*w - 2*w);
                }
        }
    }
    return player.y ;//- player.y%32;
}

void Scene::playerJump(float myGravity,
    float maxFallSpeed,
    float *myJumpForce,
    float *curJumpForce,
    float deltaTime,
    bool *jump){
                player.y += *curJumpForce;

                *curJumpForce += myGravity;
                moveIndex = 11;

                if(hitObject()){
                    *jump = false;
                    *curJumpForce = 0;
                    player.y = getRestingPosition();
                    moveIndex = 0;
                }

                SDL_RenderCopy(renderer,run[moveIndex],NULL,&player);
    }

void Scene::playerDown(){
    int i,j;

    player.y+=8;
}

void Scene::gravityAction(float myGravity, float *curGravForce)
    {
        if(!hitObject()){
            player.y += *curGravForce;
            *curGravForce += myGravity;
        }
        if(hitObject()){
                    *curGravForce = 8.0f;
                    player.y = getRestingPosition();
                }
    }

bool Scene::bottomHit(float *curVelocity){
    int w = 32;
    for(int i=0; i<24; i++)
    for(int j=0; j<24; j++){
        if((abs(player.x - j*w) < 32 ) && arr[i][j]!=0 && arr[i][j]!=8){
                if(abs(player.y - i*w + w) < 0 && *curVelocity < 0)
                {
                    return true;
                }
            }

    }
    return false;
}



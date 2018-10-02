#include "platformer.h"
using namespace std;

WindowResolution getResolution()
{
    if(SDL_GetCurrentDisplayMode(0, &display_mode) == 0)
    {
        display.height = display_mode.h;
        display.width = display_mode.w;
    }

    else
    {
        std::cout << "Resolution Error : " << SDL_GetError();
        SDL_Quit();
    }

    return display;
}

bool Platformer::Init(){

    //initialize audio/video
    TTF_Init();
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        std::cout << "SDL_Init Error: " << std::endl;
        return false;
    }

    display = getResolution();
    display.width = SCREEN_DIM;
    display.height = SCREEN_DIM;

    cerr<<"\nIntializing Window\n";
    window = SDL_CreateWindow(
        "Platformer",
        SCREEN_X, SCREEN_Y,
        display.width, display.height,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
     //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
           printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
           SDL_Quit();
    }

    cerr<<"\nIntializing Renderer\n";
    renderer = SDL_CreateRenderer(
        window, // Window
        -1,     // Video driver. -1 means "any compatible"
        SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC // Options
    );
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    font = TTF_OpenFont("./assets/Font2.ttf", 35);
    surface = SDL_GetWindowSurface(window);
    weather = new Weather(0, 0, display.height - 32, display.width);
    scene = new Scene(renderer, display, weather);
    return true;
}

void Platformer::menuDisplay()
{
    int navigate = mainMenu->displayMenu(window, surface, font);
    if(navigate == 0)
    {
        load();
        runGame();
    }
    else if (navigate == 1){
        int x = mainMenu->displaySettingsMenu(window, surface, font);
        for(int i=0; i<5; i++){
            if(i==x){
                weatherOption[i] = true;
            }
            else
                weatherOption[i] = false;
        }

        if(weatherOption[0] == true){
            weather->setWindTextureDimensions(32, 64);
            weather->addWind(renderer, "assets/wind.png", 4.0f, -1, 40);
            load();
            runGame();
        }
        else if(weatherOption[1] == true){
            weather->setParticleDimensions(12, 12);
            weather->addRain(renderer, "assets/Raindrop.png", 2, 40);
            load();
            runGame();
        }
        else if(weatherOption[2] == true){
            weather->setParticleDimensions(12, 12);
            weather->addSnow(renderer, "assets/snowflake.png", 2, 40);
            load();
            runGame();
        }
        else if(weatherOption[3] == true){
            weather->setParticleDimensions(12, 12);
            weather->addRain(renderer, "assets/Raindrop.png", 4, 30);
            weather->setWindTextureDimensions(32, 64);
            weather->addWind(renderer, "assets/wind.png", 1.0f, 1, 20);
            load();
            runGame();
        }
        else if(weatherOption[4] == true){
            weather->setParticleDimensions(3, 3);
            weather->addSandstorm(renderer, "assets/sand.png",3.0f, 4.0f, 1, 10000);
            load();
            runGame();
        }
        menuDisplay();
    }
    if(navigate == 2)
        {
            annihilate();
            exit(0);
        }
}

void Platformer::load(){
    //Load music
    music = Mix_LoadMUS( "assets/music.mp3" );
    if( music == NULL )
    {
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
        SDL_Quit();
    }

    fire_sound = Mix_LoadWAV( "./assets/fire.mp3" );
    if( fire_sound == NULL ){
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    jump_sound = Mix_LoadWAV( "./assets/jump.mp3" );
    if( jump_sound == NULL ){
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    death = Mix_LoadWAV( "./assets/dead.wav" );
    if( death == NULL ){
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    }

}

void Platformer::runGame(){

    SDL_Event e;
    bool quit = false;
    bool paused=false;
    Mix_PlayMusic(music,-1);
    float myGravity = 0.4f;
    float maxFallSpeed = -16.0f;
    float myJumpForce = -8.0f;
    float curJumpForce = 0.0f;
    float deltaTime = 1.0f/20.0f;
    bool jump = false;
    float curGravForce = 8.0f;


    if(Mix_PausedMusic()==1)
        Mix_ResumeMusic();

    const char* current;

    while (!quit) {
            current = ("Score :  " + patch::to_string(scene->score)).c_str();
            SDL_SetWindowTitle(window,current);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            }
            if(!scene->playerAlive){
                Mix_PlayChannel(-1,death,1);
                SDL_Delay(3000);
                SDL_Quit();
            }

            if (e.type == SDL_KEYDOWN) {

                    if(e.key.keysym.sym == SDLK_LEFT)
                    {
                        scene->moveLeft(w_speed);
                    }

                    else if(e.key.keysym.sym == SDLK_RIGHT)
                    {
                        scene->moveRight(w_speed);
                    }

                    if(e.key.keysym.sym == SDLK_UP)
                    {
                        if(!jump){
                            Mix_PlayChannel(-1,jump_sound,0);
                            jump = true;
                            curJumpForce = myJumpForce;
                        }
                    }

                    else if(e.key.keysym.sym == SDLK_DOWN)
                    {
                        scene->playerDown();
                    }
                    if(e.key.keysym.sym == SDLK_SPACE){
                        if(scene->playerAlive){
                            if(scene->fire ==0){
                                Mix_PlayChannel(-1,fire_sound,0);
                                scene->createBullet();
                                scene->fire = 1;
                            }
                        }
                    }
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        annihilate();
                        if(Init())
                            menuDisplay();
                        break;
                    case SDLK_p:
                        if(paused==false){
                            paused=true;
                            cerr<<"game paused"<<endl;
                        }
                        else{
                            paused=false;
                        }
                        break;
                }

            }

        }
        if(scene->bottomHit(&curJumpForce))
        {
            jump = false;
        }

        else if(jump){
            scene->playerJump(myGravity, maxFallSpeed,
                        &myJumpForce,
                        &curJumpForce,
                        deltaTime,
                        &jump);
        }
        else
            scene->gravityAction(myGravity,&curGravForce);

        if(!quit){
            scene->render();
        }
    }

    SDL_DestroyWindow(window);
    Mix_FreeMusic( music );
    music = NULL;
    Mix_Quit();
    SDL_Quit();
}

void Platformer::annihilate(){
    music = NULL;
    weather = NULL;
    scene = NULL;
    window = NULL;
    SDL_DestroyWindow(window);
    renderer = NULL;
    SDL_Quit();
}


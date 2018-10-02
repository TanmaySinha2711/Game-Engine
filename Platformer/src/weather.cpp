#include "weather.h"

Weather::Weather(int window_origin_x, int window_origin_y, int windowHeight, int windowWidth)
{
    origin_x = window_origin_x;
    origin_y = window_origin_y;
    window_width = windowWidth;
    window_height = windowHeight;
}

//empty constructor
Weather::Weather()
{

}

Weather::~Weather()
{
    //dtor
}

//update weather at each frame
void Weather::updateWeather(SDL_Renderer *r){

    if(sandEnabled){
        for(int i=0;i<particleDensity;i++){
            weather_particles[i].y += -particleSpeed + rand()%(int)(particleSpeed + particleSpeed + 1);
            weather_particles[i].x += -windSpeed + rand()%(int)(2*windSpeed + windSpeed + 1);
            if(windDirection == 1 && (weather_particles[i].x >= origin_x + window_width)){
                weather_particles[i].x = origin_x;
                weather_particles[i].y = origin_y + rand()%(window_height - origin_y + 1);
            }
            else if(windDirection == -1 && weather_particles[i].x <= origin_x){
                weather_particles[i].x = origin_x + window_width;
                weather_particles[i].y = origin_y + rand()%(window_height - origin_y + 1);
            }
            SDL_RenderCopy(r, weather_texture,NULL,&weather_particles[i]);
        }
    }
    else {
        if(windEnabled){
            for(int i = 0; i<windDensity ; i++){
                wind_rect[i].x += windSpeed;
                if(abs(wind_rect[i].x) % window_width/64 < window_width/128)
                    wind_rect[i].y += cos(i);
                else
                    wind_rect[i].y += -1*cos(i);
                if(windDirection == 1 && (wind_rect[i].x >= origin_x + window_width)){
                    wind_rect[i].x = origin_x;
                    wind_rect[i].y = origin_y + rand()%(window_height - origin_y + 1);
                }
                else if(windDirection == -1 && wind_rect[i].x <= origin_x){
                    wind_rect[i].x = origin_x + window_width;
                    wind_rect[i].y = origin_y + rand()%(window_height - origin_y + 1);
                }
                SDL_RenderCopy(r, wind_texture,NULL,&wind_rect[i]);
            }

        }

        if(rainEnabled || snowEnabled){
            for(int i=0;i<particleDensity;i++){

                weather_particles[i].y += particleSpeed;
                if(weather_particles[i].y >= window_height)
                    weather_particles[i].y = origin_y;
                if(windEnabled){
                    weather_particles[i].x += (rand()%((int)windSpeed + 1));
                    if(weather_particles[i].x > origin_x + window_width){
                        weather_particles[i].x = origin_x;
                        weather_particles[i].y = origin_y + rand()%(window_height - origin_y + 1);
                    }

                    if(weather_particles[i].x < origin_x){
                        weather_particles[i].x = origin_x + window_width;
                        weather_particles[i].y = origin_y + rand()%(window_height - origin_y + 1);
                    }
                }
                SDL_RenderCopy(r, weather_texture,NULL,&weather_particles[i]);
            }
        }
    }
}

//initialize rain particle array in order to add rain
void Weather::addRain(SDL_Renderer *r, std::string filepath, float fallSpeed, int density){
    rainEnabled = true;
    particleSpeed = fallSpeed;
    particleDensity = density;
    weather_particles = new SDL_Rect[particleDensity];

    for(int i=0;i<particleDensity;i++){
        weather_particles[i].h = particleHeight;
        weather_particles[i].w = particleWidth;
        weather_particles[i].x = origin_x + rand()%(window_width - origin_x + 1);
        weather_particles[i].y = origin_y + rand()%(window_height - origin_y + 1);
    }

    SDL_Surface *image = IMG_Load(filepath.c_str());
    if(!image)
        SDL_Quit();
    weather_texture = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);
}

//initialize snow particle array in order to add snow
void Weather::addSnow(SDL_Renderer *r, std::string filepath, float fallSpeed, int density){
    snowEnabled = true;
    particleSpeed = fallSpeed;
    particleDensity = density;
    weather_particles = new SDL_Rect[particleDensity];

    for(int i=0;i<particleDensity;i++){
        weather_particles[i].h = particleHeight;
        weather_particles[i].w = particleWidth;
        weather_particles[i].x = origin_x + rand()%(window_width - origin_x + 1);
        weather_particles[i].y = origin_y + rand()%(window_height - origin_y + 1);
    }

    SDL_Surface *image = IMG_Load(filepath.c_str());
    if(!image)
        SDL_Quit();
    weather_texture = SDL_CreateTextureFromSurface(r,image);
    SDL_FreeSurface(image);
}

//enable wind effect by setting windspeed and populating the wind SDL_rects array
void Weather::addWind(SDL_Renderer *r, std::string filepath, float speed, int direction, int density){
    windEnabled = true;
    windDirection = direction;
    windSpeed = speed * windDirection;
    windDensity = density;
    wind_rect = new SDL_Rect[windDensity];

    if(sizeof(filepath)>1){
        for(int i = 0; i<windDensity; i++ ){
            wind_rect[i].h = windParticleHeight;
            wind_rect[i].w = windParticleWidth;
            wind_rect[i].x = origin_x + rand()%(window_width - origin_x + 1);
            wind_rect[i].y = origin_y + rand()%(window_height - origin_y + 1);
        }
        SDL_Surface *image = IMG_Load(filepath.c_str());
        if(!image)
            SDL_Quit();
        wind_texture = SDL_CreateTextureFromSurface(r,image);
        SDL_FreeSurface(image);
    }
}

//add sandstorm particles to scene
void Weather::addSandstorm(SDL_Renderer *r, std::string filepath, float VerticalSpeed, float HorizontalSpeed, int direction, int density){
    sandEnabled = true;
    windDirection = direction;
    windSpeed = HorizontalSpeed * windDirection;
    particleSpeed = VerticalSpeed;
    particleDensity = density;
    weather_particles = new SDL_Rect[particleDensity];

    if(sizeof(filepath)>1){
        for(int i = 0; i<particleDensity; i++ ){
            weather_particles[i].h = particleHeight;
            weather_particles[i].w = particleWidth;
            weather_particles[i].x = origin_x + rand()%(window_width - origin_x + 1);
            weather_particles[i].y = origin_y + rand()%(window_height - origin_y + 1);
        }
        SDL_Surface *image = IMG_Load(filepath.c_str());
        if(!image)
            SDL_Quit();
        weather_texture = SDL_CreateTextureFromSurface(r,image);
        SDL_FreeSurface(image);
    }
}

//set wind speed given by the user
void Weather::setWindSpeed(float speed){
    windSpeed = speed;
}

//set particle y-velocity given by the user
void Weather::setRainSpeed(float speed){
    particleSpeed = speed;
}

//set particle y-velocity given by the user
void Weather::setSnowSpeed(float speed){
    particleSpeed = speed;
}

//set the size of snow or rain particles
void Weather::setParticleDimensions(int height, int width){
    particleHeight = height;
    particleWidth = width;
}

//set the size of wind texture
void Weather::setWindTextureDimensions(int height, int width){
    windParticleHeight = height;
    windParticleWidth = width;
}

//calculate change in an object's position in each frame due to the effect of rain based on the object's mass and surface area
float Weather::windDisplacementX(float mass, float surface_area){
    if(rand()%100 >= 90){
        float displacement = ((int)(windSpeed*surface_area/mass)%2);
        return displacement;
    }
    return 0;
}

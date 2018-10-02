#ifndef WEATHER_H
#define WEATHER_H
#include<SDL.h>
#include<string>
#include<stdlib.h>
#include "SDL_image.h"
#include<iostream>
#include<math.h>

class Weather
{
    public:
        Weather();
        Weather(int window_origin_x, int window_origin_y, int windowHeight, int windowWidth);
        virtual ~Weather();

        void setWindSpeed(float speed);
        void setWindTextureDimensions(int height, int width);
        void setRainSpeed(float speed);
        void setSnowSpeed(float speed);
        void setRainDirection(float direction);
        void setParticleDimensions(int height, int width);
        void rainFrequency(float frequency);
        void addWind(SDL_Renderer *r, std::string filepath, float speed, int direction, int density);
        void addSandstorm(SDL_Renderer *r, std::string filepath, float VerticalSpeed, float HorizontalSpeed, int direction, int density);
        void addRain(SDL_Renderer *r, std::string filepath, float fallSpeed, int density);
        void addSnow(SDL_Renderer *r, std::string filepath, float fallSpeed, int density);

        void updateWeather(SDL_Renderer *r);

        float windDisplacementX(float mass, float surface_area);
        float windDisplacementY(SDL_Rect object, float mass);


    protected:

    private:
        int windParticleHeight = 0, windParticleWidth = 0;
        int window_width, window_height;
        int origin_x, origin_y;
        int particleDensity;
        int windDensity;
        int rowCount, colCount;
        int TileWidth, TileHeight;
        int windDirection;
        int particleWidth, particleHeight;

        float windSpeed;
        float particleSpeed;
        float rainDirection;

        bool windEnabled;
        bool rainEnabled;
        bool snowEnabled;
        bool sandEnabled;

        SDL_Rect* weather_particles;
        SDL_Rect *wind_rect;
        SDL_Texture *weather_texture;
        SDL_Texture *wind_texture;
};

#endif // WEATHER_H

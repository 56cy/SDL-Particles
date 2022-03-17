#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#include "SDL2/include/SDL2/SDL.h"

class Particle{
    public:
        
        float x, y, xvel, yvel;
        bool update();
        Particle(float x, float y, float xvel, float yvel);
};

Particle::Particle(float x, float y, float xvel, float yvel){
    this->x = x;
    this->y = y;
    this->xvel = xvel;
    this->yvel = yvel;
}

bool Particle::update(){
    x -= xvel;
    y -= yvel;
    yvel -= 0.1;
    return y > 650;
}

int main(int argc, char* argv[]){

    // setup sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Ptc Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // random
    std::random_device r;
    std::default_random_engine rand(r());
    std::uniform_int_distribution<int> rand_dist_x(-200, 200);
    std::uniform_int_distribution<int> rand_dist_y(200, 400);

    // fps capping
    const int FPS = 60;
    const float FRAME_DELAY = 1000 / FPS;
    Uint32 framestart;
    int frametime;

    // particles
    std::vector<Particle> particles;
    particles.reserve(1000000);

    // thing
    std::vector<SDL_Point> draw_points;
    draw_points.reserve(1000000);

    // game state
    bool running = true;

    while (running){

        for (int i = 0; i != 5000; i++){
            // particles.push_back(Particle(300, 300, 1, 1));
            particles.push_back(Particle(300, 300, rand_dist_x(rand) * 0.01f, rand_dist_y(rand) * 0.01f));
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        draw_points.clear();

        // update particles and remove dead ones
        particles.erase(
            std::remove_if(
                particles.begin(),
                particles.end(),
                [&draw_points](Particle& particle){
                    SDL_Point pt = {int(particle.x), int(particle.y)};
                    draw_points.push_back(pt);
                    return particle.update();
                }
            ),
            particles.end()
        );

        SDL_Point* pts = &draw_points[0];
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoints(renderer, pts, draw_points.size());

        SDL_RenderPresent(renderer);
        
        // cap fps
        frametime = SDL_GetTicks() - framestart;
        if (FRAME_DELAY > frametime){
            SDL_Delay((FRAME_DELAY - frametime));
        }
        std::cout << (FRAME_DELAY - frametime) << " with " << particles.size() << "\n";
        framestart = SDL_GetTicks();

    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;

}
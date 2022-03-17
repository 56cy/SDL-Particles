#include <iostream>
#include <vector>
#include <algorithm>

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

    // fps capping
    const int FPS = 60;
    const float FRAME_DELAY = 1000 / FPS;
    Uint32 framestart;
    int frametime;

    // particles
    std::vector<Particle> particles;
    particles.reserve(1000000);

    // game state
    bool running = true;

    while (running){

        for (int i = 0; i != 20000; i++){
            particles.push_back(Particle(300, 300, 1, 1));
        }

        // update particles and remove dead ones
        particles.erase(
            std::remove_if(
                particles.begin(),
                particles.end(),
                [](Particle& particle){
                    return particle.update();
                }
            ),
            particles.end()
        );;
        
        // cap fps
        frametime = SDL_GetTicks() - framestart;
        if (FRAME_DELAY > frametime){
            SDL_Delay((FRAME_DELAY - frametime));
        }
        std::cout << (FRAME_DELAY - frametime) << " with " << particles.size() << "\n";
        framestart = SDL_GetTicks();

    }

    return 0;

}
#include "Timing.hpp"

FpsLimiter::FpsLimiter() { }

void FpsLimiter::init(float maxFPS) {
    _maxFPS = maxFPS;
}


void FpsLimiter::begin() {
    _startTicks = SDL_GetTicks();
}

float FpsLimiter::end() {
    calculateFPS();

    //for fps limiting to _maxFPS
    uint32_t frameTicks = SDL_GetTicks() - _startTicks;
    if (1000.0f / _maxFPS > frameTicks) {
        SDL_Delay(1000.0f / _maxFPS - frameTicks);
    }
    return _fps;
}

void FpsLimiter::calculateFPS() {
    static const int NUM_SAMPLES = 20;
    static float frameTimes[NUM_SAMPLES];
    static int currentFrame = 0;

    static uint32_t prevTicks = SDL_GetTicks();
    uint32_t currentTicks;
    currentTicks = SDL_GetTicks();
    _frameTime = currentTicks - prevTicks;
    prevTicks = currentTicks;

    frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

    int count;
    currentFrame++;
    if (currentFrame < NUM_SAMPLES) {
        count = currentFrame;
    } else {
        count = NUM_SAMPLES;
    }

    float frameTimeAverage = 0;
    for (int i = 0; i < count; i++) {
        frameTimeAverage += frameTimes[i];
    }
    frameTimeAverage /= count;

    if (frameTimeAverage > 0) {
        _fps = 1000.0f / frameTimeAverage;
    } else {
        _fps = 60.0f;
    }
}

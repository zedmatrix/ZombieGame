#pragma once
#include <SDL2/SDL.h>
#include <cstdint>

class FpsLimiter {

public:
    FpsLimiter();

    void init(float maxFPS);
    void begin();
    float end();

private:

    void calculateFPS();
    float _maxFPS;
    uint32_t _startTicks;
    float _time;

    float _fps;
    float _frameTime;
};

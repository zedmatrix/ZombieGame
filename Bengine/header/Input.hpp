#ifndef INPUT_HPP
#define INPUT_HPP
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <cstdint>
#include <unordered_map>
class Input {

public:

    Input() : _mouseCoords(0) { }


    void pressKey(uint32_t keyID);
    void releaseKey(uint32_t keyID);
    bool isKeyPressed(uint32_t keyID);
    void setMouseCoords(float x, float y);
    glm::vec2 getMouseCoords() const { return _mouseCoords; }

private:

    std::unordered_map<uint32_t, bool> _keyMap;
    glm::vec2 _mouseCoords;

};

#endif //INPUT_HPP

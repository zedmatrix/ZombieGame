#include "Input.hpp"

void Input::pressKey(uint32_t keyID) {
    _keyMap[keyID] = true;
}

void Input::releaseKey(uint32_t keyID) {
    _keyMap[keyID] = false;
}

bool Input::isKeyPressed(uint32_t keyID) {
    auto it = _keyMap.find(keyID);
    if (it != _keyMap.end()) {
        return it->second;
    }
    return false;
}

void Input::setMouseCoords(float x, float y) {
    _mouseCoords.x = x;
    _mouseCoords.y = y;
}

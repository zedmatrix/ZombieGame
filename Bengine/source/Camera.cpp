#include "Camera.hpp"

Camera::Camera()
    : _needMatrixUpdate(true),
    _position(0.0f, 0.0f),
    _cameraMatrix(1.0f),
    _orthoMatrix(1.0f),
    _scale(1.0f) {}

void Camera::init(int screenWidth, int screenHeight) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _orthoMatrix = glm::ortho(0.0f, static_cast<float>(_screenWidth),
                              0.0f, static_cast<float>(_screenHeight));

}

void Camera::update() {

    if (_needMatrixUpdate) {

        // Camera Translation
        glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
        _cameraMatrix = glm::translate(_orthoMatrix, translate);

        // Camera Scale
        glm::vec3 scale(_scale, _scale, 0.0f);
        _cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

        _needMatrixUpdate = false;
    }
}

glm::vec2 Camera::getWorldCoords(glm::vec2 screenCoords) {
    //Invert y coordinates
    screenCoords.y = _screenHeight - screenCoords.y;

    // Make the center of screen ZERO
    screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);

    // Scale the coordinates
    screenCoords /= _scale;

    //Translate to camera position
    screenCoords += _position;
    return screenCoords;
}

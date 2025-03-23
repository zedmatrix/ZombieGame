#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

public:
    Camera();

    void update();
    void init(int screenWidth, int screenHeight);
    // Setters
    void setPosition(const glm::vec2& newPosition) { _position = newPosition; _needMatrixUpdate = true; }
    void setScale(float newScale) { _scale = newScale; _needMatrixUpdate = true; }

    // Getters
    glm::vec2 getPosition() const { return _position; }
    glm::vec2 getWorldCoords(glm::vec2 screenCoords);
    const glm::mat4& getCameraMatrix() const { return _cameraMatrix; }
    float getScale() const { return _scale; }

private:
    int _screenWidth, _screenHeight;
    bool _needMatrixUpdate;
    glm::vec2 _position;
    glm::mat4 _cameraMatrix;
    glm::mat4 _orthoMatrix;
    float _scale;

};

#endif //CAMERA_HPP

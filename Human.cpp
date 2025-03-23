#define GLM_ENABLE_EXPERIMENTAL
#include "Human.hpp"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Human::Human() {

}

// Human::~Human() {
//
// }

void Human::init(float speed, glm::vec2 pos) {
    _color.r = 64;
    _color.g = 64;
    _color.b = 64;
    _color.a = 255;
    _health = 300;
    std::mt19937 randomEngine(time(nullptr));
    std::uniform_real_distribution<float> randDir(-2.0f, 2.0f);

    _speed = speed;
    _position = pos;
    _direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
    if (_direction.length() == 0) _direction = glm::vec2(1.0, 0.0f);

    _direction = glm::normalize(_direction);

}

void Human::update(const std::vector<std::string>& _levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies) {

    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randRotate(-10.0f, 10.0f);

    _position += _direction * _speed;
    //_direction = glm::rotate(_direction, randRotate(randomEngine)); ///< jitterbug movement
    if (collideWithLevel(_levelData) ) {
        _direction = glm::rotate(_direction, randRotate(randomEngine));
    }
}

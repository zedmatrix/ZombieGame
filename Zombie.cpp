#define GLM_ENABLE_EXPERIMENTAL
#include "Zombie.hpp"
#include "Human.hpp"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Zombie::Zombie() {

}

void Zombie::update(const std::vector<std::string>& _levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies) {

    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randRotate(-2.0f, 2.0f);
    Human* closestHuman = getNearestHuman(humans);

    if (closestHuman != nullptr) {
        glm::vec2 _direction = glm::normalize(closestHuman->getPosition() - _position);
        _position += _direction * _speed;
    }

    if (collideWithLevel(_levelData) ) {
        _direction = glm::rotate(_direction, randRotate(randomEngine));
    }
}

void Zombie::init(float speed, glm::vec2 pos) {
    _speed = speed;
    _position = pos;
    _color.r = 32;
    _color.g = 255;
    _color.b = 32;
    _color.a = 255;
    _health = 300;

    std::mt19937 randomEngine(time(nullptr));
    std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

    _speed = speed;
    _position = pos;
    _direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
    if (_direction.length() == 0) _direction = glm::vec2(1.0, 0.0f);

    _direction = glm::normalize(_direction);
}

Human* Zombie::getNearestHuman(std::vector<std::unique_ptr<Human>>& humans) {
    Human* closestHuman = nullptr;
    float smallestDistance = 99999;

    for (size_t i = 0; i < humans.size(); i++) {
        glm::vec2 distVec = humans[i]->getPosition() - _position;
        float distance = glm::length(distVec);
        if (distance < smallestDistance) {
            smallestDistance = distance;
            closestHuman = humans[i].get();
        }

    }
    return closestHuman;
}

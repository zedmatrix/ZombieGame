#include "Player.hpp"

Player::Player() : _input(nullptr), _currentGunIndex(-1) {
    //Empty
}

void Player::init(float speed, glm::vec2 pos, Input* input, Camera* camera, std::vector<Bullet>* bullets) {
    _speed = speed;
    _position = pos;
    _input = input;
    _camera = camera;
    _bullets = bullets;
    _color.r = 255;
    _color.g = 64;
    _color.b = 64;
    _color.a = 255;
    _health = 300;
}

void Player::addGun(std::unique_ptr<Gun> gun) {
    _guns.push_back(std::move(gun));
    if (_currentGunIndex == -1) {
        _currentGunIndex = 0;
    }
}

void Player::update(const std::vector<std::string>& _levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies) {

    if (_input->isKeyPressed(SDLK_UP)) _position.y += _speed;
    if (_input->isKeyPressed(SDLK_DOWN)) _position.y -= _speed;
    if (_input->isKeyPressed(SDLK_RIGHT)) _position.x += _speed;
    if (_input->isKeyPressed(SDLK_LEFT)) _position.x -= _speed;

    if (_input->isKeyPressed(SDLK_1) && _guns.size() >= 0) {
        _currentGunIndex = 0;
    } else if (_input->isKeyPressed(SDLK_2) && _guns.size() >= 1) {
        _currentGunIndex = 1;
    }

    if (_currentGunIndex != -1) {

        glm::vec2 mouseCoords = _input->getMouseCoords();
        mouseCoords = _camera->getWorldCoords(mouseCoords);

        glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);

        glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);

        _guns[_currentGunIndex]->update(_input->isKeyPressed(SDL_BUTTON_LEFT),
                                        centerPosition,
                                        direction,
                                        *_bullets);
    }
    collideWithLevel(_levelData);

}


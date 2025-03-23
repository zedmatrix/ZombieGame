#define GLM_ENABLE_EXPERIMENTAL
#include "Weapon.hpp"
#include "ResourceManager.hpp"
#include "Vertex.hpp"
#include "Agent.hpp"
#include "Level.hpp"
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(std::string name, int rate, int bps, int damage, float speed, float spread) :
    _name(name),
    _fireRate(rate),
    _bulletsPerShot(bps),
    _bulletDamage(damage),
    _bulletSpeed(speed),
    _spread(spread),
    _frameCounter(0) {
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets) {
    _frameCounter++;
    if (_frameCounter >= _fireRate && isMouseDown) {
        fire(position, direction, bullets);
        _frameCounter = 0;
    }

}
void Gun::fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets) {
    static std::mt19937 randomEngine(time(nullptr));
    std::uniform_real_distribution<float> randRotate(-_spread, _spread);

    for (int i = 0; i < _bulletsPerShot; i++) {
        bullets.emplace_back(position, direction, _bulletSpeed, _bulletDamage);
        //bullets.emplace_back(position, glm::rotate(direction, randRotate(randomEngine)), _damage, _speed);
    }
}

/// Bullet Class
Bullet::Bullet(const glm::vec2& position, const glm::vec2& direction, float damage, float speed) :
    _position(position),
    _direction(direction),
    _damage(damage),
    _speed(speed) {

}
bool Bullet::update(const std::vector<std::string>& _levelData /*, float deltaTime */ ) {
    _position += _direction * _speed;
    return collideWithWorld(_levelData);
}

bool Bullet::collideWithWorld(const std::vector<std::string>& _levelData) {
    glm::ivec2 gridPosition;
    gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
    gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

    // If we are outside the world, just return
    if (gridPosition.x < 0 || gridPosition.x >= _levelData[0].size() ||
        gridPosition.y < 0 || gridPosition.y >= _levelData.size()) {
        return true;
    }

    return (_levelData[gridPosition.y][gridPosition.x] != '.');
}

void Bullet::draw(SpriteBatch& _spriteBatch) {
    glm::vec4 destRect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
    glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    Color greyColor;
    greyColor.r = 128;
    greyColor.g = 128;
    greyColor.b = 128;
    greyColor.a = 128;
    _spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture("Textures/bullet.png").id, 0.0f, greyColor);
}
bool Bullet::collideWithAgent(Agent* agent) {
    const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

    glm::vec2 centerPosA = _position + glm::vec2(BULLET_RADIUS);
    glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

    glm::vec2 distVec = centerPosA - centerPosB;

    float distance = glm::length(distVec);

    float collisionDepth = MIN_DISTANCE - distance;
    if (collisionDepth > 0) {
        return true;
    }
    return false;

}

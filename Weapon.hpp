#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Sprite.hpp"
#include <random>
#include <memory>
#include <ctime>

class Agent;
class Human;
class Zombie;
// Bullet Class

const int BULLET_RADIUS = 6;
class Bullet {

public:
    Bullet(const glm::vec2& position, const glm::vec2& direction, float damage, float speed);

    //When update returns true delete bullet
    bool update(const std::vector<std::string>& _levelData /*, float deltaTime*/ );

    void draw(SpriteBatch& _spriteBatch);

    bool collideWithAgent(Agent* agent);
    float getDamage() const { return _damage; }


private:

    bool collideWithWorld(const std::vector<std::string>& _levelData);
    glm::vec2 _position;
    glm::vec2 _direction;
    float _damage;
    float _speed;

};





// Gun class
class Gun {

public:
    Gun(std::string name, int rate, int bps, int damage, float speed, float spread);
    void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);


private:
    void fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);
    std::string _name;
    int _fireRate;
    int _bulletsPerShot;
    int _bulletDamage;
    float _bulletSpeed;
    float _spread;

    glm::vec2 _direction;

    int _frameCounter;
};

#endif //WEAPON_HPP

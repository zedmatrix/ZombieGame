#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "Human.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "Weapon.hpp"

class MainGame;
class Player : public Human {

public:
    Player();
    void init(float speed, glm::vec2 pos, Input* input, Camera* camera, std::vector<Bullet>* bullets);
    virtual void update(const std::vector<std::string>& _levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies);
    void addGun(std::unique_ptr<Gun> gun);

private:
    Input* _input;

    std::vector<std::unique_ptr<Gun>> _guns;
    int _currentGunIndex;
    Camera* _camera;
    std::vector<Bullet>* _bullets;

};

#endif //PLAYER_HPP

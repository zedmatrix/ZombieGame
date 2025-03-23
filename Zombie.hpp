#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP
#include "Agent.hpp"

class Zombie : public Agent {

public:
    Zombie();

    void init(float speed, glm::vec2 pos);
    virtual void update(const std::vector<std::string>& _levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies);

private:
    glm::vec2 _direction;
    Human* getNearestHuman(std::vector<std::unique_ptr<Human>>& humans);

};

#endif //ZOMBIE_HPP

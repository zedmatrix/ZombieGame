#ifndef HUMAN_HPP
#define HUMAN_HPP
#include "Agent.hpp"


class Human : public Agent {

public:
    Human();
    ///virtual ~Human();

    void init(float speed, glm::vec2 pos);
    virtual void update(const std::vector<std::string>& _levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies);

private:
    glm::vec2 _direction;

};

#endif //HUMAN_HPP

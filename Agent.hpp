#ifndef AGENT_HPP
#define AGENT_HPP
#include <glm/glm.hpp>
#include "Sprite.hpp"
#include "Vertex.hpp"
#include "Level.hpp"
#include <cmath>
#include <memory>
#include <algorithm>

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2;
const float TILE_RADIUS = TILE_WIDTH / 2;

class Zombie;
class Human;

class Agent {

public:
    Agent();
    ///virtual ~Agent();
    virtual void update(const std::vector<std::string>& _levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies) = 0;
    bool collideWithLevel(const std::vector<std::string>& _levelData);
    bool collideWithAgent(Agent* agent);

    void setTexture(const std::string& texturePath) {
        textureID = ResourceManager::getTexture(texturePath).id;
    }
    void draw(SpriteBatch& _spriteBatch);
    bool applyDamage(float damage);

    glm::vec2 getPosition() const { return _position; }

protected:
    void checkTilePosition(const std::vector<std::string>& _levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y);
    void collideWithTile(glm::vec2 tilePos);
    glm::vec2 _position;
    float _speed;
    float _health;
    Color _color;


private:
    GLuint textureID;

};

#endif //AGENT_HPP

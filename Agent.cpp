#include "Agent.hpp"
#include "ResourceManager.hpp"
#include "Error.hpp"
Agent::Agent() {

}

// Agent::~Agent() {
//
// }

void Agent::draw(SpriteBatch& _spriteBatch) {
    //static int textureID = ResourceManager::getTexture(texturePath).id;
    if (textureID != 0) {
        const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        glm::vec4 destRect;
        destRect.x = _position.x;
        destRect.y = _position.y;
        destRect.z = AGENT_WIDTH;
        destRect.w = AGENT_WIDTH;

        _spriteBatch.draw(destRect, uvRect, textureID, 0.0f, _color);
    } else {
        fatalError("textureID not set");
    }


}
bool Agent::applyDamage(float damage) {
    _health -= damage;
    if (_health <= 0) {
        return true;
    }
    return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& _levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y) {
/*
    // Ensure _levelData is non-empty
    if (_levelData.empty()) {
        std::cerr << "ERROR: _levelData is empty!" << std::endl;
        return;
    }*/

    if (std::isnan(x) || std::isnan(y)) {
        // x or y is NaN and offscreen ?
        return;
    }
    glm::vec2 cornerPos = glm::vec2(std::floor(x / (float)TILE_WIDTH),
                                      std::floor(y / (float)TILE_WIDTH));

    // outside world check
    if (cornerPos.x < 0 || cornerPos.x >= _levelData[0].length() || cornerPos.y < 0 || cornerPos.y >= _levelData.size()) return;

    if (_levelData[cornerPos.y][cornerPos.x] != '.') {
        collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
    }
}

bool Agent::collideWithLevel(const std::vector<std::string>& _levelData) {
    if (std::isnan(_position.x) || std::isnan(_position.y)) {
        //std::cerr << "ERROR: " << _position.x << ',' << _position.y << std::endl;
        return false;
    }

    std::vector<glm::vec2> collideTilePosition;
    checkTilePosition(_levelData, collideTilePosition, _position.x, _position.y);
    checkTilePosition(_levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y);
    checkTilePosition(_levelData, collideTilePosition, _position.x, _position.y + AGENT_WIDTH);
    checkTilePosition(_levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);
    if (collideTilePosition.size() == 0) {
        return false;
    }
    for (size_t i = 0; i < collideTilePosition.size(); i++) {
        collideWithTile(collideTilePosition[i]);
    }
    return true;
}
bool Agent::collideWithAgent(Agent* agent) {
    if (!agent) {
        std::cerr << "Error: collideWithAgent called with nullptr!" << std::endl;
        return false;
    }
    // Minimum distance before there is a collision
    const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

    // Center position of this agent and parameter agent
    glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
    glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

    // Distance vector between the two agents
    glm::vec2 distVec = centerPosA - centerPosB;

    float distance = glm::length(distVec);
    float collisionDepth = MIN_DISTANCE - distance;

    if (collisionDepth > 0) {
        glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
        _position += collisionDepthVec / 2.0f;
        agent->_position -= collisionDepthVec / 2.0f;

        return true;
    }
    return false;
}

void Agent::collideWithTile(glm::vec2 tilePos) {

    const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
    glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
    glm::vec2 distVec = centerPlayerPos - tilePos;

    float xdepth = MIN_DISTANCE - std::abs(distVec.x);
    float ydepth = MIN_DISTANCE - std::abs(distVec.y);

    /// collision
    if (xdepth > 0 && ydepth > 0) {
        if (std::max(xdepth, 0.0f) < std::max(ydepth, 0.0f)) {
            if (distVec.x < 0) {
                _position.x -= xdepth;
            } else {
                _position.x += xdepth;
            }
        } else {
            if (distVec.y < 0) {
                _position.y -= ydepth;
            } else {
                _position.y += ydepth;
            }
        }
    }
}



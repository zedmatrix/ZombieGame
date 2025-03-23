#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Sprite.hpp"
#include "ResourceManager.hpp"

const int TILE_WIDTH = 64;

class Level {

public:
    Level(const std::string& fileName);
    void draw();

    glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
    const std::vector<std::string>& getLevelData() const { return _levelData; }
    const std::vector<glm::vec2>& getStartZombiePos() const { return _zombieStartPos; }

    int getWidth() const { return _levelData[0].size(); }
    int getHeight() const { return _levelData.size(); }
    int getNumHumans() const { return _numHumans; }

private:
    std::vector<std::string> _levelData;
    int _numHumans;
    SpriteBatch _spriteBatch;
    glm::vec2 _startPlayerPos;
    std::vector<glm::vec2> _zombieStartPos;


};

#endif //LEVEL_HPP

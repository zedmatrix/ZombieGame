#include "Level.hpp"
#include "Error.hpp"
#include "Vertex.hpp"

Level::Level(const std::string& fileName) {

    std::ifstream file;
    file.open(fileName);
    if (file.fail()) {
        fatalError("Failed to Open " + fileName);

    }

    std::string tmp;
    file >> tmp >> _numHumans;

    std::getline(file, tmp);    // throw away first line and fill vector ? leftover whitespace gdb a=1, b=-1

    while (std::getline(file, tmp)) {
        _levelData.push_back(tmp);
    }

    _spriteBatch.init();
    _spriteBatch.begin();
    glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    Color whiteColor;
    whiteColor.r = 255;
    whiteColor.g = 255;
    whiteColor.b = 255;
    whiteColor.a = 255;

    // render the level tiles
    for (size_t y = 0; y < _levelData.size(); y++) {
        for (size_t x = 0; x < _levelData[y].size(); x++) {
            char tile = _levelData[y][x];
            //get dest rect

            glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

            switch (tile) {
                case 'R':
                    _spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
                    break;
                case 'L':
                    _spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture("Textures/wood.png").id, 0.0f, whiteColor);
                    break;
                case 'B':
                case 'G':
                    _spriteBatch.draw(destRect, uvRect, ResourceManager::getTexture("Textures/glass.png").id, 0.0f, whiteColor);
                    break;
                case '@':
                    _startPlayerPos.x = x * TILE_WIDTH;
                    _startPlayerPos.y = y * TILE_WIDTH;
                    _levelData[y][x] = '.';
                    break;
                case 'Z':
                    _zombieStartPos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
                    _levelData[y][x] = '.';
                    break;
                case '.':
                    break;
                default:
                    std::printf("Unexpected Symbol %c at (%ld, %ld)\n", tile, x, y);
                    break;

            }
        }

    }

    _spriteBatch.end();
}

void Level::draw() {
    _spriteBatch.renderBatch();

}

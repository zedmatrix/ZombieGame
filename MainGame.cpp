#include "MainGame.hpp"
#include "IOManager.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Timing.hpp"

const float HUMAN_SPEED = 2.0f;
const float ZOMBIE_SPEED = 1.25f;
const float PLAYER_SPEED = 5.0f;
int _playerHealth = 10000;

MainGame::MainGame () :
    _screenWidth(1024),
    _screenHeight(768),
    _fps(0),
    _numHumansKilled(0),
    _numZombiesKilled(0),
    _gameState(GameState::PLAY),
    _player(nullptr) {

}

/// Called from main.cpp
void MainGame::run() {

    initSystems();
    initLevel();
    gameLoop();

}

/// Initialize System
void MainGame::initSystems() {
    // Initialize Everything and Set Up Double Buffering
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    _mainWindow.create("Zombie Game", _screenWidth, _screenHeight, 0);
    initShaders();
    _agentSpriteBatch.init();
    _camera.init(_screenWidth, _screenHeight);
}

/// Initialize Level
void MainGame::initLevel() {
    _levels.push_back(std::make_unique<Level>("level1.txt"));

    _currentLevel = 0;

    // Initialize Player
    _player = new Player();
    _player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);
    _humans.emplace_back(_player);


    //// Gun(std::string name, int rate, int bps, int damage, float speed, float spread)
    const float BULLET_SPEED = 20.0f;
    _player->addGun(std::make_unique<Gun>("Magnum", 5, 5, 10.0f, BULLET_SPEED, 10.0f));
    _player->addGun(std::make_unique<Gun>("Shotgun", 10, 20, 5.0f, BULLET_SPEED, 40.0f));

    std::mt19937 randomEngine;
    randomEngine.seed(time(nullptr));
    std::uniform_int_distribution<int> randPosX(5, _levels[_currentLevel]->getWidth() - 5);
    std::uniform_int_distribution<int> randPosY(5, _levels[_currentLevel]->getHeight() - 5);

    // Human Initialize
    for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++) {
        _humans.push_back(std::make_unique<Human>());
        glm::vec2 pos(randPosX(randomEngine) * TILE_WIDTH, randPosY(randomEngine) * TILE_WIDTH);
        _humans.back()->init(HUMAN_SPEED, pos);
    }

    // Zombie start position
    const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getStartZombiePos();

    std::cout << "Zombie Start Positions: " << std::endl;
    for (size_t i = 0; i < zombiePositions.size(); i++) {
        _zombies.push_back(std::make_unique<Zombie>());
        _zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
    }
}

/// Initialize Shader Program
void MainGame::initShaders() {
    _colorProgram.compileShaders("Shaders/VertexShader", "Shaders/FragmentShader");
    _colorProgram.addAttribute("vertexPosition");
    _colorProgram.addAttribute("vertexColor");
    _colorProgram.addAttribute("vertexUV");
    _colorProgram.linkShaders();

}

void MainGame::gameLoop() {
    FpsLimiter _fpsLimiter;
    _fpsLimiter.init(60);

    while (_gameState == GameState::PLAY) {
        _fpsLimiter.begin();

        checkVictory();

        processInput();

        updateAgents();
        updateBullets();

        _camera.setPosition(_player->getPosition());

        _camera.update();

        drawGame();

        _fps = _fpsLimiter.end();
    }

}

void MainGame::updateAgents() {
    /// update all humans
    for (size_t i = 0; i < _humans.size(); i++) {
        _humans[i]->update(_levels[_currentLevel]->getLevelData(),
                           _humans, _zombies);
    }
    // update zombies
    for (size_t i = 0; i < _zombies.size(); i++) {
        _zombies[i]->update(_levels[_currentLevel]->getLevelData(),
                           _humans, _zombies);
    }
    // Update Human Collisions
    for (size_t i = 0; i < _humans.size(); i++) {
        for (size_t j = i + 1; j < _humans.size(); j++) {
            _humans[i]->collideWithAgent(_humans[j].get());
        }
    }
    // Update Zombie Collisions
    for (size_t i = 0; i < _zombies.size(); i++) {
        // Collide with Zombies
        for (size_t j = i + 1; j < _zombies.size(); j++) {
            _zombies[i]->collideWithAgent(_zombies[j].get());
        }
        // Collide with Humans
        for (size_t j = 1; j < _humans.size(); j++) {

            if (_zombies[i]->collideWithAgent(_humans[j].get())) {
                _zombies.push_back(std::make_unique<Zombie>());
                _zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
                _humans[j] = std::move(_humans.back());
                _humans.pop_back();
            }
        }
        if (_zombies[i]->collideWithAgent(_player)) {
            std::cout << "Zombie Attacked You: " << _playerHealth-- << std::endl;
        }
    }


}
void MainGame::updateBullets() {
    for (size_t i = 0; i < _bullets.size(); ) {
        if (_bullets[i].update(_levels[_currentLevel]->getLevelData())) {
            _bullets[i] = _bullets.back();
            _bullets.pop_back();
        } else {
            i++;
        }
    }
    bool wasBulletRemoved;
    // Collide with Humans and Zombies
    for (size_t i = 0; i < _bullets.size(); i++) {
        wasBulletRemoved = false;
        for (size_t j = 0; j < _zombies.size(); ) {
            if (_bullets[i].collideWithAgent(_zombies[j].get())) {
                //damage zombie

                if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
                    // kill the zombie
                    _zombies[j] = std::move(_zombies.back());
                    _zombies.pop_back();
                    _numZombiesKilled++;
                } else {
                    j++;
                }
                // remove the bullet
                _bullets[i] = _bullets.back();
                _bullets.pop_back();
                wasBulletRemoved = true;
                i--;
                break;  ///< bullet is gone
            } else {
                j++;
            }
        }
        //Loop thru humans
        if (wasBulletRemoved == false) {
            for (size_t j = 1; j < _humans.size(); j++) {   ///< skip 0 = player
                if (_bullets[i].collideWithAgent(_humans[j].get())) {
                    if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
                        // kill the human
                        _humans[j] = std::move(_humans.back());
                        _humans.pop_back();
                        _numHumansKilled++;
                    } else {
                        j++;
                    }
                    _bullets[i] = _bullets.back();
                    _bullets.pop_back();

                    wasBulletRemoved = true;
                    break;
                } else {
                    j++;
                }
            }
        }
    }
} //end of updateBullets
void MainGame::checkVictory() {
    /// TODO multiple level change

    if (_zombies.empty()) {
        std::printf("*** You Win ***\n\t You Killed %d Humans and %d Zombies\n\t There are %ld out of %d humans left",
                    _numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_currentLevel]->getNumHumans());
        _gameState = GameState::EXIT;
    }
    if (_humans.size() < 2) {
        std::printf("*** You Loose ***\n\t You Killed %d Humans and %d Zombies\n\t There are %ld Zombies Left\n",
                    _numHumansKilled, _numZombiesKilled, _zombies.size());
        _gameState = GameState::EXIT;
    }

}

void MainGame::processInput() {
    SDL_Event _event;
    while (SDL_PollEvent(&_event)) {
        switch (_event.type) {
            case SDL_QUIT:
                _gameState = GameState::EXIT;
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(_event.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(_event.button.button);
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(_event.motion.x, _event.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.pressKey(_event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.releaseKey(_event.key.keysym.sym);
                break;
        }
        if (_inputManager.isKeyPressed(SDLK_q)) _gameState = GameState::EXIT;
    }
}

void MainGame::drawGame() {
    //Set the base depth and clear color and depth buffer
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);

    _colorProgram.use();
    // binding textures

    GLuint textureLocation = _colorProgram.getUniformLocation("mySampler");
    glUniform1i(textureLocation, 0);

    glm::mat4 cameraMatrix = _camera.getCameraMatrix();

    GLuint pLocation = _colorProgram.getUniformLocation("P");

    glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(cameraMatrix));

    /// Draw the Level Data
    _levels[_currentLevel]->draw();

    /// Draw the Humans and Players
    _agentSpriteBatch.begin();

    /// Player should always be 0 as initialized first
    _humans[0]->setTexture("Textures/player.png");
    _humans[0]->draw(_agentSpriteBatch);

    /// Humans are next in initialization
    for (size_t i = 1; i < _humans.size(); i++) {
        _humans[i]->setTexture("Textures/human.png");
        _humans[i]->draw(_agentSpriteBatch);
    }

    /// Zombies are next in initialization
    for (size_t i = 0; i < _zombies.size(); i++) {
        _zombies[i]->setTexture("Textures/zombie.png");
        _zombies[i]->draw(_agentSpriteBatch);
    }

    for (size_t i = 0; i < _bullets.size(); i++) {
        _bullets[i].draw(_agentSpriteBatch);
    }
    _agentSpriteBatch.end();
    _agentSpriteBatch.renderBatch();


    // Swap our buffer and unbind opengl
    glBindTexture(GL_TEXTURE_2D, 0);
    _colorProgram.unuse();
    _mainWindow.swapBuffer();
}

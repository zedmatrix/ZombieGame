#ifndef MAINGAME_HPP
#define MAINGAME_HPP
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <format>

#include "MainWindow.hpp"
#include "Error.hpp"
#include "GLSLProgram.hpp"
#include "GLTexture.hpp"
#include "ResourceManager.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Zombie.hpp"
#include "Weapon.hpp"
#include <random>
#include <ctime>

enum class GameState {PLAY, EXIT};

class MainGame {

public:
    MainGame();

    void run();

private:
    void initSystems();
    void initLevel();
    void initShaders();
    void gameLoop();
    void processInput();
    void checkVictory();
    void drawGame();
    void updateAgents();
    void updateBullets();

    MainWindow _mainWindow;
    int _screenWidth;
    int _screenHeight;
    float _fps;
    int _currentLevel;
    int _numHumansKilled;
    int _numZombiesKilled;

    GameState _gameState;
    Camera _camera; ///< main camera
    SpriteBatch _agentSpriteBatch; ///< draws all agents
    GLSLProgram _colorProgram;
    Input _inputManager;

    Player* _player;
    std::vector<std::unique_ptr<Level>> _levels;
    std::vector<std::unique_ptr<Human>> _humans; ///< all humans + player
    std::vector<std::unique_ptr<Zombie>> _zombies; ///< all zombies
    std::vector<Bullet> _bullets;
};

#endif //MAINGAME_HPP

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <format>
#include "Error.hpp"

enum MainWindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4, MAXIMIZED = 0x8 };

class MainWindow {

public:
    MainWindow();
    ~MainWindow();

    int create(std::string windowTitle, int screenWidth, int screenHeight, uint32_t windowFlags);

    void swapBuffer();
    int getScreenWidth() { return _screenWidth; }
    int getScreenHeight() { return _screenHeight; }

private:
    SDL_Window* _sdlWindow;
    int _screenWidth, _screenHeight;

};

#endif //MAINWINDOW_HPP
/*
SDL_WINDOW_FULLSCREEN = 0x00000001,         < fullscreen window
SDL_WINDOW_OPENGL = 0x00000002,             < window usable with OpenGL context
SDL_WINDOW_SHOWN = 0x00000004,              < window is visible
SDL_WINDOW_HIDDEN = 0x00000008,             < window is not visible
SDL_WINDOW_BORDERLESS = 0x00000010,         < no window decoration
SDL_WINDOW_RESIZABLE = 0x00000020,          < window can be resized
SDL_WINDOW_MINIMIZED = 0x00000040,          < window is minimized
SDL_WINDOW_MAXIMIZED = 0x00000080,          < window is maximized
*/

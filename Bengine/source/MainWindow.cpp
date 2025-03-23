#include "MainWindow.hpp"

MainWindow::MainWindow() {

}
MainWindow::~MainWindow() {

}

int MainWindow::create(std::string windowTitle, int screenWidth, int screenHeight, uint32_t windowFlags) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;

    uint32_t _flags = SDL_WINDOW_OPENGL;

    if (windowFlags & INVISIBLE) _flags |= SDL_WINDOW_HIDDEN;
    if (windowFlags & FULLSCREEN) _flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (windowFlags & BORDERLESS) _flags |= SDL_WINDOW_BORDERLESS;
    if (windowFlags & MAXIMIZED) _flags |= SDL_WINDOW_MAXIMIZED;

    // Create your window using the predefine variables
    _sdlWindow = SDL_CreateWindow(windowTitle.c_str(),
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               _screenWidth,
                               _screenHeight,
                               _flags);
    if (_sdlWindow == nullptr) {
        fatalError("SDL Window could not be created!");
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
    if (glContext == nullptr) {
        fatalError("SDL_GL Context could not be initialized");
    }
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        fatalError("Could not Initialize GLEW");
    }
    std::cout << "*** OpenGL Version: " << glGetString(GL_VERSION) << " ***\n";
    std::cout << "*** OpenGL Renderer: " << glGetString(GL_RENDERER) << " ***\n";

    //set background color dark-semi-transparent grey
    glClearColor(0.3f, 0.2f, 0.2f, 0.2f);

    // Enable VSYNC
    SDL_GL_SetSwapInterval(1);

    // Enable Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void MainWindow::swapBuffer() {
    SDL_GL_SwapWindow(_sdlWindow);
}

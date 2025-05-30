#pragma once
#include <GL/glew.h>

struct Position {
    float x;
    float y;
};

struct Color {
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

struct UV {
    float u;
    float v;
};

struct Vertex {

    // position of sprite/texture X, Y
    Position position;

    // Color 4 bytes Red, Green, Blue, Alpha channels
    Color color;

    //UV Texture Coordinates
    UV uv;

    void setPos(float x, float y) {
        position.x = x;
        position.y = y;
    }
    void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }
    void setUV(float u, float v) {
        uv.u = u;
        uv.v = v;
    }
};

#ifndef SPRITE_HPP
#define SPRITE_HPP
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <random>
#include <string>
#include "GLTexture.hpp"
#include "Vertex.hpp"

enum class GlyphSortType {
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

// Each render batch is used for a single draw call
class RenderBatch {
public:
    RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset),
        numVertices(NumVertices), texture(Texture) {
    }
    GLuint offset;
    GLuint numVertices;
    GLuint texture;
};

class Glyph {
public:
    Glyph() {};
    Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Color& color);

    GLuint texture;
    float depth;

    Vertex topLeft;
    Vertex bottomLeft;
    Vertex topRight;
    Vertex bottomRight;
private:

};

class SpriteBatch {

public:
    SpriteBatch() : _vbo(0), _vao(0) {}
    void init();
    void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
    void end();
    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);
    void renderBatch();

private:
    void createRenderBatches();
    void createVertexArray();
    void sortGlyphs();
    GLuint _vbo;
    GLuint _vao;
    std::vector<Glyph> _glyphs;
    std::vector<RenderBatch> _renderBatches;
    GlyphSortType _sortType;

    GLuint lastTexture = 0; ///< debugging textures

};

#endif //SPRITE_HPP

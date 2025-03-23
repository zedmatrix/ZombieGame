#include "Sprite.hpp"
#include <iostream>
/// Glyph
Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Color& color) :
    texture(Texture),
    depth(Depth) {

    topLeft.color = color;
    topLeft.setPos(destRect.x, destRect.y + destRect.w);
    topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    bottomLeft.color = color;
    bottomLeft.setPos(destRect.x, destRect.y);
    bottomLeft.setUV(uvRect.x, uvRect.y);

    bottomRight.color = color;
    bottomRight.setPos(destRect.x + destRect.z, destRect.y);
    bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    topRight.color = color;
    topRight.setPos(destRect.x + destRect.z, destRect.y + destRect.w);
    topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}

//SpriteBatch
void SpriteBatch::init() {
    createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType /*GlyphSortType::TEXTURE */) {
    _sortType = sortType;
    _renderBatches.clear();
    _glyphs.clear();
}

void SpriteBatch::end() {
    sortGlyphs();
    createRenderBatches();
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color) {
    _glyphs.emplace_back(destRect, uvRect, texture, depth, color);

}

void SpriteBatch::createRenderBatches() {
    std::vector <Vertex> vertices;
    vertices.resize(_glyphs.size() * 6);

    if (_glyphs.empty()) return;

    int offset{0};
    int currentVertex{0};
    _renderBatches.emplace_back(offset, 6, _glyphs[0].texture);
    vertices[currentVertex++] = _glyphs[0].topLeft;
    vertices[currentVertex++] = _glyphs[0].bottomLeft;
    vertices[currentVertex++] = _glyphs[0].bottomRight;
    vertices[currentVertex++] = _glyphs[0].bottomRight;
    vertices[currentVertex++] = _glyphs[0].topRight;
    vertices[currentVertex++] = _glyphs[0].topLeft;
    offset += 6;

    for (size_t currentGlyph = 1; currentGlyph < _glyphs.size(); currentGlyph++) {

        if (_glyphs[currentGlyph].texture != _glyphs[currentGlyph - 1].texture) {
            _renderBatches.emplace_back(offset, 6, _glyphs[currentGlyph].texture);
        } else {
            _renderBatches.back().numVertices += 6;
        }
        vertices[currentVertex++] = _glyphs[currentGlyph].topLeft;
        vertices[currentVertex++] = _glyphs[currentGlyph].bottomLeft;
        vertices[currentVertex++] = _glyphs[currentGlyph].bottomRight;
        vertices[currentVertex++] = _glyphs[currentGlyph].bottomRight;
        vertices[currentVertex++] = _glyphs[currentGlyph].topRight;
        vertices[currentVertex++] = _glyphs[currentGlyph].topLeft;
        offset += 6;
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void SpriteBatch::createVertexArray() {

    if (_vao == 0) glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    if (_vbo == 0) glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Position Attribute pointer // (void*)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);

}
void SpriteBatch::sortGlyphs() {
    switch (_sortType) {
        case GlyphSortType::FRONT_TO_BACK:
            std::stable_sort(_glyphs.begin(), _glyphs.end(),
                [](const Glyph& a, const Glyph& b) { return a.depth < b.depth; });
            break;
        case GlyphSortType::BACK_TO_FRONT:
            std::stable_sort(_glyphs.begin(), _glyphs.end(),
                [](const Glyph& a, const Glyph& b) { return a.depth > b.depth; });
            break;
        case GlyphSortType::TEXTURE:
            std::stable_sort(_glyphs.begin(), _glyphs.end(),
                [](const Glyph& a, const Glyph& b) { return a.texture < b.texture; });
            break;
        case GlyphSortType::NONE:
            break;
    }
}
void SpriteBatch::renderBatch() {
    // Bind our VAO. This sets up the opengl state we need, including the
    // vertex attribute pointers and it binds the VBO
    glBindVertexArray(_vao);

    for (size_t i = 0; i < _renderBatches.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
        glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
    }
    glBindVertexArray(0);
}

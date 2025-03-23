#include "TextureCache.hpp"
#include "IOManager.hpp"
#include <utility>
#include <iostream>
TextureCache::TextureCache() {

}

TextureCache::~TextureCache() {

}

GLTexture TextureCache::getTexture(std::string texturePath) {

    // Lookup texture map in map
    std::map<std::string, GLTexture>::iterator mit = _textureMap.find(texturePath);

    if (mit == _textureMap.end()) {
        GLTexture newTexture = IOManager::loadPNG(texturePath);

        _textureMap.insert(make_pair(texturePath, newTexture));

        std::cout << "New Texture Loaded!\n";
        return newTexture;
    }
    return mit->second;

}

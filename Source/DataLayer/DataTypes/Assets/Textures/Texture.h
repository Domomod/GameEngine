//
// Created by dominik on 29.03.19.
//

#ifndef GAMEENGINE_TEXTURE_H
#define GAMEENGINE_TEXTURE_H

#include <GL/gl3w.h>

class Texture {
    friend class TextureLoader;
public:
    GLuint texID;
    void bind();

    virtual ~Texture();

private:
    Texture(const int &width, const int &height, unsigned char *data);
};
#endif //GAMEENGINE_TEXTURE_H
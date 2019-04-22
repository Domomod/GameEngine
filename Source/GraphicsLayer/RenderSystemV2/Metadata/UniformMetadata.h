//
// Created by dominik on 10.03.19.
//

#ifndef GAMEENGINE_UNIFORMMETADATA_H
#define GAMEENGINE_UNIFORMMETADATA_H

#include <GL/gl3w.h>
#include <cstddef>

#include "Source/MyExceptions.h"

struct UniformMetadata {
    void* pointerToData;
    GLenum dataType;
    size_t size;
    size_t allign;

    UniformMetadata(void *pointerToData, GLenum dataType);
    UniformMetadata();

    virtual ~UniformMetadata();
};


#endif //GAMEENGINE_UNIFORMMETADATA_H
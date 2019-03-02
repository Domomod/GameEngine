
//
// Created by dominik on 09.02.19.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include "../View/Window.h"
#include "../View/Shader.h"
#include "../MyExceptions.h"
#include "OnChangeListener.h"
#include "Mesh.h"
#include "ToGPUattribueSender.h"
#include "ToGPUniformSender.h"
#include "AbstractRenderer.h"

class AbstractApplication {
public:
    AbstractApplication();

    virtual ~AbstractApplication();

    void start();
private:
    //View
    Window mainWindow;
    std::shared_ptr<AbstractRenderer> renderer;
    GLint maxPatchVerticies = 0;

    void initialiseOpenGL();

    virtual void initialiseRenderer() = 0;
    virtual void initialiseCommunication() = 0;
    virtual void mainLoop() = 0;

    static void GLAPIENTRY
    MessageCallback( GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam );
};


#endif //GAMEENGINE_APPLICATION_H

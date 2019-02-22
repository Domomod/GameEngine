//
// Created by dominik on 09.02.19.
//

#include "Application.h"

#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


Application::Application() : mainWindow("Game Engine", 800, 600) {
    generateSymetricalRectanuglarMesh(4, 4, 3.f, 3.f, 0.0f, -0.5f, -3.0f);

    Projection = glm::perspective(FOV, aspect, zNear, zFar);

    onWindowResizeProjectionUpdater.setReactionFuncPtr([&](std::pair<int,int> newWidthHeight){
        int width = newWidthHeight.first;
        int height = newWidthHeight.second;
        aspect = width / height;
        Projection = glm::perspective(FOV, aspect, zNear, zFar);
    });
}


Application::~Application() {
    mainShader.deleteProgram();
    glDeleteBuffers(1, &vboIndiciesId);
    glDeleteBuffers(1, &vboVerticesId);
    glDeleteVertexArrays(1, &vaoId);
}


void Application::start() {
    //There must be a current window before OpenGL initialisation
    mainWindow.makeCurrent();
    mainWindow.getResizeNotifierPtr()->addListener(&onWindowResizeProjectionUpdater);

    //OpenGL initialisation
    if(gl3wInit()) {
        std::cerr << "Failed to initialize OpenGl.\n";
        throw GlfwInitalisationFailedException();
    }

    std::cout << "\tVendor:\t" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "\tRenderer:\t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "\tOpenGL version:\t" << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tGLSL version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glClearColor(0.1,0.1,0.1,0);
    glfwSwapInterval(1);

    //Shader loading and consolidation
    try {
        mainShader.loadFromFile(Shader::VERTEX, shadersPath + "basic.vert");
        mainShader.loadFromFile(Shader::FRAGMENT, shadersPath + "basic.frag");
        mainShader.createAndLinkProgram();
        mainShader.use();
        mainShader.addAttribute("position");
        mainShader.addAttribute("color");
        mainShader.addUniform("ModelViewProjection");
        mainShader.unuse();
    } catch( MyException& e) {
        std::cerr << e.getType() << ":\n" << e.getMessage();
        throw e;
    }
    //TODO: ask for proper path if sth goes wrong

    mainLoop();
}


void Application::mainLoop() {
    createBuffers();
    insertGeometryAndTopologyIntoBuffers();
    while(mainWindow.isRunning()){
        Render();
        glfwPollEvents();
    }
}

void Application::createBuffers() {
    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboVerticesId);
    glGenBuffers(1, &vboIndiciesId);
}

void Application::insertGeometryAndTopologyIntoBuffers() {
    GLsizei stride = sizeof(verticies[0]);

    size_t indicesSize = indicies.size() * sizeof(indicies[0]);
    size_t verticesSize = verticies.size() * sizeof(verticies[0]);
    size_t positionOffset = offsetof(Vertex, position);
    size_t colorOffset = offsetof(Vertex, color);

    GLuint positionLocation = static_cast<GLuint>(mainShader.getAttribute("position"));
    GLuint colorLocation = static_cast<GLuint>(mainShader.getAttribute("color"));
    GLuint numElements = static_cast<GLuint>(verticies.size()); // NOLINT(modernize-use-auto)


    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesId);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, &verticies[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(positionOffset));

    glBufferData(GL_ARRAY_BUFFER, verticesSize, &verticies[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(colorOffset));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndiciesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, &indicies[0], GL_STATIC_DRAW);

}

void Application::Render() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    mainShader.use();

    GLuint numIndicies = static_cast<GLuint>(indicies.size());
    GLboolean transpose = GL_FALSE;
    glUniformMatrix4fv(mainShader.getUniform("ModelViewProjection"), 1, transpose, glm::value_ptr(Projection * ModelView));
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_SHORT, 0);

    mainShader.unuse();
    mainWindow.swapBuffers();
}

void Application::MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                  const GLchar *message, const void *userParam) {
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message );
    if(type == GL_DEBUG_TYPE_ERROR)
        throw new OpenGlException();
}


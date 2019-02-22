
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

class Application {
public:
    Application();

    virtual ~Application();

    void start();

private:
    //Object data
    class Vertex{
    public:
        glm::vec3 color;
        glm::vec3 position;

        Vertex(const glm::vec3 &color, const glm::vec3 &position) : color(color), position(position) {}
    };
    std::vector<Vertex> verticies;
    std::vector<GLushort> indicies;

    //View
    Window mainWindow;
    Shader mainShader;

    GLfloat FOV = 45.0;
    GLfloat aspect = 800.f/600.f;
    GLfloat zNear = 1;
    GLfloat zFar = 1000;

    std::string shadersPath = "../Shaders/";

    GLuint vaoId;
    GLuint vboVerticesId;
    GLuint vboIndiciesId;

    //Communication
    OnChangeListener<std::pair<int,int>> onWindowResizeProjectionUpdater;

    //Math
    glm::mat4 Projection = glm::mat4(1);
    glm::mat4 ModelView = glm::mat4(1);


    void mainLoop();
    void createBuffers();

    void genereateRectangularGeometry(GLushort numVerticiesAlongXaxis, GLushort numVerticiesAlongZaxis, GLfloat sizeX,
                                          GLfloat sizeZ, GLfloat centerX, GLfloat centerY, GLfloat centerZ) {
        float halfSizeX = sizeX/2;
        float halfSizeZ = sizeZ/2;
        GLushort numEdgesAlongXaxis = numVerticiesAlongXaxis - short(1);
        GLushort numEdgesAlongZaxis = numVerticiesAlongZaxis - short(1);
        float stepX = sizeX / numEdgesAlongXaxis;
        float stepZ = sizeZ/ numEdgesAlongZaxis;

        for(int j=0;j<numVerticiesAlongZaxis;j++) {
            for(int i=0;i<numVerticiesAlongXaxis;i++) {
                float x = -halfSizeX + centerX + i*stepX;
                float y = centerY;
                float z = -halfSizeZ + centerZ + j*stepZ;

                glm::vec3 color(1);
                glm::vec3 position(x,y,z);

                verticies.emplace_back(color,position);
            }
        }
    }

    void generateSymetricalRectanuglarMesh(GLushort numVerticiesAlongXaxis = 2, GLushort numVerticiesAlongZaxis = 2,
                                               GLfloat sizeX = 100.0, GLfloat sizeZ = 100.0, GLfloat centerX = 0.0f,
                                               GLfloat centerY = 0.0f, GLfloat centerZ = 0.0f) {
        genereateRectangularGeometry(numVerticiesAlongXaxis, numVerticiesAlongZaxis, sizeX, sizeZ, centerX, centerY,
                centerZ);

        GLushort numEdgesAlongXaxis = numVerticiesAlongXaxis - short(1);
        GLushort numEdgesAlongZaxis = numVerticiesAlongZaxis - short(1);

        for (int j = 0; j < numEdgesAlongZaxis; j++) {
            for (int i = 0; i < numEdgesAlongXaxis; i++) {
                int column = i;
                int row = j * numVerticiesAlongXaxis;
                int nexColumn = column + 1;
                int nextRow = row + numVerticiesAlongXaxis;

                int topLeft = row + column;
                int topRight = row + nexColumn;
                int bottomLeft = nextRow + column;
                int bottomRight = nextRow + nexColumn;

                if ((j+i)%2) {
                    indicies.push_back(topLeft);
                    indicies.push_back(bottomLeft);
                    indicies.push_back(topRight);

                    indicies.push_back(topRight);
                    indicies.push_back(bottomLeft);
                    indicies.push_back(bottomRight);
                } else {
                    indicies.push_back(topLeft);
                    indicies.push_back(bottomLeft);
                    indicies.push_back(bottomRight);

                    indicies.push_back(topLeft);
                    indicies.push_back(bottomRight);
                    indicies.push_back(topRight);
                }
            }
        }
    }

    void generateAsymetricalRectanuglarMesh(GLushort numVerticiesAlongXaxis = 2, GLushort numVerticiesAlongZaxis = 2,
                                                GLfloat sizeX = 10.0, GLfloat sizeZ = 10.0, GLfloat centerX = 0.0f,
                                                GLfloat centerY = 0.0f, GLfloat centerZ = 0.0f) {
        genereateRectangularGeometry(numVerticiesAlongXaxis, numVerticiesAlongZaxis, sizeX, sizeZ, centerX, centerY,
                                     centerZ);

        GLushort numEdgesAlongXaxis = numVerticiesAlongXaxis - short(1);
        GLushort numEdgesAlongZaxis = numVerticiesAlongZaxis - short(1);

        for (int j = 0; j < numEdgesAlongZaxis; j++) {
            for (int i = 0; i < numEdgesAlongXaxis; i++) {
                int column = i;
                int row = j * numVerticiesAlongXaxis;
                int nexColumn = column + 1;
                int nextRow = row + numVerticiesAlongXaxis;

                int topLeft = row + column;
                int topRight = row + nexColumn;
                int bottomLeft = nextRow + column;
                int bottomRight = nextRow + nexColumn;

                indicies.push_back(topLeft);
                indicies.push_back(bottomLeft);
                indicies.push_back(topRight);

                indicies.push_back(topRight);
                indicies.push_back(bottomLeft);
                indicies.push_back(bottomRight);
            }
        }
    }

    void generateTriangeMesh(){
        verticies.emplace_back(
                glm::vec3(1,1,1), glm::vec3(-1,-0.2,0)
        );
        verticies.emplace_back(
                glm::vec3(1,1,1), glm::vec3(0,-0.2,-20)
        );
        verticies.emplace_back(
                glm::vec3(1,1,1), glm::vec3(1,-0.2,0)
        );

        indicies.push_back(0);
        indicies.push_back(1);
        indicies.push_back(2);
    }

    void generateSimpleRectangleMesh(){
        verticies.emplace_back(
                glm::vec3(0,1,1), glm::vec3(-50,-0.7,-50)
        );
        verticies.emplace_back(
                glm::vec3(1,0,1), glm::vec3(50,-0.7,-50)
        );
        verticies.emplace_back(
                glm::vec3(1,1,0), glm::vec3(-50,-0.7,50)
        );
        verticies.emplace_back(
                glm::vec3(1,1,1), glm::vec3(50,-0.7,50)
        );

        indicies.push_back(0);
        indicies.push_back(2);
        indicies.push_back(1);

        indicies.push_back(1);
        indicies.push_back(2);
        indicies.push_back(3);
    }

    void generateMoreComplexRectangleMesh(){
        verticies.emplace_back(
                glm::vec3(0,1,1), glm::vec3(-50,-0.7,-50)
        );
        verticies.emplace_back(
                glm::vec3(0,1,1), glm::vec3(0,-0.7,-50)
        );
        verticies.emplace_back(
                glm::vec3(0,1,1), glm::vec3(50,-0.7,-50)
        );
        verticies.emplace_back(
                glm::vec3(0,1,1), glm::vec3(-50,-0.7,0)
        );
        verticies.emplace_back(
                glm::vec3(0,1,1), glm::vec3(0,-0.7,0)
        );
        verticies.emplace_back(
                glm::vec3(0,1,1), glm::vec3(50,-0.7,0)
        );
        verticies.emplace_back(
                glm::vec3(0,1,1), glm::vec3(-50,-0.7,50)
        );
        verticies.emplace_back(
                glm::vec3(0,1,1), glm::vec3(0,-0.7,50)
        );
        verticies.emplace_back(
                glm::vec3(0,1,1), glm::vec3(50,-0.7,50)
        );


        indicies.push_back(0);
        indicies.push_back(3);
        indicies.push_back(1);

        indicies.push_back(1);
        indicies.push_back(3);
        indicies.push_back(4);

        indicies.push_back(1);
        indicies.push_back(4);
        indicies.push_back(2);

        indicies.push_back(2);
        indicies.push_back(4);
        indicies.push_back(5);

        indicies.push_back(3);
        indicies.push_back(6);
        indicies.push_back(4);

        indicies.push_back(4);
        indicies.push_back(6);
        indicies.push_back(7);

        indicies.push_back(4);
        indicies.push_back(7);
        indicies.push_back(5);

        indicies.push_back(5);
        indicies.push_back(7);
        indicies.push_back(8);

    }

    void insertGeometryAndTopologyIntoBuffers();
    void Render();

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
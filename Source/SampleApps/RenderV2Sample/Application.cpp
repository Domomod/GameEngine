//
// Created by dominik on 17.03.19.
//

#include "Source/GraphicsLayer/OpenGLinitializer.h"
#include "Application.h"

#include "Source/GraphicsLayer/RenderSystemV2/Buffers/VertexArrayObject.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/AttributeBuffer.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/ElementArrayBuffer.h"
#include "Source/GraphicsLayer/RenderSystemV2/Metadata/AttributeMetadata.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/UniformBuffer.h"

#include "Source/DataLayer/DataTypes/Assets/MeshGenerator.h"
#include "Source/DataLayer/DataTypes/Assets/ModelLoader.h"
#include "Source/DataLayer/DataTypes/Assets/Model.h"
#include "Source/DataLayer/DataTypes/Assets/Textures/Texture.h"
#include "Source/DataLayer/DataTypes/Assets/Textures/TextureLoader.h"

Application::Application() {
    openGlInitalizer = std::make_shared<OpenGlInitalizer>();
    openGlInitalizer->initGLFW();
    window = std::make_shared<Window>();
    window->makeCurrent();
    openGlInitalizer->initOpenGL();

    freeCamera = std::make_shared<FreeCamera>();


    windowInputSystem = std::make_shared<WindowInputSystem>();
    windowInputSystem->connectToWindow(*window);
    windowInputSystem->connectToKeyboardStateListener(freeCamera->getKeyboardStateListener());
    windowInputSystem->connectToMouseMovedListener(freeCamera->getMouseMovementListener());

    animationShader = std::make_shared<Shader>();

    try {
        animationShader->loadFromFile(Shader::VERTEX, "../Shaders/AnimationShader/animated.vs");
        animationShader->loadFromFile(Shader::FRAGMENT, "../Shaders/AnimationShader/animated.fs");
        animationShader->createAndLinkProgram();
    } catch( MyException& e) {
        std::cerr << e.getType() << ":\n" << e.getMessage();
        throw e;
        //TODO: ask for proper path if sth goes wrong
    }

    View = glm::lookAt(glm::vec3(0.f,0.f,3.f), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f,1.f,0.f));
    Projection = glm::perspective(glm::radians(45.f),8.f/4.f, 1.f, 150.f);

    windowResizeListener.setReactionFuncPtr([&](std::pair<int,int> size){
        Projection =  glm::perspective(glm::radians(45.f),(float)size.first / (float)size.second, 1.f, 30.f);
    });

    window->getResizeNotifierPtr()->addListener(&windowResizeListener);

    try {
//        entitySystem.addModel("Triangle",   ModelFactory()
//                .addMesh( MeshGenerator::generateTriangeMesh() )
//                .make()
//        );
//        entitySystem.addEntity("T1",
//                               entitySystem.entityFactory.make("Triangle", glm::vec3(0.f, 0.f, 0.f)));
//        entitySystem.addEntity("T2",
//                               entitySystem.entityFactory.make("Triangle", glm::vec3(1.f, 0.f, 0.f)));
//        entitySystem.addEntity("T3",
//                               entitySystem.entityFactory.make("Triangle", glm::vec3(-1.f, 0.f, 0.f)));
//        entitySystem.addEntity("T4",
//                               entitySystem.entityFactory.make("Triangle", glm::vec3(2.f, 2.f, 0.f)));
//
//        entitySystem.addModel("Quad",       ModelFactory()
//                .addMesh( MeshGenerator::generateSimpleRectangleMesh(10, -1, 10) )
//                .make()
//        );
//
//        entitySystem.addEntity("Q1",
//                               entitySystem.entityFactory.make("Quad", glm::vec3(-10, -1.7, -20)));
//        entitySystem.addEntity("Q2",
//                               entitySystem.entityFactory.make("Quad", glm::vec3(10, -1.7, -20)));
//        entitySystem.addEntity("Q3",
//                               entitySystem.entityFactory.make("Quad", glm::vec3(-10, -1.7, 0)));
//        entitySystem.addEntity("Q4", entitySystem.entityFactory.make("Quad", glm::vec3(10, -1.7, 0)));
//
//        entitySystem.addModel("Barrel", ModelLoader::loadModel("Meshes/barrel.obj"));
//        entitySystem.addEntity("B1", entitySystem.entityFactory.make("Barrel", glm::vec3(0, 0, -10)));


        entitySystem.addModel("Cowboy", ModelLoader::loadModel("Meshes/cowboy.dae"));
        entitySystem.addEntity("C1", entitySystem.entityFactory.make("Cowboy", glm::vec3(0, -5, -20), glm::vec3(-90.f, 0.f, 0.f)) );
    } catch (MeshLoadingException& e){
        std::cerr << e.getMessage();
        exit(1);
    }
}

void Application::main() {
    VertexArrayObject vao;
    vao.bind();

    AttributeBuffer posBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(0, 4, GL_FLOAT, 0, sizeof(glm::vec4)))
            .make();

    AttributeBuffer texCoordBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(1, 2, GL_FLOAT, 0, sizeof(glm::vec2)))
            .make();

    AttributeBuffer boneIdsBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(2, 4, GL_FLOAT, 0, sizeof(glm::vec4)))
            .make();

    AttributeBuffer boneWeightsBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(3, 4, GL_FLOAT, 0, sizeof(glm::vec4)))
            .make();

    ElementArrayBuffer elementArrayBuffer;

    const std::shared_ptr<Model> cowboy = entitySystem.getModel("Cowboy");
    const std::vector<glm::mat4>* cowboy_models = entitySystem.getAllFromModelSpaceMatricesForModel("Cowboy");

    UniformBuffer animatedUniformBuffer = UniformBufferFactory()
            .setBinding(2)
            .insert( UniformMetadata( &ModelViewProjection, GL_FLOAT_MAT4 ) )
            .insert( UniformMetadata( cowboy->animator->getCurrentPoseTransformation(), GL_FLOAT_MAT4, SkeletalSystem::MAX_BONES) )
            .make();

    std::shared_ptr<Texture> cowboyTexture = TextureLoader::loadTexture("Textures/cowboy.png");

    cowboy->animator->setCurrentAnimation(cowboy->skeletalAnimation);

    glEnable(GL_DEPTH_TEST);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    animationShader->use();

    while(window->isRunning()){
        time = static_cast<float>(glfwGetTime());
        View = freeCamera->calculateViewMatrix();

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        cowboyTexture->bind();
        cowboy->animator->calculateCurrentPose();

        ModelViewProjection = Projection * View * (*cowboy_models)[0];

        for( auto& mesh : cowboy->meshes) {

            posBuffer.bind();
            posBuffer.sendBufferToGPUifVaoBinded(mesh->positions);

            texCoordBuffer.bind();
            texCoordBuffer.sendBufferToGPUifVaoBinded(mesh->uv);

            boneIdsBuffer.bind();
            boneIdsBuffer.sendBufferToGPUifVaoBinded(mesh->boneIds);

            boneWeightsBuffer.bind();
            boneWeightsBuffer.sendBufferToGPUifVaoBinded(mesh->boneWeights);

            elementArrayBuffer.bind();
            elementArrayBuffer.sendIfVaoEnabled(mesh->indicies);

            animatedUniformBuffer.bind();
            animatedUniformBuffer.bakeData();
            animatedUniformBuffer.sendBufferToGPU();

            glDrawElements(GL_TRIANGLES, mesh->indicies.size(), GL_UNSIGNED_SHORT, nullptr);

        }

        boneIdsBuffer.unbind();
        boneWeightsBuffer.unbind();

        window->swapBuffers();
        glfwPollEvents();
        windowInputSystem->keyboardStateNotify();
    }

    waterShader->unuse();

    elementArrayBuffer.unbind();
    posBuffer.unbind();
    vao.unbind();
}

Application::~Application() {

}

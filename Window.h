//
// Created by dominik on 08.02.19.
//

#ifndef GAMEENGINE_WINDOW_H
#define GAMEENGINE_WINDOW_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <map>

#include "OnChangeListenerTemplate/OnChangeNotifier.h"
class Window {



private:
    GLFWwindow* window;

    //tip: inline static members don't need to be declared in cpp file
    inline static std::map<GLFWwindow*, OnChangeNotifier<std::pair<int,int>>> onWindowResizeNotifiersMap;

    static void onShutdown(GLFWwindow* window);

    static void onResize(GLFWwindow* window, int newWidth, int newHeight);

public:
    explicit Window(std::string title = "Window", int width = 800, int height = 400);

    virtual ~Window();

    void onRender();

    void makeCurrent();

    bool isRunning();

    OnChangeNotifier<std::pair<int,int>>& getResizeNotifier();

    Window(const Window&) = delete;
};


#endif //GAMEENGINE_WINDOW_H

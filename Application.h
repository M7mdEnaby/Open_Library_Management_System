#ifndef APPLICATION_H
#define APPLICATION_H

#include "BookManager.h"
#include "BookUI.h"
#include "ImGuiManager.h"

class Application {
public:
    void run();

private:
    void mainLoop(GLFWwindow* window);
};

#endif // APPLICATION_H

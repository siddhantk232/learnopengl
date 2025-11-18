#include <stdio.h>

#include "../include/glad/glad.h"
#include "GLFW/glfw3.h"

#define INFO(...)  fprintf(stderr, "[INFO] " __VA_ARGS__)
#define ERROR(...) fprintf(stderr, "[ERROR] " __VA_ARGS__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        INFO("Q pressed. Closing...\n");
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    if (!glfwInit()) {
        return 69;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 800;
    int height = 600;

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);

    if (!window) {
        ERROR("create GLFW window\n");
        glfwTerminate();
        return 69;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ERROR("init GLAD\n");
        return 69;
    }

    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}

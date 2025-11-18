#include <stdio.h>

#include "../include/glad/glad.h"
#include "GLFW/glfw3.h"

#define INFO(...) fprintf(stderr, "[INFO] " __VA_ARGS__)
#define ERROR(...) fprintf(stderr, "[ERROR] " __VA_ARGS__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    INFO("window resize w=%d h=%d\n", width, height);
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        INFO("Q pressed. Closing...\n");
        glfwSetWindowShouldClose(window, true);
    }
}

char const* const VERTEX_SHADER_SOURCE =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n";

char const* const FRAGMENT_SHADER_SOURCE =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

int checkCompileShader(char const* const* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char buf[512];
        glGetShaderInfoLog(shader, sizeof(buf), NULL, buf);
        ERROR("shader compile, type = %d: %s\n", shaderType, buf);
    }

    return shader;
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

    GLFWwindow* window =
        glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);

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

    GLuint vertexShader =
        checkCompileShader(&VERTEX_SHADER_SOURCE, GL_VERTEX_SHADER);

    GLuint fragmentShader =
        checkCompileShader(&FRAGMENT_SHADER_SOURCE, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    {
        int success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char buf[512];
            glGetProgramInfoLog(shaderProgram, sizeof(buf), NULL, buf);
            ERROR("shader program link: %s", buf);
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // opengl normalised coords
    // clang-format off
    float triangle[] = {
        -0.5f , -0.5f , 0.0f,
        0.5f  , -0.5f , 0.0f,
        0.0f  , 0.5f  , 0.0f,
    };
    // clang-format on

    GLuint VAO; // vertex array object
    glGenVertexArrays(1, &VAO);

    GLuint VBO; // vertex buffer object
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // 0. copy triangle in gpu memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    // 1. setup inputs (attributes) for vertex shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(triangle[0]), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}

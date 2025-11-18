#include <stdio.h>

#include "../include/glad/glad.h"
#include "GLFW/glfw3.h"

int main() {
    int x = glfwInit();

    printf("%d", x);
}

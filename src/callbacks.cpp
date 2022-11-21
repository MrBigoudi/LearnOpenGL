#include "callbacks.hpp"

#include <stdio.h>

void error_callback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 
#ifndef __CALLBACKS_HPP__
#define __CALLBACKS_HPP__

#include <GLFW/glfw3.h>

void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif
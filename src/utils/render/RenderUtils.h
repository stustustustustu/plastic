#ifndef RENDERUTILS_H
#define RENDERUTILS_H

#include "../src/config.h"
#include "../shader/ShaderUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool initializeWindow(GLFWwindow*& window, int width, int height, const std::string& title);
void drawRectangle(int x, int y, int width, int height, int hex);
void drawLine(float x1, float y1, float x2, float y2, float thickness, int hex);

#endif //RENDERUTILS_H

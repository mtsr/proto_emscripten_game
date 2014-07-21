#ifndef main_h
#define main_h

static void updateFPSCounter(GLFWwindow* window);

static void glfw_error_callback(int error, const char* description);
static void glfw_windowClose_callback(GLFWwindow* window);
static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void glfw_windowSize_callback (GLFWwindow* window, int width, int height);

#endif

#include "ve/window.h"

#include "ve/dim.h"

#include <GLFW/glfw3.h>
#include <string/string.h>

struct ve_window {
    int width;
    int height;
    string_view_t window_name;
    GLFWwindow* window;
};

const size_t ve_window_sizeof = sizeof(ve_window_t);

static void ve_window_init_window(ve_window_t* window);

void ve_window_init(ve_window_t* window, dim_t dim, string_view_t name) {
    window->width = dim.width;
    window->height = dim.height;
    window->window_name = name;
    ve_window_init_window(window);
}

void ve_window_cleanup(ve_window_t* window) {
    glfwDestroyWindow(window->window);
    glfwTerminate();
}

bool ve_window_should_close(ve_window_t* window) {
    return glfwWindowShouldClose(window->window);
}

inline void ve_window_init_window(ve_window_t* window) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window->window = glfwCreateWindow(window->width, window->height,
                                      window->window_name.data, NULL, NULL);
}

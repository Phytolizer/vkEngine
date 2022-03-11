#include "ve/first_app.h"

#include "ve/window.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>

typedef struct first_app {
    ve_window_t* window;
} first_app_t;

const size_t first_app_sizeof = sizeof(first_app_t);

void first_app_init(first_app_t* app) {
    app->window = malloc(ve_window_sizeof);
    ve_window_init(app->window, (dim_t){FIRST_APP_WIDTH, FIRST_APP_HEIGHT},
                   STRING_VIEW_C("Hello Vulkan!"));
}

void first_app_cleanup(first_app_t* app) {
    ve_window_cleanup(app->window);
    free(app->window);
}

void first_app_run(first_app_t* app) {
    while (!ve_window_should_close(app->window)) {
        glfwPollEvents();
    }
}

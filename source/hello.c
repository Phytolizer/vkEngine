#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

int main(void) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window =
        glfwCreateWindow(800, 600, "Vulkan window", NULL, NULL);

    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);

    mat4 matrix = {0};
    vec4 vec = {0};
    vec4 test;
    glm_mat4_mulv(matrix, vec, test);

    printf("%" PRIu32 " extensions supported\n", extension_count);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

#include "lib.h"

#include <GLFW/glfw3.h>
#include <cblas.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, const char* argv[]) {
	(void)argc;
	(void)argv;
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", NULL, NULL);

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);

	printf("%" PRIu32 " extensions supported\n", extensionCount);

	float matrix[16] = {1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F,
			0.0F, 0.0F, 0.0F, 1.0F};
	float vector[4] = {0.0F, 1.0F, 2.0F, 3.0F};
	float test[4] = {0};
	cblas_sgemv(CblasRowMajor, CblasNoTrans, 4, 4, 1.0F, matrix, 4, vector, 1, 0.0F, test, 1);

	for (uint32_t i = 0; i < 4; i++) {
		printf("%f\n", test[i]);
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

#include "lib.h"

#include "buffer.h"
#include "maybe.h"
#include "span.h"

#include <GLFW/glfw3.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 600

static const char* const VALIDATION_LAYERS[] = {
		"VK_LAYER_KHRONOS_validation",
};

#ifdef NDEBUG
#define ENABLE_VALIDATION_LAYERS false
#else
#define ENABLE_VALIDATION_LAYERS true
#endif

struct HtaPrivate {
	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
};

typedef SPAN_TYPE(const char*) StringSpan;
typedef BUFFER_TYPE(const char*) StringBuffer;
typedef MAYBE_TYPE(uint32_t) MaybeUint32;

typedef struct {
	MaybeUint32 graphicsFamily;
} QueueFamilyIndices;

static bool queueFamilyIndicesIsComplete(QueueFamilyIndices indices) {
	return indices.graphicsFamily.present;
}

static void initWindow(HelloTriangleApplication* app) {
	(void)app;
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	app->private->window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);
}
static const char* validateSupportedExtensions(VkExtensionProperties* extensions,
		uint32_t extensionCount, const char** glfwExtensions, uint32_t glfwExtensionCount) {
	if (extensionCount < glfwExtensionCount) {
		return "Not enough extensions for GLFW";
	}

	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		bool found = false;
		for (uint32_t j = 0; j < extensionCount; j++) {
			if (strcmp(glfwExtensions[i], extensions[j].extensionName) == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			return "Missing required extension for GLFW";
		}
	}
	return NULL;
}
static bool checkValidationLayerSupport(void) {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, NULL);

	VkLayerProperties* availableLayers = malloc(sizeof(VkLayerProperties) * layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

	for (uint32_t i = 0; i < sizeof(VALIDATION_LAYERS) / sizeof(VALIDATION_LAYERS[0]); i++) {
		bool layerFound = false;
		for (uint32_t j = 0; j < layerCount; j++) {
			if (strcmp(VALIDATION_LAYERS[i], availableLayers[j].layerName) == 0) {
				layerFound = true;
				break;
			}
		}
		if (!layerFound) {
			free(availableLayers);
			return false;
		}
	}

	free(availableLayers);
	return true;
}
static StringSpan getRequiredExtensions(void) {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	StringBuffer extensions = BUFFER_INIT;
	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		BUFFER_PUSH(&extensions, glfwExtensions[i]);
	}
	if (ENABLE_VALIDATION_LAYERS) {
		BUFFER_PUSH(&extensions, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return (StringSpan)BUFFER_AS_SPAN(extensions);
}
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	(void)messageSeverity;
	(void)messageType;
	(void)pUserData;
	printf("validation layer: %s\n", pCallbackData->pMessage);
	return VK_FALSE;
}
static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo) {
	*pCreateInfo = (VkDebugUtilsMessengerCreateInfoEXT){0};
	pCreateInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	pCreateInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	pCreateInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	pCreateInfo->pfnUserCallback = debugCallback;
}
static const char* createInstance(HelloTriangleApplication* app) {
	if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()) {
		return "Validation layers requested, but not available!";
	}

	VkApplicationInfo appInfo = {0};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {0};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	StringSpan extensions = getRequiredExtensions();

	createInfo.enabledExtensionCount = (uint32_t)extensions.length;
	createInfo.ppEnabledExtensionNames = extensions.begin;

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {0};
	if (ENABLE_VALIDATION_LAYERS) {
		createInfo.enabledLayerCount = sizeof(VALIDATION_LAYERS) / sizeof(VALIDATION_LAYERS[0]);
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS;
		populateDebugMessengerCreateInfo(&debugCreateInfo);
		createInfo.pNext = &debugCreateInfo;
	} else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = NULL;
	}

	if (vkCreateInstance(&createInfo, NULL, &app->private->instance) != VK_SUCCESS) {
		return "Failed to create instance";
	}

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
	VkExtensionProperties* presentExtensions =
			malloc(sizeof(VkExtensionProperties) * extensionCount);
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, presentExtensions);

	printf("available extensions:\n");
	for (uint32_t i = 0; i < extensionCount; i++) {
		printf("\t%s\n", presentExtensions[i].extensionName);
	}

	const char* validationResult = validateSupportedExtensions(
			presentExtensions, extensionCount, extensions.begin, (uint32_t)extensions.length);
	free(presentExtensions);
	free(extensions.begin);

	return validationResult;
}
static VkResult createDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateIfno,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	PFN_vkCreateDebugUtilsMessengerEXT func =
			(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
					instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != NULL) {
		return func(instance, pCreateIfno, pAllocator, pDebugMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}
static void destroyDebugUtilsMessengerEXT(VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	PFN_vkDestroyDebugUtilsMessengerEXT func =
			(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
					instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != NULL) {
		func(instance, debugMessenger, pAllocator);
	}
}
static const char* setupDebugMessenger(HelloTriangleApplication* app) {
	if (!ENABLE_VALIDATION_LAYERS) {
		return NULL;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(&createInfo);

	if (createDebugUtilsMessengerEXT(
				app->private->instance, &createInfo, NULL, &app->private->debugMessenger)) {
		return "Failed to set up debug messenger!";
	}

	return NULL;
}
static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices = {0};

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

	VkQueueFamilyProperties* queueFamilies =
			malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

	uint32_t i = 0;
	for (size_t j = 0; j < queueFamilyCount; j++) {
		VkQueueFamilyProperties queueFamily = queueFamilies[j];
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = (MaybeUint32)MAYBE_SOME(i);
		}

		if (queueFamilyIndicesIsComplete(indices)) {
			break;
		}

		i++;
	}

	free(queueFamilies);

	return indices;
}
static uint32_t rateDeviceSuitability(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	uint32_t score = 0;

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	score += deviceProperties.limits.maxImageDimension2D;

	if (!deviceFeatures.geometryShader) {
		return 0;
	}

	QueueFamilyIndices indices = findQueueFamilies(device);
	if (!queueFamilyIndicesIsComplete(indices)) {
		return 0;
	}

	return score;
}
static void displayDeviceName(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	printf("Selected physical device: %s\n", deviceProperties.deviceName);
}
static const char* pickPhysicalDevice(HelloTriangleApplication* app) {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(app->private->instance, &deviceCount, NULL);

	if (deviceCount == 0) {
		return "Failed to find GPUs with Vulkan support!";
	}

	VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
	vkEnumeratePhysicalDevices(app->private->instance, &deviceCount, devices);

	uint32_t bestScore = 0;
	VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
	for (uint32_t i = 0; i < deviceCount; i++) {
		uint32_t score = rateDeviceSuitability(devices[i]);
		if (score > bestScore) {
			bestScore = score;
			bestDevice = devices[i];
		}
	}

	free(devices);

	if (bestScore > 0) {
		app->private->physicalDevice = bestDevice;
		displayDeviceName(bestDevice);
	} else {
		return "Failed to find a suitable GPU!";
	}

	return NULL;
}
static const char* createLogicalDevice(HelloTriangleApplication* app) {
	QueueFamilyIndices indices = findQueueFamilies(app->private->physicalDevice);

	VkDeviceQueueCreateInfo queueCreateInfo = {0};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value;
	queueCreateInfo.queueCount = 1;
	float queuePriority = 1.0F;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {0};

	VkDeviceCreateInfo createInfo = {0};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;
	if (ENABLE_VALIDATION_LAYERS) {
		createInfo.enabledLayerCount = sizeof(VALIDATION_LAYERS) / sizeof(VALIDATION_LAYERS[0]);
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS;
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(app->private->physicalDevice, &createInfo, NULL, &app->private->device) !=
			VK_SUCCESS) {
		return "Failed to create logical device!";
	}

	vkGetDeviceQueue(
			app->private->device, indices.graphicsFamily.value, 0, &app->private->graphicsQueue);

	return NULL;
}
static const char* initVulkan(HelloTriangleApplication* app) {
	const char* error = createInstance(app);
	if (error != NULL) {
		return error;
	}
	error = setupDebugMessenger(app);
	if (error != NULL) {
		return error;
	}
	error = pickPhysicalDevice(app);
	if (error != NULL) {
		return error;
	}
	return createLogicalDevice(app);
}
static void mainLoop(HelloTriangleApplication* app) {
	while (!glfwWindowShouldClose(app->private->window)) {
		glfwPollEvents();
	}
}
static void cleanup(HelloTriangleApplication* app) {
	vkDestroyDevice(app->private->device, NULL);
	if (ENABLE_VALIDATION_LAYERS) {
		destroyDebugUtilsMessengerEXT(app->private->instance, app->private->debugMessenger, NULL);
	}
	vkDestroyInstance(app->private->instance, NULL);
	glfwDestroyWindow(app->private->window);
	glfwTerminate();
}

void* MemDup(void* mem, size_t size) {
	void* newMem = malloc(size);
	memcpy(newMem, mem, size);
	return newMem;
}

HelloTriangleApplication HtaNew(void) {
	HelloTriangleApplication app;
	app.private = malloc(sizeof(struct HtaPrivate));
	app.private->physicalDevice = VK_NULL_HANDLE;
	return app;
}

const char* HtaRun(HelloTriangleApplication* app) {
	const char* result = NULL;
	initWindow(app);
	result = initVulkan(app);
	if (result != NULL) {
		return result;
	}
	mainLoop(app);
	cleanup(app);
	return result;
}

void HtaFree(HelloTriangleApplication* app) {
	free(app->private);
}

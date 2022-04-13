#pragma once

#include <stdbool.h>

typedef struct HtaPrivate HtaPrivate;

typedef struct {
	HtaPrivate* private;
} HelloTriangleApplication;

HelloTriangleApplication HtaNew(void);
const char* HtaRun(HelloTriangleApplication* app);
void HtaFree(HelloTriangleApplication* app);

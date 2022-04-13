#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct HtaPrivate HtaPrivate;

typedef struct {
	HtaPrivate* private;
} HelloTriangleApplication;

void* MemDup(void* mem, size_t size);

HelloTriangleApplication HtaNew(void);
const char* HtaRun(HelloTriangleApplication* app);
void HtaFree(HelloTriangleApplication* app);

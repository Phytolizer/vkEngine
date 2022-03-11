#pragma once

#include "ve/window.h"

#include <stddef.h>

#define FIRST_APP_WIDTH 800
#define FIRST_APP_HEIGHT 600

struct first_app;
typedef struct first_app first_app_t;

extern const size_t first_app_sizeof;

void first_app_init(first_app_t* app);
void first_app_cleanup(first_app_t* app);

void first_app_run(first_app_t* app);

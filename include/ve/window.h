#pragma once

#include "ve/dim.h"

#include <stdbool.h>
#include <stddef.h>
#include <string/string.h>

struct ve_window;
typedef struct ve_window ve_window_t;

extern const size_t ve_window_sizeof;

void ve_window_init(ve_window_t* window, dim_t dim, string_view_t name);
void ve_window_cleanup(ve_window_t* window);

bool ve_window_should_close(ve_window_t* window);

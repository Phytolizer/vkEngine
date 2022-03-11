#pragma once

#include <stddef.h>

typedef struct {
    char* data;
    size_t length;
    size_t capacity;
} dynamic_string_t;

typedef struct {
    char* data;
    size_t length;
} string_t;

typedef struct {
    const char* data;
    size_t length;
} string_view_t;

#define STRING_VIEW_C(str)                                                     \
    ((string_view_t){.data = (str), .length = sizeof(str) - 1})

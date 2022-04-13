#pragma once

#include <stdbool.h>

#define MAYBE_TYPE(T) \
	struct { \
		bool present; \
		T value; \
	}

#define MAYBE_SOME(v) \
	{ .present = true, .value = (v) }

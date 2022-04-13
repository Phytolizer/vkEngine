#include "lib.h"

#include <hedley.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct library create_library() {
	struct library lib;
	size_t name_size = sizeof "vkEngine" + 1;
	char* name = malloc(name_size);
	if (name == NULL) {
		goto exit;
	}

	(void)memcpy(name, "vkEngine", name_size);

exit:
	lib.name = name;
	return lib;
}

void destroy_library(struct library* lib) {
	free(HEDLEY_CONST_CAST(void*, lib->name));
}

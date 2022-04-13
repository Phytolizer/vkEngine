#include "lib.h"

#include <hedley.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

Library CreateLibrary() {
	Library lib;
	size_t nameSize = sizeof "vkEngine" + 1;
	char* name = malloc(nameSize);
	if (name == NULL) {
		goto exit;
	}

	(void)memcpy(name, "vkEngine", nameSize);

exit:
	lib.name = name;
	return lib;
}

void DestroyLibrary(Library* lib) {
	free(HEDLEY_CONST_CAST(void*, lib->name));
}

#include "lib.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;
	HelloTriangleApplication app = HtaNew();
	const char* error = HtaRun(&app);
	if (error) {
		fprintf(stderr, "ERROR: %s\n", error);
		HtaFree(&app);
		return EXIT_FAILURE;
	}
	HtaFree(&app);
	return EXIT_SUCCESS;
}

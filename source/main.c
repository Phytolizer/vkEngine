#include <stddef.h>
#include <stdio.h>

#include "lib.h"

int main(int argc, const char* argv[])
{
  struct library lib = create_library();

  (void)argc;
  (void)argv;

  if (lib.name == NULL) {
    (void)puts("Hello from unknown! (JSON parsing failed in library)");
  } else {
    (void)printf("Hello from %s!", lib.name);
  }
  destroy_library(&lib);
  return 0;
}

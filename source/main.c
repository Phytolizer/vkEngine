#include <stdlib.h>
#include <ve/first_app.h>

int main(void) {
    first_app_t* app = malloc(first_app_sizeof);
    first_app_init(app);

    first_app_run(app);

    first_app_cleanup(app);
    free(app);
}

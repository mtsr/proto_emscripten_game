#include "App.h"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#else
#endif

App app;

static void update() {
    app.update();
}

int main(int argc, char *argv[]) {
    app.init();
    
#ifdef EMSCRIPTEN
    emscripten_set_main_loop(update, 0, 1);
#else
    while (!app.done) {
        update();
    }
#endif
}
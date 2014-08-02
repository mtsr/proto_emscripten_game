#include "App.h"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#else
#endif

int main(int argc, char *argv[]) {
    App app;
    
    app.init();
    app.run();
}

#include <iostream>

#include "core/renderer.hpp"

int main() {
    Renderer randey(1280, 720);
    if(!randey.init()) {
        return -1;
    }
    randey.start();
    return 0;
}

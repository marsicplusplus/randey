#include <iostream>

#include "core/renderer.hpp"

int main() {
    Renderer randey(800, 600);
    if(!randey.init()) {
        return -1;
    }
    randey.start();
    return 0;
}

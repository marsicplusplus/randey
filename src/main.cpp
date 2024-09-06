#include <iostream>

#include "core/renderer.hpp"
#include "core/utils.hpp"

int main() {
    Renderer randey(1280, 720);
    if(!randey.init()) {
        return -1;
    }
    randey.start();
    return 0;
}

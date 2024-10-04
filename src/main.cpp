#include <iostream>

#include "core/renderer.hpp"
#include "core/utils.hpp"

int main(int argc, char* args[]) {
    if(argc < 2) {
        std::cerr << "No scene file was specified." << std::endl;
        return 1;
    } 
    Renderer randey(1280, 720);
    try {
        randey.init(args[1]);
        randey.start();
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
    }
    return 0;
}

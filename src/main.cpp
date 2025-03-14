#include "Application.h"

int main() {

    try {
        Kent::Application app;
        app.mainLoop();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    std::cout<<"\nend successfully";
    return 0;
}
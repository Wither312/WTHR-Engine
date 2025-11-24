#include "RuntimeApp.hpp"

int main(int argc, char** argv) {
    std::string sceneFile = argc > 1 ? argv[1] : "DefaultScene.json";
    RuntimeApp app(sceneFile);
    app.Run();
    return 0;
}

#include <Application.hpp>

int main()
{
    Application app(1280, 720, "WTHR Editor");
    if (!app.Init())
        return -1;

    app.Run();
    return 0;
}
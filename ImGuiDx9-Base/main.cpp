#include "app.h"
#include <wtypes.h>

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    App app;
    return app.Run();
}
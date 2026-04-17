#include "../includes/App.hpp"
#include <iostream>

int main()
{
    App app;

    if (!app.init())
    {
        std::cerr << "Initialization failed" << std::endl;
        return 1;
    }

    app.run();
    return 0;
}
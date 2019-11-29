#include <iostream>

#include "core/AppCore.hpp"

int main(int argc, char const *argv[])
{
    devtest::AppCore app(argc, argv);
    return app.exec();
}

#include "app.h"
#include "log.h"

using namespace glamorous;

int main(int argc, char const *argv[]) {
    Log::info("hello");
    GlamorousApp app;
    app.initialize("/home/lljbash/data");
    std::string name;
    name = app.transfer(argv[1], atoi(argv[2]), argv[3]);
    Log::info("Filename: %s", name.c_str());
    return 0;
}


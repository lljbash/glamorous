#include "app.h"
#include "log.h"

using namespace glamorous;

int main(int argc, char const *argv[]) {
    Log::info("hello");
    GlamorousApp app;
    app.initialize("/home/lljbash/data");
    std::string name;
    name = app.transfer("grass", 2);
    Log::info("Filename: %s", name.c_str());
    name = app.transfer("goldfish", 4);
    Log::info("Filename: %s", name.c_str());
    return 0;
}


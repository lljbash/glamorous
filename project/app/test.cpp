#include "log.h"
#include "component/component_factory.h"

using namespace glamorous;

int main(int argc, char const *argv[]) {
    Log::info("hello");
    SimpleComponentFactory scf;
    ComponentPointer cp = scf.create();
    RequestStatusPointer rs = std::make_shared<RequestStatus>();
    cp->push_request(rs);
    cp->push_request(nullptr);
    cp->start();
    return 0;
}

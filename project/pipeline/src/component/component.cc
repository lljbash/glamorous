#include "component/component.h"
#include "log.h"

using namespace glamorous;

Component::Component(std::string name) : name_(name) {
    queue_ = std::make_shared<WaitQueue>();
    next_ = [](RequestStatusPointer) -> ComponentPointer { return nullptr; };
    Log::info("Component %s created.", name_.c_str());
}

void Component::start(WaitQueuePointer output_queue) {
    RequestStatusPointer request;
    Log::info("Component %s started.", name_.c_str());
    while (true) {
        queue_->wait_and_pop(request);
        if (request == nullptr) {
            break;
        }
        process(request);
        ComponentPointer next = next_(request);
        if (next != nullptr) {
            next->push_request(request);
        }
        if (output_queue != nullptr) {
            output_queue->push(request);
        }
    }
    Log::info("Component %s stopped.", name_.c_str());
}

void Component::push_request(RequestStatusPointer request) {
    queue_->push(request);
}

void Component::set_next_component(ComponentPointer component) {
    next_ = [component](RequestStatusPointer) { return component; };
}

void Component::set_next_component_func(const NextFunc &func) {
    next_ = func;
}

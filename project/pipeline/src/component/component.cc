#include "component/component.h"

using namespace glamorous;

Component::Component() {
    queue_ = std::make_shared<WaitQueue>();
    next_ = [](RequestStatusPointer) -> ComponentPointer { return nullptr; };
}

void Component::start(WaitQueuePointer output_queue) {
    RequestStatusPointer request;
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
        if (output_queue) {
            output_queue->push(request);
        }
    }
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

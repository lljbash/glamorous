#include "component/component.h"
#include "log.h"

using namespace glamorous;

Component::Component(std::string name) : name_(name) {
    queue_ = std::make_shared<WaitQueue>();
    next_ = [](RequestStatusPointer) -> ComponentPointer { return nullptr; };
    Log::info("Component %s created.", name_.c_str());
}

std::string Component::get_name() const {
    return name_;
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
            if (std::find(all_next_.begin(), all_next_.end(), next) != all_next_.end()) {
                next->push_request(request);
            }
            else {
                Log::error("Component %s has invalid output component %s",
                           name_.c_str(), next->name_.c_str());
            }
        }
        if (output_queue != nullptr) {
            output_queue->push(request);
        }
    }
    for (ComponentPointer next : all_next_) {
        next->push_request(nullptr);
    }
    Log::info("Component %s stopped.", name_.c_str());
}

void Component::push_request(RequestStatusPointer request) {
    queue_->push(request);
}

void Component::set_next_component(ComponentPointer component) {
    next_ = [component](RequestStatusPointer) { return component; };
    all_next_ = {component};
}

void Component::set_next_component_func(const NextFunc &func) {
    next_ = func;
}

void Component::set_all_next_component(std::vector<ComponentPointer> all_next) {
    all_next_ = move(all_next);
}

#include "pipeline.h"
#include "log.h"

using namespace glamorous;

void Pipeline::set_static_pipeline(std::vector<ComponentPointer> component_list) {
    component_list_ = move(component_list);
    for (size_t i = 0; i < component_list_.size() - 1; ++i) {
        component_list_[i]->set_next_component(component_list_[i+1]);
    }
}

void Pipeline::set_custom_pipeline(std::vector<ComponentPointer> component_list) {
    component_list_ = move(component_list);
}

void Pipeline::start_serial(WaitQueuePointer output,
                            std::vector<RequestStatusPointer> requests) {
    if (component_list_.empty()) {
        Log::error("Empty pipeline!");
        return;
    }
    for (RequestStatusPointer request : requests) {
        component_list_[0]->push_request(request);
    }
    component_list_[0]->push_request(nullptr);
    Log::info("Start serial.");
    while (true) {
        bool all_free = true;
        for (size_t i = 0; i < component_list_.size(); ++i) {
            const ComponentPointer &cp = component_list_[i];
            if (!cp->is_free()) {
                all_free = false;
                cp->start(i == component_list_.size() - 1 ? output : nullptr);
            }
        }
        if (all_free) {
            break;
        }
    }
    Log::info("Serial finished.");
}

void Pipeline::start_parallel(WaitQueuePointer output) {
    if (component_list_.empty()) {
        Log::error("Empty pipeline!");
        return;
    }
    threads_.clear();
    Log::info("Start parallel.");
    for (size_t i = 0; i < component_list_.size(); ++i) {
        WaitQueuePointer q = i == component_list_.size() - 1 ? output : nullptr;
        std::thread t = component_list_[i]->spawn(q);
        t.detach();
        threads_.push_back(move(t));
    }
}

void Pipeline::new_request(RequestStatusPointer request) {
    if (component_list_.empty()) {
        Log::error("Empty pipeline!");
        return;
    }
    component_list_[0]->push_request(request);
}

void Pipeline::stop_parallel() {
    if (threads_.empty()) {
        Log::error("Pipeline not running in parallel!");
        return;
    }
    Log::info("Stopping parallel.");
    new_request(nullptr);
}

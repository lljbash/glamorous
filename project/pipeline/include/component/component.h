#pragma once

#include <functional>
#include <string>
#include "threadsafe_queue.h"
#include "struct/request_status.h"

namespace glamorous {

using ComponentPointer = std::shared_ptr<class Component>;

class Component {
public:
    using WaitQueue = threadsafe_queue<RequestStatusPointer>;
    using WaitQueuePointer = std::shared_ptr<WaitQueue>;
    using NextFunc = std::function<ComponentPointer(RequestStatusPointer)>;
    
    Component(std::string name = "Component");
    Component(const Component &) = delete;
    Component &operator=(const Component &) = delete;
    virtual ~Component() = default;
    
    void start(WaitQueuePointer output_queue = nullptr);
    void push_request(RequestStatusPointer request);
    
    void set_next_component(ComponentPointer component);
    void set_next_component_func(const NextFunc &func);
    
protected:
    virtual void process(RequestStatusPointer request) = 0;

private:
    std::string name_;
    
    WaitQueuePointer queue_;
    NextFunc next_;
};

} // namespace glamorous

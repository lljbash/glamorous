#pragma once

#include "component/component.h"

namespace glamorous {

class Pipeline {
public:
    using WaitQueuePointer = Component::WaitQueuePointer;
    
    // Static Pipeline:
    // System will link components in order for you.
    void set_static_pipeline(std::vector<ComponentPointer> component_list);
    
    // Custom (Dynamic) Pipeline:
    // You should link components yourself.
    // The first component will be the main entrance.
    // The last component will be the output component.
    void set_custom_pipeline(std::vector<ComponentPointer> component_list);
    
    void start_serial(WaitQueuePointer output,
                      std::vector<RequestStatusPointer> requests);
    
    void start_parallel(WaitQueuePointer output);
    void new_request(RequestStatusPointer request);
    void stop_parallel(); // try to stop, but finish current jobs

private:
    std::vector<ComponentPointer> component_list_;
    std::vector<std::thread> threads_;
};

} // namespace glamorous


#include "EventProcessor.hpp"
#include <iostream>

void process_events(ThreadSafeQueue<Event>& queue) {
    while (true) {
        Event e = queue.pop();
        std::cout << "[EVENT] " << e.symbol << " - Price: $" << e.price << " @ " << e.timestamp << std::endl;
    }
}


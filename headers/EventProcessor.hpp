
#ifndef EVENTPROCESSOR_HPP
#define EVENTPROCESSOR_HPP

#include "Event.hpp"
#include "ThreadSafeQueue.hpp"

void process_events(ThreadSafeQueue<Event>& queue);

#endif



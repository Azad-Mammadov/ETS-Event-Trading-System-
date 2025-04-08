
// event.hpp
#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>

enum class EventType {
    MARKET,
    ORDER,
    FILL
};

struct Event {
    EventType type;
    std::string content;

    Event(EventType t, const std::string& c) : type(t), content(c) {}
};

#endif // EVENT_HPP
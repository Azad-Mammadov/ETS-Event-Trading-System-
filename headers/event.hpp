// event.hpp
#ifndef EVENT_HPP
#define EVENT_HPP

#include <string> // Provides the std::string class for handling text data

// Enum to represent different types of events
enum class EventType {
    MARKET, // Represents a market data event
    ORDER,  // Represents an order event
    FILL    // Represents a fill event (order execution)
};

// Struct to represent an event with a type and content
struct Event {
    EventType type;         // The type of the event
    std::string content;    // The content or details of the event

    // Constructor to initialize an event with a type and content
    Event(EventType t, const std::string& c) : type(t), content(c) {}
};

#endif // EVENT_HPP
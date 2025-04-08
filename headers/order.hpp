
#ifndef ORDER_HPP
#define ORDER_HPP

#include "event.hpp"
#include <string>

class OrderExecution {
public:
    void execute_order(const Event& order_event);
};

#endif // ORDER_HPP


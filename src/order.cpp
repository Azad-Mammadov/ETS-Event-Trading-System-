#include "order.hpp"  // Declares the OrderExecution class and its methods for handling order execution
#include <iostream>    // Provides standard input/output operations (e.g., std::cout for logging)

void OrderExecution::execute_order(const Event& order_event) {
    std::cout << "Executing Order: " << order_event.symbol
              << " at price: " << order_event.price << std::endl;
}

// This function simulates order execution logic. In a real-world scenario, this would involve
// sending the order to an exchange or broker and handling the response.

#include "risk.hpp"
#include <iostream>

bool RiskManager::validate_order(const Event& order_event) {
    if (order_event.price > 0) {
        return true;
    }
    std::cerr << "Risk check failed: Invalid price." << std::endl;
    return false;
}


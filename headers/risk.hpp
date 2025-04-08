
#ifndef RISK_HPP
#define RISK_HPP

#include "event.hpp"

class RiskManager {
public:
    bool validate_order(const Event& order_event);
};

#endif // RISK_HPP


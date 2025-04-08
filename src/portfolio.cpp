
#include "portfolio.hpp"

void PortfolioManager::update_position(const std::string& symbol, int quantity, double price) {
    std::lock_guard<std::mutex> lock(mtx);
    positions[symbol] += quantity;
    last_price[symbol] = price;
}

int PortfolioManager::get_position(const std::string& symbol) const {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = positions.find(symbol);
    return (it != positions.end()) ? it->second : 0;
}

double PortfolioManager::get_value() const {
    std::lock_guard<std::mutex> lock(mtx);
    double total_value = 0.0;
    for (const auto& [symbol, qty] : positions) {
        auto price_it = last_price.find(symbol);
        if (price_it != last_price.end()) {
            total_value += qty * price_it->second;
        }
    }
    return total_value;
}


// This code defines a PortfolioManager class that manages a portfolio of positions. It allows updating positions, retrieving the current position for a specific symbol, and calculating the total value of the portfolio based on the last known prices.
// The class uses a mutex to ensure thread safety when accessing or modifying the portfolio data.
// The update_position method updates the quantity of a symbol in the portfolio and stores the last known price. The get_position method retrieves the current quantity for a specific symbol, and the get_value method calculates the total value of the portfolio based on the last known prices.


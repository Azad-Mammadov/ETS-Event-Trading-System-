#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include <string>
#include <unordered_map>
#include <mutex>

class PortfolioManager {
public:
    void update_position(const std::string& symbol, int quantity, double price);
    int get_position(const std::string& symbol) const;
    double get_value() const;

private:
    mutable std::mutex mtx;
    std::unordered_map<std::string, int> positions;
    std::unordered_map<std::string, double> last_price;
};

#endif // PORTFOLIO_HPP

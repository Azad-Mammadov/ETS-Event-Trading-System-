
#ifndef MARKETDATAFETCHER_HPP
#define MARKETDATAFETCHER_HPP

#include <string>
#include "Event.hpp"
#include "ThreadSafeQueue.hpp"

void fetch_market_data(const std::string& symbol, const std::string& api_key, ThreadSafeQueue<Event>& queue);

#endif



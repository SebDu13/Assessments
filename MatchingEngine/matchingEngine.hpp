#pragma once

#include<map>
#include "orderBook.hpp"
#include "trade.hpp"

namespace MatchingEngine 
{

class MatchingEngine
{
public:
    std::vector<Trade> processOrder(const Order& order);
    std::vector<std::string> dumpOrderBooks() const;
    
private:
    std::map<std::string, OrderBook> _orderBooks;
};
}
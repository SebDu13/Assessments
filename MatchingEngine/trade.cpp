#include "trade.hpp"
#include <sstream>

namespace MatchingEngine 
{
    
Trade::Trade(std::string symbol, double price, size_t quantity, size_t id1, size_t id2)
: symbol(std::move(symbol))
, price(price)
, quantity(quantity)
, id1(id1)
, id2(id2)
{
}

std::string Trade::toStr() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Trade& trade)
{
    os << trade.symbol << "," << trade.price << "," << trade.quantity << "," << trade.id1 << "," << trade.id2;
    return os;
}

}
#pragma once
#include <string>

namespace MatchingEngine 
{

struct Trade
{
    Trade(std::string symbol, double price, size_t quantity, size_t id1, size_t id2);
    
    std::string symbol;
    double price;
    size_t quantity;
    size_t id1;
    size_t id2;
    
    std::string toStr() const;
    
    friend std::ostream& operator<<(std::ostream& os, const Trade& trade);
};

}
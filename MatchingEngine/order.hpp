#pragma once

#include <string>
#include <optional> 
#include <vector>

namespace MatchingEngine 
{
namespace Conf
{
    namespace INSERT_INDEX
    {
        enum
        {
            TYPE = 0,
            ID,
            SYMBOL,
            SIDE,
            PRICE,
            QUANTITY,
            INSERT_SIZE
        };
    }
    
    namespace PULL_INDEX
    {
        enum 
        {
            TYPE = 0,
            ID,
            PULL_SIZE
        };
    }
    
    namespace AMEND_INDEX
    {
    enum
        {
            TYPE = 0,
            ID,
            PRICE,
            QUANTITY,
            AMEND_SIZE
        };
    }
};

struct Order
{
    enum class Type { INSERT, AMEND, PULL};
    enum class Side {BUY, SELL};
    
    Order() = default;
    Order(size_t id, Type type, Side side, double price, size_t quantity, std::string symbol);
    size_t id = 0;
    Type type = Type::INSERT;
    Side side = Side::SELL;
    double price = 0; // TODO Should actually be a fixed point type instead of double
    size_t quantity = 0;
    std::string symbol = "";
    
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
};

std::string toStr(Order::Type type);
std::string toStr(Order::Side side);
std::optional<Order> makeOrder(const std::string& input);
std::vector<Order> makeOrders(const std::vector<std::string>& inputs);

}




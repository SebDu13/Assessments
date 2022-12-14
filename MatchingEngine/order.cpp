#include <boost/algorithm/string.hpp>
#include <iostream>
#include "order.hpp"


namespace MatchingEngine 
{
    
Order::Order(size_t id, Type type, Side side, double price, size_t quantity, std::string symbol)
:id(id)
, type(type)
, side(side)
, price(price)
, quantity(quantity)
, symbol(std::move(symbol))
{
}
    
std::vector<Order> makeOrders(const std::vector<std::string>& inputs)
{
    std::vector<Order> orders;
    orders.reserve(inputs.size());
    
    for(const auto& input : inputs)
    {
        if(auto order = makeOrder(input))
            orders.emplace_back(*order);
        else 
        {
            std::cout << "ERROR order ill-formated: " << input << std::endl;
        }
    }
    
    return orders;
}

std::optional<Order> makeOrder(const std::string& input)
{
    std::vector<std::string> orderFields;
    boost::split(orderFields, input, boost::is_any_of(","));
    
    if(orderFields[Conf::PULL_INDEX::TYPE] == "PULL" && orderFields.size() == Conf::PULL_INDEX::PULL_SIZE)
    {
        Order order;
        order.type =  Order::Type::PULL;
        order.id = stoi(orderFields[Conf::PULL_INDEX::ID]);
        return order;
        /*return {
            .type = Order::Type::PULL,
            .id = stoi(orderFields[1])
        };*/
    }
    
    if(orderFields[Conf::AMEND_INDEX::TYPE] == "AMEND" && orderFields.size() == Conf::AMEND_INDEX::AMEND_SIZE)
    {
        Order order;
        order.type =  Order::Type::AMEND;
        order.id = stoi(orderFields[Conf::AMEND_INDEX::ID]);
        order.price = std::stod(orderFields[Conf::AMEND_INDEX::PRICE]);
        order.quantity = stoi(orderFields[Conf::AMEND_INDEX::QUANTITY]);
        return order;
    }
    
    if(orderFields[Conf::INSERT_INDEX::TYPE] == "INSERT" && orderFields.size() == Conf::INSERT_INDEX::INSERT_SIZE)
    {
        Order order;
        order.type =  Order::Type::INSERT;
        order.id = stoi(orderFields[Conf::INSERT_INDEX::ID]);
        order.symbol = orderFields[Conf::INSERT_INDEX::SYMBOL];
        
        if(orderFields[Conf::INSERT_INDEX::SIDE] == "BUY")
            order.side = Order::Side::BUY;
        else if(orderFields[Conf::INSERT_INDEX::SIDE] == "SELL")
            order.side = Order::Side::SELL;
        else 
            return std::nullopt;
        
        order.price = std::stod(orderFields[Conf::INSERT_INDEX::PRICE]);
        order.quantity = stoi(orderFields[Conf::INSERT_INDEX::QUANTITY]);
        return order;
    }
    
    return std::nullopt;
}

std::string toStr(Order::Type type)
{
    switch(type)
    {
        case Order::Type::INSERT :
            return "INSERT";
        case Order::Type::AMEND : 
            return "AMEND";
        case Order::Type::PULL : 
            return "PULL";
    }
    return "";
}

std::string toStr(Order::Side side)
{
    switch(side)
    {
        case Order::Side::BUY :
            return "BUY";
        case Order::Side::SELL : 
            return "SELL";
    }
    return "";
};

std::ostream& operator<<(std::ostream& os, const Order& order)
{
    if(order.type == Order::Type::INSERT)
        os << toStr(order.type) << "," << order.id << "," << order.symbol << "," << toStr(order.side) << "," << order.price << "," << order.quantity;
    else if(order.type == Order::Type::PULL)
        os << toStr(order.type) << "," << order.id;
    else if(order.type == Order::Type::AMEND)
        os << toStr(order.type) << "," << order.id << "," << order.price << "," << order.quantity;
        
    return os;
}

}


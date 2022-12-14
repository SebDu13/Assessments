#pragma once

#include<map>
#include "order.hpp"

namespace MatchingEngine 
{

class OrderBook
{
public:
    struct OrderInfo
    {
        size_t id;
        size_t quantity;
    };
    
    using BidMapType = std::multimap<double, OrderInfo, std::greater<double>>;
    using AskMapType = std::multimap<double, OrderInfo>;

    // I would add registry which contains id and iterator in order to find order in orders book in O(1) complexity 
    // Each time we add an order in the orderBook we will have to update the registry
    // Note:
        /*Iterator validity
        Iterators, pointers and references referring to elements removed by the function are invalidated.
        All other iterators, pointers and references keep their validity.*/
    //unordered_map<id, std::pair<iterator, Side>> idRegistry
    
    bool eraseOrder(size_t id);
    std::pair<bool, std::optional<MatchingEngine::Order>> amendOrder(size_t id, double price, size_t quantity, const std::string& symbol);
    std::vector<std::string> dump() const;
    
    BidMapType& getBidOrders();
    AskMapType& getAskOrders();
    
private:
    BidMapType _bidOrders; // buy 
    AskMapType _askOrders; // sell
};
}
#include <iostream>
#include <utility>
#include "matchingEngine.hpp"

namespace helper
{
std::vector<MatchingEngine::Trade> matchOrder(const MatchingEngine::Order& order, auto& orderBook, auto& otherSideOrderBook)
{
    std::vector<MatchingEngine::Trade> trades;
    auto itUp = otherSideOrderBook.upper_bound(order.price);
    size_t remainingQuantity = order.quantity;
    
    using It = std::remove_reference<decltype(otherSideOrderBook)>::type::iterator;
    std::vector<It> toErase;
    
    for(auto it = otherSideOrderBook.begin(); it != itUp && remainingQuantity != 0 ; ++it)
    {
        auto& [price, orderInfo] = *it;
        //std::cout << "matchOrder: price,qty found in order book, " << order.symbol << " id=" << orderInfo.id << " price:" << price << " quantity:" <<  orderInfo.quantity << std::endl;
        
        if(orderInfo.quantity > remainingQuantity)
        {
            trades.emplace_back(order.symbol, price, remainingQuantity, order.id, orderInfo.id);
            orderInfo.quantity -= remainingQuantity;
            remainingQuantity = 0;
        }
        else
        {
            trades.emplace_back(order.symbol, price, orderInfo.quantity, order.id, orderInfo.id);
            remainingQuantity -= orderInfo.quantity;
            toErase.emplace_back(it);
        }
    }
    
    for(auto& it : toErase)
    {
        otherSideOrderBook.erase(it);
    }
    
    if(remainingQuantity != 0)
    {
        orderBook.insert( {order.price, {order.id, remainingQuantity}} );
    }
    
    /*std::cout << "matchOrder orderbook:" << std::endl;
    for(auto& [price, orderInfo] : orderBook)
        std::cout << "price:" << price << " id=" << orderInfo.id << " quantity=" << orderInfo.quantity << std::endl;
        
    std::cout << "matchOrder other orderbook:" << std::endl;
    for(auto& [price, orderInfo] : otherSideOrderBook)
        std::cout << "price:" << price << " id=" << orderInfo.id << " quantity=" << orderInfo.quantity << std::endl;*/
    
    return trades;
}
}

namespace MatchingEngine 
{

std::vector<Trade> MatchingEngine::processOrder(const Order& order)
{
    //std::cout << "MatchingEngine::processOrder: " << order << std::endl;
    switch(order.type)
    {
        case Order::Type::INSERT:
        {
            OrderBook& orderBook = _orderBooks[order.symbol];
            if(order.side == Order::Side::BUY)
                return helper::matchOrder(order, orderBook.getBidOrders(), orderBook.getAskOrders());
            else if(order.side == Order::Side::SELL)
                return helper::matchOrder(order, orderBook.getAskOrders(), orderBook.getBidOrders());
        } break;

        case Order::Type::PULL:
        {
            for(auto& [symbol, orderBook] : _orderBooks)
            {
                if(orderBook.eraseOrder(order.id))
                    return {};
            }
        }break;
        
        case Order::Type::AMEND:
        {
            for(auto& [symbol, orderBook] : _orderBooks)
            {
                if(const auto& [isAmended, newOrder] = orderBook.amendOrder(order.id, order.price, order.quantity, symbol); isAmended)
                {
                    if(newOrder)
                    {
                        return processOrder(*newOrder);
                    }
                    return {};
                }
            }
        }break;
    }
    
    return {};
}

std::vector<std::string> MatchingEngine::dumpOrderBooks() const
{
    // I kept a map and not an unordered_map to dump in alphabetic order 
    std::vector<std::string> result;
    for(const auto& [symbol, orderBook] : _orderBooks)
    {
        result.emplace_back("===" + symbol + "===");
        std::vector<std::string> lines = orderBook.dump();
        result.insert(std::end(result), std::begin(lines), std::end(lines));
    }
    
    return result;
}

}
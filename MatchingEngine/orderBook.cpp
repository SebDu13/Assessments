#include "orderBook.hpp"
#include <sstream>
#include <iostream>

namespace helper
{
    
std::vector<std::pair<double, size_t>> merge(const auto& multiMap)
{
    std::vector<std::pair<double, size_t>> result;
    
    for(const auto& [price, orderInfo] : multiMap)
    {
        if(result.empty() || result.back().first != price)
            result.emplace_back(price, orderInfo.quantity);
        else
            result.back().second += orderInfo.quantity;
    }
    return result;
}

auto findIt(auto& multiMap, size_t id)
{
    return std::find_if(
          multiMap.begin(),
          multiMap.end(),
          [id](const auto& it) {return it.second.id == id; });
}

template<typename MultiMap>
MatchingEngine::Order::Side getSideTraits()
{
    if constexpr(std::is_same_v<MultiMap, MatchingEngine::OrderBook::BidMapType>)
        return MatchingEngine::Order::Side::BUY;
    return MatchingEngine::Order::Side::SELL;
}

bool erase(auto& multiMap, size_t id)
{
    if(auto it = findIt(multiMap,id); it != multiMap.end())
    {
        multiMap.erase(it);
        return true;
    }
    return false;
}

std::pair<bool, std::optional<MatchingEngine::Order>> amend(auto& multiMap, size_t id, double price, size_t quantity, const std::string& symbol)
{
    if(auto it = findIt(multiMap,id); it != multiMap.end())
    {
        if(price != it->first)
        {
            erase(multiMap, id);
            using MultiMapType = std::remove_reference<decltype(multiMap)>::type;
            return {true, MatchingEngine::Order(id, MatchingEngine::Order::Type::INSERT, getSideTraits<MultiMapType>() , price, quantity, symbol)};
        }

        if(quantity < it->second.quantity)
        {
            it->second.quantity = quantity;
            return {true, std::nullopt};
        }
        
        // erase and insert to lose the time priority
        erase(multiMap, id);
        multiMap.insert( {price, {id, quantity}} );
        return {true, std::nullopt};
    }
    return {false, std::nullopt};
}

}

namespace MatchingEngine 
{
    
OrderBook::BidMapType& OrderBook::getBidOrders()
{
    return _bidOrders;
}

OrderBook::AskMapType& OrderBook::getAskOrders()
{
    return _askOrders;
}

std::vector<std::string> OrderBook::dump() const
{
    std::vector<std::string> result;
    
    /*std::cout << "OrderBook::dump(): bidOrders" << std::endl;
    for(auto& [price, orderInfo] : bidOrders)
        std::cout << "price:" << price << " id=" << orderInfo.id << " quantity=" << orderInfo.quantity << std::endl;
        
    std::cout << "OrderBook::dump(): askOrders" << std::endl;
    for(auto& [price, orderInfo] : askOrders)
        std::cout << "price:" << price << " id=" << orderInfo.id << " quantity=" << orderInfo.quantity << std::endl;*/
    
    auto mergedBid = helper::merge(_bidOrders);
    auto mergedAsk = helper::merge(_askOrders);
    
    /*std::cout << "OrderBook::dump(): mergedBid" << std::endl;
    for(auto& [price, quantity] : mergedBid)
        std::cout << "price:" << price << " quantity=" << quantity << std::endl;
        
    std::cout << "OrderBook::dump(): mergedAsk" << std::endl;
    for(auto& [price, quantity] : mergedAsk)
        std::cout << "price:" << price << " quantity=" << quantity << std::endl;*/
    
    size_t maxSize = std::max(mergedBid.size(), mergedAsk.size());
    result.reserve(maxSize);
    
    for(int i = 0; i < maxSize; ++i)
    {
        std::stringstream ss;
        if(i < mergedBid.size())
            ss << mergedBid[i].first << "," <<  mergedBid[i].second;
        else
            ss << ",";
        
        ss << ",";
        
        if(i < mergedAsk.size())
            ss << mergedAsk[i].first << "," <<  mergedAsk[i].second;
        else
            ss << ",";
        
        result.emplace_back(ss.str());
    }
    
    return result;
}

bool OrderBook::eraseOrder(size_t id)
{
    if(helper::erase(_bidOrders, id))
        return true;
    return helper::erase(_askOrders, id);
}

std::pair<bool, std::optional<MatchingEngine::Order>> OrderBook::amendOrder(size_t id, double price, size_t quantity, const std::string& symbol)
{
    if(auto amendPair = helper::amend(_bidOrders, id, price, quantity, symbol); amendPair.first)
        return amendPair;
    return helper::amend(_askOrders, id, price, quantity, symbol);
}

}
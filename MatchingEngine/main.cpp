#include <iostream>
#include "main.hpp"
#include "order.hpp"
#include "matchingEngine.hpp"

std::vector<std::string> run(std::vector<std::string> const& input) 
{
    //std::cout << std::endl << "**** STARTING ****" << std::endl;

    // Here we could reserve result with size = input.size()
    std::vector<std::string> result;
    MatchingEngine::MatchingEngine matchingEngine;
    
    for(const auto& order : MatchingEngine::makeOrders(input))
    { 
        for(const auto& trade : matchingEngine.processOrder(order))
        {
            result.emplace_back(trade.toStr());
        }
    }
    
    auto orderBooksDump = matchingEngine.dumpOrderBooks();
    result.insert(std::end(result), std::begin(orderBooksDump), std::end(orderBooksDump));
    
    /*std::cout << "**** Result: ****" << std::endl;
    for(auto& item : result)
        std::cout << item << std::endl;*/
       
    return result;
}

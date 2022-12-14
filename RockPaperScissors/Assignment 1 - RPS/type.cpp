#include <sstream>

#include "type.hpp"


namespace Game
{
std::string_view Symbol::toStr()
{
    switch(value)
    {
        case Symbol::Type::PAPER :
            return "PAPER";
        case Symbol::Type::ROCK :
            return "ROCK";
        case Symbol::Type::SCISSORS :
            return "SCISSORS";
    }
    return "";
}

Symbol::Symbol(Symbol::Type value)
: value(value)
{
}

std::optional<Symbol> Symbol::from(int index)
{
    switch(index)
    {
        case static_cast<int>(Symbol::Type::PAPER) :
            return Symbol::Type::PAPER;
        case static_cast<int>(Symbol::Type::ROCK) :
            return Symbol::Type::ROCK;
        case static_cast<int>(Symbol::Type::SCISSORS):
            return Symbol::Type::SCISSORS;
    }
    
    return std::nullopt;
}

std::string Symbol::printSymbolWithIndex()
{
    std::stringstream ss;
    ss << std::to_string(static_cast<int>(Symbol::Type::PAPER)) << " ---> PAPER" << std::endl
        << std::to_string(static_cast<int>(Symbol::Type::ROCK)) << " ---> ROCK" << std::endl
        << std::to_string(static_cast<int>(Symbol::Type::SCISSORS)) << " ---> SCISSORS";

    return ss.str();
}

bool Symbol::operator==(const Symbol& other) const
{
    return value == other.value;
}

bool Symbol::operator!=(const Symbol& other) const
{
    return !(*this == other);
}

bool Symbol::operator>(const Symbol& other) const
{
    return *this != other && !(*this < other);
}

bool Symbol::operator<(const Symbol& other) const
{
    if(*this == other)
        return false;

    if(value == Type::PAPER)
    {
        if(other.value == Type::SCISSORS)
            return true;
        if(other.value == Type::ROCK)
            return false;
    }

    if(value == Type::SCISSORS)
    {
        if(other.value == Type::ROCK)
            return true;
        if(other.value == Type::PAPER)
            return false;
    }

    if(value == Type::ROCK)
    {
        if(other.value == Type::PAPER)
            return true;
        if(other.value == Type::SCISSORS)
            return false;
    }
    return false;
}

}
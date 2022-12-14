#pragma once

#include <string>
#include <optional>

namespace Game
{

struct Symbol
{
    enum class Type
    {
        PAPER,
        ROCK,
        SCISSORS,
        SYMBOL_SIZE
    };

    Symbol(Type value);

    // Should use magic_enum lib here 
    std::string_view toStr();

    bool operator==(const Symbol& other) const;
    bool operator!=(const Symbol& other) const;
    bool operator<(const Symbol& other) const;
    bool operator>(const Symbol& other) const;

    static std::string printSymbolWithIndex();
    static std::optional<Symbol> from(int index);

    Type value = Type::SYMBOL_SIZE;
};

}
#pragma once
#include <array>
#include <mutex>


class IDGenerator
{
public:
    static constexpr char MinValue = '!'; // 33
    static constexpr char MaxValue = '~'; // 126
    inline static const std::string FileName = "lastId.txt";

    IDGenerator(const IDGenerator& ) = delete;
    IDGenerator(IDGenerator&& ) = delete;
    IDGenerator& operator=(const IDGenerator&) = delete;
    IDGenerator& operator=(const IDGenerator&&) = delete;

    // Generate in a thread safe way a 32 bits identifier.
    // The identifier is composed by 4 printable ASCII characters (from 33 to 126)
    // The number of combinations is up to 74 805 201 ((126-33)^4)
    static std::array<char, 4> generate();
private:

    class Id
    {
    public:
        using Type = std::array<char, 4>;
        Id() = default;
        Id(const std::string& str);
        Id& operator++();    // Prefix increment operator.
        Id operator++(int);     // Postfix increment operator.
        Type get();
    private:
        Type _value = {MinValue, MinValue, MinValue, MinValue};
    };

    IDGenerator();
    ~IDGenerator();
    Id _generate();

    std::mutex _mutex;
    Id _currentId;

};
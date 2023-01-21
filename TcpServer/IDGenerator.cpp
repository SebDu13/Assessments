#include "IDGenerator.hpp"
#include <iostream>
#include <fstream>

IDGenerator::IDGenerator()
{
    std::fstream file;
    file.open(FileName);

    if(file.is_open())
    {
        int index = 0;
        std::string id;
        file >> id;
        _currentId = Id(id);
    }
}

IDGenerator::~IDGenerator()
{
    std::ofstream fileStream(FileName, std::ofstream::trunc);

    for(char c : _currentId.get())
        fileStream << c;
}

std::array<char, 4> IDGenerator::generate()
{
    static IDGenerator iDGenerator;
    return iDGenerator._generate().get();
}

IDGenerator::Id IDGenerator::_generate()
{
    std::scoped_lock(_mutex);
    return _currentId++;
}

IDGenerator::Id::Id(const std::string& str)
{
    if(str.size() != _value.size())
        throw(std::runtime_error("IDGenerator::Id::Id() str ill-formated " + str ));

    for(int i = 0; i < str.size(); ++i)
    {
        if( str[i] < MinValue || str[i] > MaxValue)
            throw(std::runtime_error("IDGenerator::Id::Id() str ill-formated " + str ));
        _value[i] = str[i];
    }
}

IDGenerator::Id& IDGenerator::Id::operator++()
{
    for(int i = 0; i < _value.size(); ++i)
    {
        if(_value[i] == MaxValue)
        {
            _value[i] = MinValue;
            continue;
        }
        else
        {
            ++_value[i];
            break;
        }
    }
    return *this;
}

IDGenerator::Id IDGenerator::Id::operator++(int)
{
   IDGenerator::Id temp = *this;
   ++*this;
   return temp;
}

std::array<char, 4> IDGenerator::Id::get()
{
    return _value;
}
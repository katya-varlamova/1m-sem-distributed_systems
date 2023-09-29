#pragma once

#include <string>

struct User
{
    User() = default;
    User(
      int id,
      const std::string& name,
      const std::string& address,
      const std::string& work,
      const int& age
    );
    int id;
    std::string name, address, work;
    int age;
    bool operator==( const User& l ) const
    {
        return l.id == this->id && l.name == this->name &&
               l.age == this->age && l.address == this->address &&
               l.work == this->work;
    }
};
#pragma once

#include <string>

typedef enum
{
    MALE,
    FEMALE,
    OTHER
} gender_t;

std::string
GenderToStr( gender_t g );
gender_t
StrToGender( const std::string& str );

struct User
{
    User() = default;
    User(
      int id,
      const std::string& login,
      const std::string& password,
      const std::string& name,
      const std::string& birthDate,
      const std::string& userData,
      const std::string& contact,
      const std::string& photo_link,
      gender_t gender
    );
    int id;
    std::string login, password, name, birthDate, userData, contact, photo_link;
    gender_t gender;
    bool operator==( const User& l ) const
    {
        return l.id == this->id && l.login == this->login &&
               l.password == this->password && l.name == this->name &&
               l.birthDate == this->birthDate && l.userData == this->userData &&
               l.contact == this->contact && l.photo_link == this->photo_link &&
               l.gender == this->gender;
    }
};
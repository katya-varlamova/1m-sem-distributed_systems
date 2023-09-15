#pragma once

#include <memory>
#include <vector>
#include "structs/User.h"

class IUserRepository
{
public:
    virtual int CreateUser( const User& session ) = 0;
    virtual User GetUserByID( int id ) = 0;
    virtual std::vector<User> GetUsers( ) = 0;
    virtual void UpdateUserByID( int id, const User& user ) = 0;
    virtual void DeleteUserByID( int id ) = 0;
    virtual ~IUserRepository() = default;
};
using IUserRepositoryPtr = std::shared_ptr<IUserRepository>;
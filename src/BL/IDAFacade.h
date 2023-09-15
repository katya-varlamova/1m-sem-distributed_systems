#pragma once

#include <config/base_config.h>
#include <structs/User.h>

class IDAFacade
{
public:
    virtual std::vector<User> GetUsers( ) = 0;
    virtual User GetUserByID( int id ) = 0;
    virtual int CreateUser( const User& user ) = 0;
    virtual void UpdateUserByID( int id, const User& user ) = 0;
    virtual void DeleteUserByID( int id ) = 0;

    virtual ~IDAFacade() = 0;
};

using IDAFacadePtr = std::shared_ptr<IDAFacade>;
#pragma once

#include <config/base_config.h>
#include <structs/User.h>
#include "IDAFacade.h"
class IBLFacade{
public:
    IBLFacade(IDAFacadePtr da_facade) : m_da_facade(da_facade) {}
    virtual std::vector<User> GetUsers( ) = 0;
    virtual User GetUserByID( int id ) = 0;
    virtual int CreateUser( const User& user ) = 0;
    virtual void UpdateUserByID( int id, const User& user ) = 0;
    virtual void DeleteUserByID( int id ) = 0;

    virtual ~IBLFacade() = 0;

protected:
    IDAFacadePtr m_da_facade;
};

using IBLFacadePtr = std::shared_ptr<IBLFacade>;



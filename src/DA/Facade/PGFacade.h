#pragma once

#include "IDAFacade.h"
#include "Repositories/IUserRepository.h"
class PGFacade : public IDAFacade
{
public:
    PGFacade(const IUserRepositoryPtr& userRepository);
    virtual std::vector<User> GetUsers( ) override;
    virtual User GetUserByID( int id ) override;
    virtual int CreateUser( const User& user ) override;
    virtual void UpdateUserByID( int id, const User& user ) override;
    virtual void DeleteUserByID( int id ) override;

private:
    IUserRepositoryPtr m_userRepository;
};

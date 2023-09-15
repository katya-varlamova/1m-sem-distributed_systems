#pragma once
#include <cstring>
#include <map>
#include <memory>
#include <string>

#include "Connection/PGConnection.h"
#include "IUserRepository.h"

#include "exceptions/database_exceptions.h"
#include "logger/LoggerFactory.h"
class PGUserRepository : public IUserRepository
{
public:
    PGUserRepository( const PGConnectionCreatorPtr& connectionCreator )
    {
        m_conn = connectionCreator->GetConnect();
        m_connectionCreator = connectionCreator;
    }
    std::vector<User> GetUsers( ) override;
    int CreateUser( const User& user ) override;
    User GetUserByID( int id ) override;
    void UpdateUserByID( int id, const User& user ) override;
    void DeleteUserByID( int id ) override;

    ~PGUserRepository() override
    {
        m_connectionCreator->FinishConnect( m_conn );
    }

protected:
    User ParseUser( std::map<std::string, std::string> strs );

private:
    PGConnectPtr m_conn;
    PGConnectionCreatorPtr m_connectionCreator;
};

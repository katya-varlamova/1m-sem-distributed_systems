#include "PGUserRepository.h"
User
PGUserRepository::ParseUser( std::map<std::string, std::string> strs )
{
    return User(
      std::stoi( strs[std::string( "id" )] ),
      strs[std::string( "name" )],
      strs[std::string( "address" )],
      strs[std::string( "work" )],
      std::stoi(strs[std::string( "age" )])
    );
}
std::vector<User> PGUserRepository::GetUsers() {
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );

    std::string query(
            "select * from person;"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string(
                        "GetUsers: "
                ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "No such users!"
        );
    }

    std::vector<User> users;
    for ( int i = 0; i < PQntuples( pgRes ); i++ ) {
        std::map<std::string, std::string> res;
        for ( int j = 0; j < PQnfields( pgRes ); ++j ) {
            char* fn = PQfname( pgRes, j );
            char* val = PQgetvalue( pgRes, i, j );
            res[std::string( fn )] = std::string( val );
        }
        users.push_back( ParseUser( res ) );
    }

    LoggerFactory::GetLogger()->LogInfo(
            "GetUsers: no errors!"
    );
    PQclear( pgRes );
    return users;
}
User
PGUserRepository::GetUserByID( int id )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
      "select * from person where id = " + std::to_string( id ) + ";"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK || PQntuples( pgRes ) == 0 ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "GetUserByID: no such user: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "no such user" );
    }

    std::map<std::string, std::string> res;
    for ( int j = 0; j < PQnfields( pgRes ); ++j ) {
        char* fn = PQfname( pgRes, j );
        char* name = PQgetvalue( pgRes, 0, j );
        res[std::string( fn )] = std::string( name );
    }
    auto user = ParseUser( res );

    LoggerFactory::GetLogger()->LogInfo( "GetUserByID: user found" );
    PQclear( pgRes );
    return user;
}
int
PGUserRepository::CreateUser( const User& user )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );

    std::string query(
            "insert into person (name, age, address, work) values ('"
            + user.name + "', "
            + std::to_string(user.age) + ", '"
            + user.address + "', '"
            + user.work + "') returning id;"
    );

    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string( "CreateUser: registration failed: " ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "registration failed" );
    }
    int id;
    for ( int i = 0; i < PQntuples( pgRes ); i++ ) {
        int fn = PQfnumber( pgRes, "id" );
        char* name = PQgetvalue( pgRes, i, fn );
        id = std::strtol( name, &name, 10 );
    }

    LoggerFactory::GetLogger()->LogInfo(
            ( std::string( "CreateUser: registration succeed with uid = " ) +
              std::to_string( id ) )
                    .c_str()
    );

    PQclear( pgRes );
    return id;
}

void
PGUserRepository::UpdateUserByID( int id, const User& user )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
            "update Person set "
            "name = '" + user.name + "', "
            "age = " + std::to_string(user.age) + ", "
            "address = '" + user.address + "', "
            "work = '"+ user.work + "' where id = " +
            std::to_string( id ) + ";"
    );

    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_COMMAND_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "UpdateUserByID: user update failed: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "execution error" );
    }
    LoggerFactory::GetLogger()->LogInfo( "UpdateUserByID: user was updated" );
    PQclear( pgRes );
}
void
PGUserRepository::DeleteUserByID( int id )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
      "delete from person where id = " + std::to_string( id ) + ";"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_COMMAND_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "DeleteUserByID: there was no such user: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "there was no such user" );
    }

    LoggerFactory::GetLogger()->LogInfo( "DeleteUserByID: user was deleted" );
    PQclear( pgRes );
}

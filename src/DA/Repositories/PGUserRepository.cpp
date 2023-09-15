#include "PGUserRepository.h"
User
PGUserRepository::ParseUser( std::map<std::string, std::string> strs )
{
    return User(
      std::stoi( strs[std::string( "id" )] ),
      strs[std::string( "login" )],
      "",
      strs[std::string( "name" )],
      strs[std::string( "birthdate" )],
      strs[std::string( "accountdata" )],
      strs[std::string( "contact" )],
      strs[std::string( "photolink" )],
      StrToGender( std::string( strs["gender"] ) )
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
    auto gen = GenderToStr( user.gender );
    const char* paramValues[8] = {
            user.login.c_str(),    user.password.c_str(),
            user.name.c_str(),     user.birthDate.c_str(),
            gen.c_str(),           user.contact.c_str(),
            user.userData.c_str(), user.photo_link.c_str()
    };
    int paramLengths[8];
    for ( int i = 0; i < 8; i++ )
        paramLengths[i] = (int)strlen( paramValues[i] );
    int paramFormats[8] = { 0, 1, 0, 0, 0, 0, 0, 0 };

    PGresult* pgRes = PQexecParams(
            m_conn,
            "insert into person (login, password, name, birthDate, "
            "gender, contact, accountData, photoLink) values "
            "($1, $2, $3, $4, $5, $6, $7, $8) returning id;",
            8,
            NULL,
            paramValues,
            paramLengths,
            paramFormats,
            0
    );

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
    auto gen = GenderToStr( user.gender );
    const char* paramValues[8] = {
        user.login.c_str(),    user.password.c_str(),
        user.name.c_str(),     user.birthDate.c_str(),
        gen.c_str(),           user.contact.c_str(),
        user.userData.c_str(), user.photo_link.c_str()
    };
    int paramLengths[8];
    for ( int i = 0; i < 8; i++ )
        paramLengths[i] = (int)strlen( paramValues[i] );
    int paramFormats[8] = { 0, 1, 0, 0, 0, 0, 0, 0 };

    PGresult* pgRes = PQexecParams(
      m_conn,
      ( "update Person set login = $1, password = $2, name = $3, "
        "birthDate = $4, gender = $5, contact = $6, accountData = $7, "
        "photoLink = $8"
        " where id = " +
        std::to_string( id ) + ";" )
        .c_str(),
      8,
      NULL,
      paramValues,
      paramLengths,
      paramFormats,
      0
    );

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

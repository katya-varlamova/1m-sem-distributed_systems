#define BOOST_TEST_MODULE test_module

#include <common_macros.h>
#include <config/yamlcpp_config.h>
#include <openssl/sha.h>

#include <boost/test/included/unit_test.hpp>
#include <cstring>
#include <functional>
#include <string>

#include "Factory/PGFactory.h"
#include "exceptions/database_exceptions.h"
#include "logger/LoggerFactory.h"


IDAFacadePtr f;

std::string CONFIG_PATH;
std::string
sha256( const std::string& str )
{
    std::vector<unsigned char> hash( SHA256_DIGEST_LENGTH );

    SHA256(
            reinterpret_cast<const unsigned char*>( &str[0] ), str.size(), &hash[0]
    );

    hash.push_back( '\0' );

    return std::string( reinterpret_cast<const char*>( &hash[0] ) );
}

IDAFacadePtr
preapre()
{
    CONFIG_PATH = "/Users/kate/Desktop/distributed_systems/src/DA/DAUnits/ConfigDA.yml";// boost::unit_test::framework::master_test_suite().argv[1];

    if ( !f ) {
        std::shared_ptr<BaseConfig> c( new YamlCppConfig( CONFIG_PATH ) );
        LoggerFactory::InitLogger( c );
        IFactoryPtr factory = IFactoryPtr( new PGFactory( c ) );
        f = factory->CreateDAFacade();
    }
    return f;
}
BOOST_AUTO_TEST_CASE( TestRegistrationRepo )
{
    // arrange
    auto facade = preapre();
    User u(
      -1,
      "reg_user",
      sha256( "pass" ),
      "name",
      "2001-09-18",
      "data",
      "contact",
      "/path\\",
      MALE
    );
    std::function<bool( const DatabaseExecutionException& e )> crt_func(
      [&]( const DatabaseExecutionException& e ) {
          return std::string( e.what() ) ==
                 std::string( "(Database error) (Database execution error) "
                              "registration failed" );
      }
    );

    // positive
    //  act
    int id = facade->CreateUser( u );
    // assert
    BOOST_CHECK_EQUAL( id != -1, true );

    // negative
    // act & assert
    BOOST_CHECK_EXCEPTION(
      facade->CreateUser( u ), DatabaseExecutionException, crt_func
    );
    facade->DeleteUserByID( id );
}

BOOST_AUTO_TEST_CASE( TestUserRepoGetters )
{
    // arrange
    auto facade = preapre();
    User u(
      -1,
      "test_user",
      sha256( "pass" ),
      "name",
      "2001-09-18",
      "data",
      "contact",
      "/path\\",
      MALE
    );
    int id = facade->CreateUser( u );
    u.id = id;

    std::function<bool( const DatabaseExecutionException& e )>
      check_such_user_exc_func( [&]( const DatabaseExecutionException& e ) {
          return std::string( e.what() ) ==
                 std::string( "(Database error) (Database execution error) "
                              "no such user" );
      } );

    // positive get by id
    // act
    User checkUser = facade->GetUserByID( id );
    // assert
    u.password = "";
    BOOST_CHECK_EQUAL( u == checkUser, true );

    // negative get by id
    // act && assert
    BOOST_CHECK_EXCEPTION(
      facade->GetUserByID( -1 ),
      DatabaseExecutionException,
      check_such_user_exc_func
    );

    User u2(
            -1,
            "test_user2",
            sha256( "pass" ),
            "name2",
            "2001-09-19",
            "data2",
            "contact2",
            "/path2",
            FEMALE
    );
    int id2 = facade->CreateUser( u2 );
    auto users = facade->GetUsers();
    BOOST_CHECK_EQUAL( users.size() == 2, true );

    facade->DeleteUserByID( id );
    facade->DeleteUserByID( id2 );
}

BOOST_AUTO_TEST_CASE( TestUserRepoBasic )
{
    // arrange
    auto facade = preapre();
    User u1(
      -1,
      "katya",
      sha256( "pass" ),
      "name",
      "2001-08-17",
      "data",
      "contact",
      "/path\\",
      FEMALE
    );
    User u1_upd(
      -1,
      "katya_u",
      sha256( "pass_u" ),
      "name_u",
      "2002-09-18",
      "data_u",
      "contact_u",
      "/path_u\\",
      OTHER
    );
    int id1 = facade->CreateUser( u1 );
    u1.id = id1;
    u1_upd.id = id1;
    u1_upd.password = "";

    std::function<bool( const DatabaseExecutionException& e )>
      check_such_user_exc_func( [&]( const DatabaseExecutionException& e ) {
          return std::string( e.what() ) ==
                 std::string( "(Database error) (Database execution error) "
                              "no such user" );
      } );

    std::function<bool( const DatabaseExecutionException& e )>
      check_was_no_such_user_exc_func( [&]( const DatabaseExecutionException& e
                                       ) {
          return std::string( e.what() ) ==
                 std::string( "(Database error) (Database execution error) "
                              "there was no such user" );
      } );
    // act
    facade->UpdateUserByID( id1, u1_upd );

    // assert
    BOOST_CHECK_EQUAL( facade->GetUserByID( id1 ) == u1_upd, true );

    // delete
    // act
    facade->DeleteUserByID( id1 );
    // assert
    BOOST_CHECK_EXCEPTION(
      facade->GetUserByID( id1 ),
      DatabaseExecutionException,
      check_such_user_exc_func
    );
}
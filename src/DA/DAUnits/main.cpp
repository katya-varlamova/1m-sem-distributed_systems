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
#include <boost/di.hpp>
#include <boost/filesystem.hpp>
namespace di = boost::di;
IDAFacadePtr f;

std::string CONFIG_PATH;

IDAFacadePtr
preapre()
{
    CONFIG_PATH = boost::unit_test::framework::master_test_suite().argv[1];
    if ( !f ) {
        auto injector = di::make_injector(
                di::bind<BaseConfig>().to<YamlCppConfig>(),
                di::bind<std::string>.named( configFileName ).to( CONFIG_PATH ),
                di::bind<IDAFactory>().to<PGDAFactory>());
        auto factory = injector.create<IDAFactoryPtr>();
        LoggerFactory::InitLogger( injector.create<BaseConfigPtr>() );
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
            "name",
            "address",
            "work",
            21
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

    facade->DeleteUserByID( id );
}

BOOST_AUTO_TEST_CASE( TestUserRepoGetters )
{
    // arrange
    auto facade = preapre();
    User u(
      -1,
      "name",
      "address",
      "work",
      21
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
            "name2",
            "address2",
            "work2",
            22
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
            "name",
            "address",
            "work",
            21
    );
    User u1_upd(
            -1,
            "name_upd",
            "address_upd",
            "work_upd",
            22
    );
    int id1 = facade->CreateUser( u1 );
    u1.id = id1;
    u1_upd.id = id1;

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
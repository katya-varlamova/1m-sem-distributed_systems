#include "User.h"
User::User(
        int id,
        const std::string& name,
        const std::string& address,
        const std::string& work,
        const int& age
)
  : id( id )
  , name( name )
  , address( address )
  , work( work )
  , age( age )
{
}
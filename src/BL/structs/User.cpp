#include "User.h"
std::string
GenderToStr( gender_t g )
{
    switch ( g ) {
        case MALE:
            return "male";
        case FEMALE:
            return "female";
        case OTHER:
            return "other";
    }
    return "other";
}
gender_t
StrToGender( const std::string& str )
{
    if ( str == "male" )
        return MALE;
    if ( str == "female" )
        return FEMALE;
    if ( str == "other" )
        return OTHER;
    return OTHER;
}
User::User(
  int id,
  const std::string& login,
  const std::string& password,
  const std::string& name,
  const std::string& birthDate,
  const std::string& userData,
  const std::string& contact,
  const std::string& photo_link,
  gender_t gender
)
  : id( id )
  , login( login )
  , password( password )
  , name( name )
  , birthDate( birthDate )
  , userData( userData )
  , contact( contact )
  , photo_link( photo_link )
  , gender( gender )
{
}
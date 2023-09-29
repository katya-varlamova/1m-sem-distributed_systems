#include "PGFacade.h"
PGFacade::PGFacade(
  const IUserRepositoryPtr& userRepository
) : m_userRepository( userRepository )
{
}
std::vector<User> PGFacade::GetUsers( ) {
    return m_userRepository->GetUsers( );
}
User
PGFacade::GetUserByID( int id )
{
    return m_userRepository->GetUserByID( id );
}

int
PGFacade::CreateUser( const User& user )
{
    return m_userRepository->CreateUser( user );
}
void
PGFacade::UpdateUserByID( int id, const User& user )
{
    m_userRepository->UpdateUserByID( id, user );
}
void
PGFacade::DeleteUserByID( int id )
{
    m_userRepository->DeleteUserByID( id );
}

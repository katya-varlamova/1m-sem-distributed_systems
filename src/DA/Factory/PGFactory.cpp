#include "PGFactory.h"
IDAFacadePtr
PGDAFactory::CreateDAFacade()
{
    if ( m_idaFacade )
        return m_idaFacade;

    return (
      m_idaFacade = IDAFacadePtr( new PGFacade(
        CreateUserRepository()
      ) )
    );
}
IUserRepositoryPtr
PGDAFactory::CreateUserRepository()
{
    PGConnectionCreatorPtr connectionCreator =
      PGConnectionCreatorPtr( new PGConnection( m_config ) );
    return m_userRepository ? m_userRepository
                            : ( m_userRepository = IUserRepositoryPtr(
                                  new PGUserRepository( connectionCreator )
                                ) );
}

#include "PGFactory.h"
IDAFacadePtr
PGFactory::CreateDAFacade()
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
PGFactory::CreateUserRepository()
{
    PGConnectionCreatorPtr connectionCreator =
      PGConnectionCreatorPtr( new PGConnection( m_config ) );
    return m_userRepository ? m_userRepository
                            : ( m_userRepository = IUserRepositoryPtr(
                                  new PGUserRepository( connectionCreator )
                                ) );
}

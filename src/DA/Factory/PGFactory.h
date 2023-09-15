#pragma once

#include <boost/di.hpp>

#include "Connection/PGConnection.h"
#include "Facade/PGFacade.h"
#include "IFactory.h"
#include "Repositories/PGUserRepository.h"

class PGFactory : public IFactory
{
public:
    PGFactory (const std::shared_ptr<BaseConfig>& config )
      : m_config( config )
    {}
    IDAFacadePtr CreateDAFacade() override;
    ~PGFactory() = default;

protected:
    IUserRepositoryPtr CreateUserRepository();

private:
    std::shared_ptr<BaseConfig> m_config;
    IUserRepositoryPtr m_userRepository;
    IDAFacadePtr m_idaFacade;
};

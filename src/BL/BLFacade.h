#pragma once

#include "IBLFacade.h"
#include <boost/di.hpp>
class BLFacade : public IBLFacade {
public:
    BOOST_DI_INJECT( BLFacade, const std::shared_ptr<IDAFactory> &factory) {
        m_da_facade = factory->CreateDAFacade();
    }
    User GetUserByID(int id) override;
    std::vector<User> GetUsers() override;
    void UpdateUserByID(int id, const User &user) override;
    void DeleteUserByID(int id) override;
    int CreateUser(const User &user) override;
protected:
    IDAFacadePtr m_da_facade;
};




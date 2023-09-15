#pragma once

#include "IBLFacade.h"
class BLFacade : public IBLFacade {
public:
    BLFacade(IDAFacadePtr idaFacade) : IBLFacade(idaFacade) {}
    User GetUserByID(int id) override;
    std::vector<User> GetUsers() override;
    void UpdateUserByID(int id, const User &user) override;
    void DeleteUserByID(int id) override;
    int CreateUser(const User &user) override;
};




#include "BLFacade.h"
int BLFacade::CreateUser(const User &user) {
    m_da_facade->CreateUser(user);
}
int BLFacade::UpdateUserByID(int id, const User &user) {
    m_da_facade->UpdateUserByID(id, user);
}
std::vector<User> BLFacade::GetUsers()
{
    m_da_facade->GetUsers();
}
void BLFacade::DeleteUserByID(int id)
{
    m_da_facade->DeleteUserByID(id);
}
User BLFacade::GetUserByID(int id)
{
    m_da_facade->GetUserByID(id);
}
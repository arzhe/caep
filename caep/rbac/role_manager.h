#ifndef CAEP_ROLE_MANAGER_H
#define CAEP_ROLE_MANAGER_H 

#include <string>
#include <vector>

namespace caep {

class RoleManager {
public:

    virtual void Clear() = 0;

    virtual void AddLink(std::string role1, std::string role2) = 0;

    virtual void DeleteLink(std::string role1, std::string role2) = 0;

    virtual bool HasLink(std::string role1, std::string role2) = 0;

    virtual std::vector<std::string> GetRoles(std::string role) = 0;

    virtual std::vector<std::string> GetUsers(std::string role) = 0;

    virtual void PrintRoles() = 0;
};


} // namespace caep 

#endif

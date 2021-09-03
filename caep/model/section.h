#ifndef CAEP_SECTION_H
#define CAEP_SECTION_H

#include <memory>

#include "../rbac/role_manager.h"

namespace caep {

enum policy_op {
    policy_add,
    policy_remove
};

typedef enum policy_op policy_op;

/******************************
 * class Section represents an expression in CAEP model.
 *
 * eg: # model.ini
 * [applicability]
 * a = sub, obj, act
 *
 * eg: # policy.csv
 * a, Alice, file, read
 * a, Bob, file, write
 * 
 * in this case:
 * @member 
 *  #key: "a"
 *  #value: "sub, obj, act"
 *  #key_val: "sub", "obj", "act"
 *  #policy: {{"Alice", "file", "read"},
 *            {"Bob", "file", "write"}}
 *  #rm: define roles manager 
 ******************************/
class Section {
public:
    std::string key;
    std::string value;
    std::vector<std::string> key_val;
    std::vector<std::vector<std::string>> policy;
    std::shared_ptr<RoleManager> rm;

    void BuildIncrementalRoleLinks(std::shared_ptr<RoleManager> rm, policy_op op, std::vector<std::vector<std::string>> rules);
    
    void BuildRoleLinks(std::shared_ptr<RoleManager> rm);
};

} // namespace caep 

#endif

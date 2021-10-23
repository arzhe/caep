#ifndef CAEP_DEFAULT_EFFECTOR_CPP
#define CAEP_DEFAULT_EFFECTOR_CPP

#include "./default_effector.h"
#include "../exception/caep_exception.h"

namespace caep {

/**
 * @breif MergeEffects merges all matching results collected by the enforcer into a single decision.
 */
bool DefaultEffector::MergeEffects(std::string expr, std::vector<Effect> effects, std::vector<float> results) {
    bool result;

    if(!expr.compare("AllowPriority")) {
        result = false;
        for(size_t i = 0; i < effects.size(); ++i) {
            if(effects[i] == Effect::Allow) {
                result = true;
                break;
            }
        }
    }
    else if(!expr.compare("DenyPriority")) {
            result = true;
            for(size_t i = 0; i < effects.size(); ++i) {
                if(effects[i] == Effect::Deny) {
                    result = false;
                    break;
                }
            }
    }
    else if(!expr.compare("FirstPriority")) {
        result = effects[0] == Effect::Allow;
    }
    else {
        throw UnsupportedOperationException("Unsupported effect");
    }

    return result;

}

} // namespace ceap 


#endif

#ifndef CAEP_DEFAULT_EFFECTOR_H
#define CAEP_DEFAULT_EFFECTOR_H

#include "./effector.h"

namespace caep {

/**
 * @breif DefaultEffector is default effector for Casbin.
 */
class DefaultEffector : public Effector{
public:
    /**
    * @breif MergeEffects merges all matching results collected by the enforcer into a single decision.
    */
    bool MergeEffects(std::string expr, std::vector<Effect> effects, std::vector<float> results);
};

} // namespace caep 

#endif

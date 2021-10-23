#ifndef CAEP_EFFECTOR_H
#define CAEP_EFFECTOR_H

#include <string>
#include <vector>

#include "./effect.h"

namespace caep {

/**
* @breif Effector is the abstract class for Caep effectors.
*/
class Effector{
public:
    virtual ~Effector() {}
    /**
    * @breif MergeEffects merges all matching results collected by the enforcer into a single decision.
    *
    * @param expr the expression of [policy_effect].
    * @param effects the effects of all matched rules.
    * @param results the matcher results of all matched rules.
    * @return the final effect.
    */
    virtual bool MergeEffects(std::string expr, std::vector<Effect> effects, std::vector<float> results) = 0;
};

} // namespace caep 

#endif

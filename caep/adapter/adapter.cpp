#ifndef CAEP_ADAPTER_CPP
#define CAEP_ADAPTER_CPP

#include "./adapter.h"
#include "../util/caep_util.h"

namespace caep {

// LoadPolicyLine loads a text line as a policy rule to model.
void LoadPolicyLine(std::string line, Model* model) {
    if(line == "" || line.find("#") == 0)
        return;

    auto tokens = CaepUtil::Split(line, ",", -1);
    for(auto& t : tokens)
        t = CaepUtil::Trim(t);

    auto key = tokens[0];
    auto sec = key.substr(0, 1);
    std::vector<std::string> new_tokens(tokens.begin() + 1, tokens.end());

    if(model->m.find(sec) == model->m.end())
        model->m[sec] = SectionMap();

    (model->m[sec].section_map[key]->policy).push_back(new_tokens);
}

} // namespace caep 

#endif

#include <gtest/gtest.h>
#include <caep/caep.h>

namespace {

void TestMatcher(caep::Matcher& m, const std::unordered_map<std::string, bool> res) {
    auto model = caep::Model::NewModelFromFile("../../example/basic_rbac_model.ini");
    auto adapter = caep::FileAdapter("../../example/basic_rbac_model.csv");
    adapter.LoadPolicy(model);
    m.LoadMatcherFromModel(model);

    auto my_res = m.matcher_switch;

    ASSERT_EQ(my_res, res);
}

TEST(TestMatcher, TestMather) {
    caep::Matcher m;
    TestMatcher(m, {{"DefaultMatcher", true},
                    {"RoleMatcher", true}});
}

}

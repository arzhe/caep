#include <gtest/gtest.h>
#include <caep/caep.h>

namespace {

void TestAdapterFn(const std::vector<std::vector<std::string>>& rules) {
    caep::Model* model = caep::Model::NewModelFromFile("../../example/basic_rbac_model.ini");
    caep::FileAdapter f_adapter("../../example/basic_rbac_model.csv");
    f_adapter.LoadPolicy(model);
    auto my_rules = model->GetPolicy("a", "a");
    
    ASSERT_EQ(my_rules, rules);

}

TEST(TestAdapter, TestAdapter) {
    TestAdapterFn({{"Alice", "data1", "read"},
                   {"Bob", "data2", "read"},
                   {"admin", "data1", "write"},
                   {"admin", "data2", "write"}
                  });
}

}

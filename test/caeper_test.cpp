#include <gtest/gtest.h>
#include <caep/caep.h>

namespace {

TEST(TestCaeper, TestThreeParams) {
    std::string model = "../../example/basic_rbac_model.ini";
    std::string policy = "../../example/basic_rbac_model.csv";

    caep::Caeper c(model, policy);
    
    ASSERT_EQ(c.Caep({"Alice", "data1", "read"}), true);
    ASSERT_EQ(c.Caep({"Bob", "data2", "read"}), true);
    ASSERT_EQ(c.Caep({"admin", "data1", "write"}), true);
    ASSERT_EQ(c.Caep({"admin", "data2", "write"}), true);

    ASSERT_EQ(c.Caep({"Alice", "data1", "write"}), true);
    ASSERT_EQ(c.Caep({"Alice", "data2", "write"}), true);

    ASSERT_EQ(c.Caep({"Bob", "data2", "write"}), false);
    ASSERT_EQ(c.Caep({"Bob", "data1", "read"}), false);
}

//TEST(TestCaeper, TestFourParams) {
//    std::string model = "../../example/rbac_with_domain.ini";
//    std::string policy = "../../example/rbac_with_domain.csv";
//
//    caep::Caeper c(model, policy);
//
//    ASSERT_EQ(c.Caep({"Alice", "data1", "read", "domain1"}), true);
//    ASSERT_EQ(c.Caep({"Bob", "data2", "read", "domain2"}), true);
//    ASSERT_EQ(c.Caep({"admin", "data1", "write", "domain1"}), true);
//    ASSERT_EQ(c.Caep({"admin", "data2", "write", "domain2"}), true);
//    ASSERT_EQ(c.Caep({"Alice", "data1", "write", "domain1"}), true);
//    ASSERT_EQ(c.Caep({"Bob", "data2", "write", "domain2"}), true);
//
//    ASSERT_EQ(c.Caep({"Alice", "data1", "read", "domain2"}), false);
//    ASSERT_EQ(c.Caep({"Bob", "data1", "write", "domain1"}), false);
//}
//
}

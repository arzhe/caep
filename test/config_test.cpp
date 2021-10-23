#include <gtest/gtest.h>
#include <caep/caep.h>

namespace {

std::shared_ptr<caep::Config> GetTestConfig() {
    return caep::Config::NewConfigFromFile("../../caep/config/testdata/testini.ini");
}

TEST(TestConfig, TestBool) {
    auto config = GetTestConfig();
    EXPECT_EQ(config->GetBool("debug"), true);
}

TEST(TestConfig, TestString) {
    auto config = GetTestConfig();
    EXPECT_EQ(config->GetString("config_string::c_string"), "string");
}

TEST(TestConfig, TestInt) {
    auto config = GetTestConfig();
    EXPECT_EQ(config->GetInt("config_int::c_int"), 1);
}

TEST(TestConfig, TestFloat) {
    auto config = GetTestConfig();
    EXPECT_FLOAT_EQ(config->GetFloat("config_float::c_float"), 1.1);
}

TEST(TestConfig, TestStrings) {
    auto config = GetTestConfig();
    auto values = config->GetStrings("config_strings::c_strings");
    EXPECT_EQ(std::string("string1"), values[0]);
    EXPECT_EQ(std::string("string2"), values[1]);
}

} // namespace 

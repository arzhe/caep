#include <gtest/gtest.h>
#include <caep/caep.h>

namespace {

std::shared_ptr<caep::Config> GetTestConfig() {
    return caep::Config::NewConfigFromFile("../caep/config/testdata/testini.ini");
}

TEST(TestConfig, TestBool) {
    auto config = GetTestConfig();
    EXPECT_EQ(config->GetBool("debug"), true);
}

TEST(TestConfig, TestString) {
    auto config = GetTestConfig();
    EXPECT_EQ(config->GetString("config_string::config"), "string");
}

TEST(TestConfig, TestInt) {
    auto config = GetTestConfig();
    EXPECT_EQ(config->GetInt("config_int::config"), 1);
}

TEST(TestConfig, TestFloat) {
    auto config = GetTestConfig();
    EXPECT_FLOAT_EQ(config->GetFloat("config_float::config"), 1.1);
}

TEST(TestConfig, TestStrings) {
    auto config = GetTestConfig();
    auto values = config->GetStrings("config::config");
    EXPECT_EQ(std::string("config_1"), values[0]);
    EXPECT_EQ(std::string("config_2"), values[1]);
}

} // namespace 

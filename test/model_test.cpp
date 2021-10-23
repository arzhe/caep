#include <gtest/gtest.h>
#include <caep/caep.h>
#include <fstream>

namespace {

std::string basic_example = "../../example/basic_rbac_model.ini";
std::shared_ptr<caep::Config> basic_config;
void InitTest() {
    basic_config = caep::Config::NewConfigFromFile(basic_example);
}

TEST(TestModel, TestNewModel) {
    caep::Model* model = caep::Model::NewModel();
    ASSERT_NE(model, nullptr);
}

TEST(TestModel, TestNewModelFromFile) {
    caep::Model* model = caep::Model::NewModelFromFile(basic_example);
    ASSERT_NE(model, nullptr);
}

TEST(TestModel, TestNewModelFromString) {
    std::ifstream infile;
    infile.open(basic_example);
    std::string content;
    std::getline(infile, content, '\0');
    caep::Model* model = caep::Model::NewModelFromText(content);

    ASSERT_NE(model, nullptr);
}

TEST(TestModel, TestLoadModelFromConfig) {
    InitTest();
    caep::Model* model = caep::Model::NewModel();
    model->LoadModelFromConfig(basic_config);

    model = caep::Model::NewModel();
    std::shared_ptr<caep::Config> config = caep::Config::NewConfigFromText("");
    try {
        model->LoadModelFromConfig(config);
        FAIL();
    }
    catch (caep::MissingRequiredSections e) {
    }
}

TEST(TestModel, TestHasSection) {
    InitTest();
    caep::Model* model = caep::Model::NewModel();
    model->LoadModelFromConfig(basic_config);

    for (size_t i = 0; i < (caep::Model::required_sections).size(); i++) {
        ASSERT_TRUE(model->HasSection((caep::Model::required_sections)[i]));
    }

    model = caep::Model::NewModel();
    std::shared_ptr<caep::Config> config = caep::Config::NewConfigFromText("");
    try {
        model->LoadModelFromConfig(config);
        FAIL();
    }
    catch (caep::MissingRequiredSections e) {
    }

    for (const auto& section : caep::Model::required_sections)
        ASSERT_FALSE(model->HasSection(section));
}

TEST(TestModel, TestModel_AddDef) {
    caep::Model *model = caep::Model::NewModel();
    std::string s = "r";
    std::string v = "sub, res, act";

    bool ok = model->AddDef(s, s, v);
    ASSERT_TRUE(ok);

    ok = model->AddDef(s, s, "");
    ASSERT_FALSE(ok);
}

} // namespace

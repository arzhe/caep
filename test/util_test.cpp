#include <gtest/gtest.h>
#include <caep/caep.h>

namespace {

void TestRemoveCommentsFn(const std::string& s, const std::string& res) {
    std::string my_res = CaepUtil::RemoveComments(s);
    ASSERT_EQ(my_res, res);
}

TEST(TestCaepUtil, TestRemoveComments) {
    TestRemoveCommentsFn("a.sub # comments", "a.sub");
    TestRemoveCommentsFn("a.sub#comments", "a.sub");
    TestRemoveCommentsFn("a.sub###", "a.sub");
    TestRemoveCommentsFn("### comments", "");
    TestRemoveCommentsFn("a.sub", "a.sub");
}

void TestArrayEqualFn(const std::vector<std::string>& a, const std::vector<std::string>& b, bool res) {
    bool my_res = CaepUtil::ArrayEqual(a, b);
    ASSERT_EQ(my_res, res);
}

TEST(TestCaepUtil, TestArrayEquals) {
    TestArrayEqualFn({"a", "b", "c"}, {"a", "b", "c"}, true);
    TestArrayEqualFn({"a", "b", "c"}, {"a", "b"}, false);
    TestArrayEqualFn({"a", "b", "c"}, {"a", "c", "b"}, true);
    TestArrayEqualFn({"a", "b", "c"}, {}, false);
}

void TestSetSubtractFn(const std::vector<std::string>& a, const std::vector<std::string>& b, const std::vector<std::string>& res) {
    std::vector<std::string> my_res = CaepUtil::SetSubtract(a, b);
    ASSERT_EQ(my_res, res);
}

TEST(TestCaepUtil, TestSetSubtract) {
    TestSetSubtractFn({"a", "b", "c"}, {"a", "b", "c"}, {});
    TestSetSubtractFn({"a", "b", "c"}, {"b", "c"}, {"a"});
    TestSetSubtractFn({"a", "b", "c"}, {"c"}, {"a", "b"});
    TestSetSubtractFn({"a", "b", "c"}, {}, {"a", "b", "c"});
    TestSetSubtractFn({"a", "b", "c"}, {"d", "e", "f"}, {"a", "b", "c"});
}

void TestArrayRemoveDuplicatesFn(const std::vector<std::string>& s, const std::vector<std::string>& res) {
    auto my_res = s;
    CaepUtil::ArrayRemoveDuplicates(my_res);
    ASSERT_EQ(my_res, res);
}

TEST(TestCaepUtil, TestArrayRemoveDuplicates) {
    TestArrayRemoveDuplicatesFn({"a", "b", "c"}, {"a", "b", "c"});
    TestArrayRemoveDuplicatesFn({"a", "b", "c", "c"}, {"a", "b", "c"});
    TestArrayRemoveDuplicatesFn({"a", "b", "c", "c", "b", "a"}, {"a", "b", "c"});
    TestArrayRemoveDuplicatesFn({"c", "b", "a", "a", "b", "c"}, {"c", "b", "a"});
}

void TestTrimFn(const std::string& str, const std::string& res) {
    std::string my_res = CaepUtil::Trim(str);
    ASSERT_EQ(my_res, res);
}

TEST(TestCaepUtil, TestTrim) {
    TestTrimFn("trim", "trim");
    TestTrimFn("trim ", "trim");
    TestTrimFn(" trim", "trim");
    TestTrimFn(" trim ", "trim");
    TestTrimFn("trim trim", "trim trim");
}

void TestSplitFn(const std::string& str, const std::string& delim, int limit, const std::vector<std::string>& res) {
    auto my_res = CaepUtil::Split(str, delim, limit);
    ASSERT_EQ(my_res, res);
}

TEST(TestCaepUtil, TestSplit) {
    TestSplitFn("a, b, c", ",", -1, {"a", " b", " c"});
    TestSplitFn("a, b, c", ",", 1, {"a", " b, c"});
    TestSplitFn("a, b, c", ",", 5, {"a", " b", " c"});
    TestSplitFn("a && b && c", "&&", -1, {"a ", " b ", " c"});
}   

} // namespace 

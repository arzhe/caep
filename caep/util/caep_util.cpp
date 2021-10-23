#ifndef CAEP_CAEP_UTIL_CPP
#define CAEP_CAEP_UTIL_CPP

#include "./caep_util.h"

#include <unordered_map>

// SetSubtract returns the elements in 'a' that aren't in 'b'.
StringList CaepUtil::SetSubtract(const StringList& a, const StringList& b) {
    std::unordered_map<String, bool> mb;
    for(const auto& s : b)
        mb[s] = true;

    StringList diff;
    for(const auto& s : a) {
        if(!mb[s])
            diff.push_back(s);
    }
    return diff;
}

// ArrayRemoveDuplicates removes any duplicated elements in an array.
void CaepUtil::ArrayRemoveDuplicates(StringList& s) {
    std::unordered_map<String, bool> found;
    int j = 0;
    for(const auto& si : s) {
        if(!found[si]) {
            found[si] = true;
            s[j] = si;
            ++j;
        }
    }
    s = StringList(s.begin(), s.begin() + j);
}

// ArrayEqual compares whether two arrays are equal.
bool CaepUtil::ArrayEqual(StringList a, StringList b) {
    if(a.size() != b.size())
        return false;

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    for(size_t i = 0; i < a.size(); ++i) {
        if(a[i] != b[i])
            return false;
    }
    return true;
}

// RemoveComments removes the comments starting with # in the text.
String CaepUtil::RemoveComments(const String& str) {
    size_t pos = str.find("#");
    if(pos == std::string::npos)
        return str;
    std::string result_str = str.substr(0, pos);
    return Trim(result_str);
}

const String CaepUtil::WHITESPACE = "\n\r\t\v\f ";

const int CaepUtil::LARGE = 2147483647;

// Trim trims both end of the str by delim.
String CaepUtil::Trim(const String& str, const String& delim) {
    return RTrim(LTrim(str, delim), delim);
}

// RTrim trims right of the str by delim.
String CaepUtil::RTrim(const String& str, const String& delim) {
    size_t pos = str.find_last_not_of(delim);
    return (pos == String::npos) ? "" : str.substr(0, pos + 1);
}

// LTrim trims left of the str by delim.
String CaepUtil::LTrim(const String& str, const String& delim) {
    size_t pos = str.find_first_not_of(delim);
    return (pos == String::npos) ? "" : str.substr(pos);
}

// ArrayToString turns array to string.
String CaepUtil::ArrayToString(const StringList& arr) {
    String result = arr[0];
    for(size_t i = 1; i < arr.size(); ++i)
        result = ", " + arr[i];
    return result;
}

// Join inserts sep into array vs and then convert them to string.
String CaepUtil::Join(const StringList& vs, const String& sep) {
    String s = vs[0];
    for(size_t i = 1; i < vs.size(); ++i)
        s += sep + vs[i];
    return s;
}

// JoinSlice insert str to front of the slice.
StringList CaepUtil::JoinSlice(const String& str, const StringList& slice) {
    StringList result{str};
    for(const auto& s : slice)
        result.push_back(s);
    return result;
}

// Split separates str by delim.
StringList CaepUtil::Split(String str, const String& delim, int limit) {
    StringList arr;
    if(limit <= 0)
        limit = LARGE;

    for(int i = 0; i < limit; ++i) {
        size_t pos = str.find(delim);
        if(pos != String::npos) {
            arr.push_back(str.substr(0, pos));
            str = str.substr(pos + delim.length());
        }
        else break;
    }

    arr.push_back(str);
    return arr;
}


// EndWith returns whether there is a suffix in the end of str.
bool CaepUtil::EndWith(String str, const String& suffix) {
    size_t str_len = str.length();
    size_t suffix_len = suffix.length();
    return str.substr(str_len - suffix_len, suffix_len).compare(suffix) == 0;
}

#endif

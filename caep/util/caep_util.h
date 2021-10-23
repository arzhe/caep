#ifndef CAEP_CAEP_UTIL_H
#define CAEP_CAEP_UTIL_H

#include <string>
#include <vector>

typedef std::string String;
typedef std::vector<String> StringList;

class CaepUtil {
public:
    // SetSubtract returns the elements in 'a' that aren't in 'b'.
    static StringList SetSubtract(const StringList& a, const StringList& b);
    
    // ArrayRemoveDuplicates removes any duplicated elements in an array.
    static void ArrayRemoveDuplicates(StringList& s);

    // ArrayEqual compares whether two arrays are equal.
    static bool ArrayEqual(StringList a, StringList b);

    // RemoveComments removes the comments starting with # in the text.
    static String RemoveComments(const String& str);

    // Trim trims both end of the str by delim.
    static String Trim(const String& str, const String& delim = CaepUtil::WHITESPACE);
    
    // RTrim trims right of the str by delim.
    static String RTrim(const String& str, const String& delim = CaepUtil::WHITESPACE);

    // LTrim trims left of the str by delim.
    static String LTrim(const String& str, const String& delim = CaepUtil::WHITESPACE);

    // ArrayToString turns array to string.
    static String ArrayToString(const StringList& arr);

    // Join inserts sep into array vs and convert them to string.
    static String Join(const StringList& vs, const String& sep = " ");

    // JoinSlice insert str to front of the slice.
    static StringList JoinSlice(const String& s, const StringList& slice);

    // Split separates str by delim.
    static StringList Split(String str, const String& delim, int limit = 0);

    // EndWith returns whether there is a suffix in the end of str.
    static bool EndWith(String str, const String& suffix);

private:
    static const String WHITESPACE;
    static const int LARGE;
};

#endif

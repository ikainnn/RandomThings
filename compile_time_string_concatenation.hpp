#include <array>

inline constexpr std::size_t string_length(const char* src)
{
    for (std::size_t pos{ 0 }; src[pos]; ++pos)
    {
        if (!src[pos+1]) return pos + 1;
    }
}

template <const char* LeftSide, const char* RightSide> struct StringCat
{
private:
    static constexpr std::size_t TotalSize = string_length(LeftSide) + string_length(RightSide);

    static constexpr void push_chars(char* dest, const char* src, std::size_t pos = 0)
    {
        if (pos < string_length(src))
        {
           *dest = src[pos]; return push_chars(++dest, src, ++pos);
        }
    }

    static constexpr auto concatenate(const char* leftSide, const char* rightSide)
    {
        std::array<char, TotalSize> stringArray = { };

        push_chars(&stringArray[0], leftSide);
        push_chars(&stringArray[string_length(leftSide)], rightSide);

        return stringArray;
    }

    static constexpr auto _stringArray = concatenate(LeftSide, RightSide);

public:
    static constexpr auto value = _stringArray.data();
};

/*
Example Usage:
    constexpr char g_your[] = "Your ";
    constexpr char g_mom[]  = "Mom!\n";

    constexpr auto g_yourMom = StringCat<g_your, g_mom>::value;
*/

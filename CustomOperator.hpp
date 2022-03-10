#ifndef USER_DEFINED_OPERATORS_HPP
#define USER_DEFINED_OPERATORS_HPP

#include <cstdint>
#include <concepts>
#include <iostream>

/*
Example Usage:
    using ws::Divisible;
    
    if (4 <Divisible> 2) { ... }
 */

namespace ws
{
    struct Divisible_ { } Divisible;

    template <std::integral T> struct DivisibleProxy
    {
        constexpr DivisibleProxy(std::integral auto value) : m_value(value) { }

        constexpr T get_value() const
        {
            return m_value;
        }

    private:
        T m_value;
    };

    inline auto constexpr operator<(std::integral auto lhs, const Divisible_&)
    {
        return DivisibleProxy<decltype(lhs)>(lhs);
    }

    inline bool constexpr operator>(const DivisibleProxy<std::int32_t>& lhs, std::int32_t rhs)
    {
        return !(lhs.get_value() % rhs);
    }
}

#endif

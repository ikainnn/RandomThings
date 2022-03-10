#include <functional>
#include <iostream>
#include <tuple>
#include <typeinfo>

/*
Example Usage:
    auto _f = ws::bind(func, args...);
    _f(...);
*/

namespace ws
{
    template <class _Signature> struct FunctionTraits;
    template <class _ReturnType, class... _ArgumentTypes> 
    struct FunctionTraits<_ReturnType(_ArgumentTypes...)>
    {
        using ReturnType = _ReturnType;

        template <std::size_t _Index> struct TypeAt
        {
            using Type = std::tuple_element<_Index, std::tuple<_ArgumentTypes...>>::type;
        };

        constexpr static std::size_t argumentsCount = sizeof...(_ArgumentTypes);
    };

    template <class T> using RemoveConstReference = std::remove_const_t<std::remove_reference_t<T>>;

    template <class _Function, std::size_t _Index> 
    inline constexpr void is_same_or_placeholder(const auto& arguments) noexcept
    {
        using FuncArgumentType = FunctionTraits<_Function>::template TypeAt<_Index>::Type;
        using BindArgumentType = RemoveConstReference<decltype(std::get<_Index>(arguments))>;
        
        static_assert((std::is_constructible_v<FuncArgumentType, BindArgumentType> ||
                       std::is_convertible_v<FuncArgumentType, BindArgumentType>   ||
                       std::is_placeholder_v<BindArgumentType>), "Mismatching parameter type.");
    }

    template <class _Function, class... _ArgumentTypes>
    inline constexpr auto bind(const _Function& func, const _ArgumentTypes&... arguments)
    {
        static_assert(FunctionTraits<_Function>::argumentsCount >= sizeof...(arguments), "Bind exceeds function's arguments size.");
        static_assert(FunctionTraits<_Function>::argumentsCount && sizeof...(arguments), "Function doesn't take 0 arguments.");

        /*        cppcheck moment        */
        /* cppcheck-suppress syntaxError */
        auto checkTypes = [_arguments = std::make_tuple(arguments...)]<std::size_t... _Indexes>(std::index_sequence<_Indexes...>)
        {
            (is_same_or_placeholder<_Function, _Indexes>(_arguments), ...);
        };

        checkTypes(std::make_index_sequence<sizeof...(arguments)>{ });

        return std::bind(func, arguments...);
    }
}
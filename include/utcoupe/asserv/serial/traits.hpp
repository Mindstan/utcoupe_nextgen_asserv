#ifndef UTCOUPE_ASSERV_SERIAL_TRAITS_HPP
#define UTCOUPE_ASSERV_SERIAL_TRAITS_HPP

#include <charconv>
#include <concepts>
#include <type_traits>

namespace utcoupe::asserv::serial {
    template<typename T>
    concept CanIntantiateFromChars = requires(T a, const char* first, const char* last) {
        { std::from_chars(first, last, a) };
    };
    
    
    
    template<typename T>
    // TODO Maybe add char as available type
    concept Deserializable = std::integral<T> || std::floating_point<T>;
} // namespace utcoupe::asserv::serial

#endif // UTCOUPE_ASSERV_SERIAL_TRAITS_HPP

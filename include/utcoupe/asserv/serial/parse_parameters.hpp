#ifndef UTCOUPE_ASSERV_SERIAL_PARSE_PARAMETERS_HPP
#define UTCOUPE_ASSERV_SERIAL_PARSE_PARAMETERS_HPP

#include "utcoupe/asserv/serial/traits.hpp"

#include <charconv>
#include <optional>
#include <tuple>

namespace utcoupe::asserv::serial {
    /**
     * Helper when std::from_chars for current type is not available.
     * 
     * @TODO handle floating values
     * @TODO maybe assert error
     */
    template<typename CurArgT, typename... OtherArgsT>
    std::optional<std::tuple<CurArgT, OtherArgsT...>> parseParameters([[maybe_unused]] const char* first, [[maybe_unused]] const char* last) noexcept {
        return std::nullopt;
    }
    
    
    /**
     * Parse numbers given in a string, and returns filled arguments in a tuple.
     * 
     * Compatible types are all integer types, float, double, long double. Floating point version may not be implemented in your compiler.
     * Parameters must be separed by a character like ' ', ';', ..., that can't be interpreted as part of a number. See https://en.cppreference.com/w/cpp/utility/from_chars for more details
     * If the number of parameters is smaller than the number of arguments to fill, it will return std::nullopt.
     * 
     * @param first Address of first character of the string
     * @param last Address + 1 of the last character of the string
     * @return A tuple containing values for all given argument types, in the same order, or std::nullopt if it has failed
     */
    template<typename CurArgT, typename... OtherArgsT>
    requires CanIntantiateFromChars<CurArgT>
    std::optional<std::tuple<CurArgT, OtherArgsT...>> parseParameters(const char* first, const char* last) noexcept {
        if (first + 1 >= last) {
            return std::nullopt;
        }
        CurArgT val;
        auto [nextFirst, ec] = std::from_chars(first, last, val);
        if (ec != std::errc{}) {
            return std::nullopt;
        }
        
        if constexpr (sizeof ...(OtherArgsT) == 0) {
            // TODO maybe warn unused parameters? (if nextFirst + 1 < last)
            return std::tuple{ val };
        } else {
            auto otherValues = parseParameters<OtherArgsT...>(nextFirst + 1, last);
            if (!otherValues) {
                return std::nullopt;
            }
            return std::tuple_cat(std::tuple{ val }, otherValues.value());
        }
    }
    
    /**
     * Deserializes values contained in the string.
     * 
     * This function does handle empty tuples (tuple without values).
     * @param str The string containing the serialized values.
     * @param values The tuple to set deserialized values.
     * @return True if it succeeded, false else.
     */
    template<typename... ArgsT>
    bool parseAndSetParameters(std::string_view str, std::tuple<ArgsT...>& values) {
        if constexpr (sizeof ...(ArgsT) == 0) {
            values = {};
            return true;
        } else {
            auto results = parseParameters<ArgsT...>(
                str.data(), str.data() + str.size()
            );
            
            if (!results) {
                return false;
            }
            
            values = *results;
            return true;
        }
    }
} // namespace utcoupe::asserv::serial

#endif // UTCOUPE_ASSERV_SERIAL_PARSE_PARAMETERS_HPP

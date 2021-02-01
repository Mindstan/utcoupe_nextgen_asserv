#ifndef UTCOUPE_ASSERV_SERIAL_PARSE_PARAMETERS_HPP
#define UTCOUPE_ASSERV_SERIAL_PARSE_PARAMETERS_HPP

#include "utcoupe/asserv/serial/traits.hpp"

#include <charconv>
#include <cstdlib>
#include <concepts>
#include <optional>
#include <tuple>

namespace utcoupe::asserv::serial {
    namespace impl {
        /**
         * Helper when std::from_chars for floating-point types is not available.
         * 
         * It internaly uses std::strtof/strtod/strtold, which are not as fast as std::from_chars<double>(...). It also does NOT CHECK boundaries, the string MUST either ends with '\0' or contain a char that isn't a part of a floating point value string representation.
         * @param first The address of the first char of the string
         * @param value The variable to set in case of success, or remain untouched
         * @return The address +1 of the last parsed char, or std::nullopt in case of error
         */
        template<std::floating_point ArgT>
        std::optional<const char *> from_chars_fp(const char* first, ArgT& value) {
            char* last = nullptr;
            ArgT result;
            
            // std::remove_cv_t is not be needed
            if constexpr (std::is_same_v<ArgT, float>) {
                result = std::strtof(first, &last);
            } else if constexpr (std::is_same_v<ArgT, double>) {
                result = std::strtod(first, &last);
            } else /* if constexpr (std::is_same_v<ArgT, long double>) */ {
                result = std::strtold(first, &last);
            }
            
            // std::strtof/strtod/strtold sets the value of last to first if it failed to parse.
            if (last == first) {
                return std::nullopt;
            }
            value = result;
            return last;
        }
    } // namespace impl
    
    /**
     * Helper when std::from_chars for current type is not available.
     * 
     * @TODO maybe assert error at compile time
     * 
     * @param first A pointer to the first char of the string
     * @param last A pointer to the last char of the string
     */
    template<typename CurArgT, typename... OtherArgsT>
    std::optional<std::tuple<CurArgT, OtherArgsT...>> parseParameters([[maybe_unused]] const char* first, [[maybe_unused]] const char* last) noexcept {
        return std::nullopt;
    }
    
    /**
     * Helper when std::from_chars for floating-point types is not available.
     * 
     * @TODO 
     * 
     * @param first A pointer to the first char of the string
     * @param last A pointer to the last char of the string
     */
    template<std::floating_point CurArgT, typename... OtherArgsT>
    requires (not CanIntantiateFromChars<CurArgT>)
    std::optional<std::tuple<float, OtherArgsT...>> parseParameters(const char* first, const char* last) noexcept {
        if (first + 1 >= last) {
            return std::nullopt;
        }
        CurArgT val;
        auto nextFirstOpt = impl::from_chars_fp(first, val);
        if (!nextFirstOpt) {
            return std::nullopt;
        }
        
        // Same as general parseParameters
        if constexpr (sizeof ...(OtherArgsT) == 0) {
            // TODO maybe warn unused parameters? (if nextFirst + 1 < last)
            return std::tuple{ val };
        } else {
            auto otherValues = parseParameters<OtherArgsT...>(*nextFirstOpt + 1, last);
            if (!otherValues) {
                return std::nullopt;
            }
            return std::tuple_cat(std::tuple{ val }, otherValues.value());
        }
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

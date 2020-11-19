#ifndef UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HXX
#define UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HXX

#include "utcoupe/asserv/serial/protocol.hpp"

#include <charconv>
#include <concepts>
#include <functional>
#include <optional>
#include <utility>

namespace utcoupe::asserv::serial {
    
    namespace impl {
        template<typename T>
        concept CanIntantiateFromChars = requires(T a, const char* first, const char* last) {
            { std::from_chars(first, last, a) };
        };
    } // namespace impl
    
    /**
     * Parse a number given in a string, and returns the parsed value.
     * 
     * Compatible types are all integer types, float, double, long double.
     * If it fails to parse, it will return std::nullopt.
     * 
     * @param first Address of first character of the string
     * @param last Address + 1 of the last character of the string
     * @return The parsed value
     */
    template<typename ArgT>
    requires impl::CanIntantiateFromChars<ArgT>
    std::optional<ArgT> parseParameters(const char* first, const char* last) noexcept {
        ArgT val;
        auto result = std::from_chars(first, last, val);
        if (result.ec != std::errc{}) {
            return std::nullopt;
        }
        return val;
    }
    
    /**
     * Helper when std::from_chars for current type is not available.
     * 
     * @TODO handle floating values
     * @TODO maybe assert error
     */
    template<typename ArgT>
    std::optional<ArgT> parseParameters([[maybe_unused]] const char* first, [[maybe_unused]] const char* last) noexcept {
        return std::nullopt;
    }
    
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
     * Compatible types are all integer types, float, double, long double.
     * Parameters must be separed by a character like ' ', ';', ..., that can't be interpreted as part of a number. See https://en.cppreference.com/w/cpp/utility/from_chars for more details
     * If the number of parameters is smaller than the number of arguments to fill, it will return std::nullopt.
     * 
     * @param first Address of first character of the string
     * @param last Address + 1 of the last character of the string
     * @return A tuple containing values for all given argument types, in the same order, or std::nullopt if it has failed
     */
    template<typename CurArgT, typename... OtherArgsT>
    requires impl::CanIntantiateFromChars<CurArgT>
    std::optional<std::tuple<CurArgT, OtherArgsT...>> parseParameters(const char* first, const char* last) noexcept {
        CurArgT val;
        auto [nextFirst, ec] = std::from_chars(first, last, val);
        if (ec != std::errc{} || nextFirst + 1 >= last) {
            return std::nullopt;
        }
        
        auto otherValues = parseParameters<OtherArgsT...>(nextFirst + 1, last);
        if (!otherValues) {
            return std::nullopt;
        }
        return std::tuple_cat(std::tuple{ val }, otherValues.value());
    }
    
    template<typename... ArgsT>
    bool parseThenCall(std::string_view msgParams, std::function<void(ArgsT...)> callback) noexcept {
        auto argumentValues = parseParameters<ArgsT...>(msgParams.data(), msgParams.data() + msgParams.size());
        if (!argumentValues) {
            return false;
        }
        std::apply(callback, argumentValues.value());
        return true;
    }
    
    template<class ObjectT, typename... ArgsT>
    bool parseThenCall(std::string_view msgParams, ObjectT& obj, void(ObjectT::* callback)(ArgsT...)) noexcept {
        auto argumentValues = parseParameters<ArgsT...>(msgParams.data(), msgParams.data() + msgParams.size());
        if (!argumentValues) {
            return false;
        }
        auto fun = std::mem_fn(callback);
        std::apply(
            [&](auto&&... args){ fun(obj, std::forward<decltype(args)>(args)...); },
            argumentValues.value()
        );
        return true;
    }
    
    template<TasksDispatcherLike TTasksDispatcher>
    bool parseTask(std::string_view msg, TTasksDispatcher& tasksDispatcher) {
        using namespace std::placeholders;
        
        switch(static_cast<TaskTypes>(msg.at(0))) {
        case TaskTypes::CLEAN_GOALS:
            tasksDispatcher.cleanGoals();
            break;
            
        case TaskTypes::GET_CODER:
            tasksDispatcher.getCoder();
            break;
        
        case TaskTypes::GET_LAST_ID:
            tasksDispatcher.getLastID();
            break;
            
        case TaskTypes::GET_POS:
            tasksDispatcher.getPos();
            break;
        
        case TaskTypes::GET_POS_ID:
            tasksDispatcher.getPosID();
            break;
            
        case TaskTypes::GET_SPD:
            tasksDispatcher.getSpeed();
            break;
        
        case TaskTypes::GET_TARGET_SPD:
            tasksDispatcher.getTargetSpeed();
            break;
            
        case TaskTypes::GOTO_WITH_ANGLE:
            if (msg.size() > 2) {
                return parseThenCall(
                    msg.substr(2, msg.size() - 2),
                    tasksDispatcher,
                    &TTasksDispatcher::gotoWithAngle
                );
            }
            return false;
        
        case TaskTypes::HALT:
            tasksDispatcher.halt();
            break;
        
        case TaskTypes::KILL_GOAL:
            tasksDispatcher.killGoal();
            break;
        
        case TaskTypes::PAUSE:
            tasksDispatcher.pause();
            break;
        
        case TaskTypes::RESET_ID:
            tasksDispatcher.resetID();
            break;
            
        case TaskTypes::RESUME:
            tasksDispatcher.resume();
            break;
        
        case TaskTypes::START:
            tasksDispatcher.start();
            break;
        
        case TaskTypes::WHO_AMI:
            tasksDispatcher.whoAmi();
            break;
            
        default:
            return false;
        }

        return true;
    }
    
} // namespace utcoupe::serial

#endif // UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HXX

#ifndef UTCOUPE_ASSERV_SERIAL_ORDER_HPP
#define UTCOUPE_ASSERV_SERIAL_ORDER_HPP

#include "utcoupe/asserv/serial/traits.hpp"

#include <concepts>
#include <functional>

namespace utcoupe::asserv::serial {
    /**
     * Represents a reference to an order and its associated callback.
     */
    template<class CallbackT, Deserializable... CallbackArgsT>
    struct Order {
        /** Alias representing all parameter types of the order. */
        using CallbackArgs = std::tuple<CallbackArgsT...>;
        
        /** The character that will be use to identify the order inside a string message. */
        char ch_order;
        
        /**
         * A pointer on a Executor member function that will be called with the order parameters.
         * 
         * See createOrder for more explanation.
         */
        CallbackT callback;
    };
    
    /**
     * Instanciates and returns a new Order struct.
     * 
     * By using a member function pointer from Executor class, this helper is able to automaticaly deduce all parameter types only and only if there are no ambiguity on the function (e.g., if there are'nt any two member functions having the same name, no matter their arguments).
     * 
     * @param chOrder The character identifying the order.
     * @param callback A pointer to member function that will be called when this order will be successfully dessirialized.
     * @return An initialized Order.
     */
    template<typename CbReturnT, typename Executor, typename... CallbackArgsT>
    constexpr auto createOrder(char chOrder, CbReturnT(Executor::* callback)(CallbackArgsT...)) noexcept {
        return Order<CbReturnT(Executor::*)(CallbackArgsT...), CallbackArgsT...>{chOrder, callback};
    }
    
    /**
     * Calls the order's callback with arguments contained in a tuple.
     * 
     * @param executor The class from which is the order's callback from.
     * @param order An Order object containing the callback.
     * @param values A tuple containing the callback argument values.
     * @return The value returned by the callback.
     */
    template <class Executor, class OrderT>
    constexpr auto executeOrder(Executor& executor, OrderT& order, typename OrderT::CallbackArgs&& values) noexcept {
        return std::apply(
            [&executor, &order](auto&&... argValues){
                return std::invoke(order.callback, executor, std::forward<decltype(argValues)>(argValues)...);
            },
            values
        );
    }
} // namespace utcoupe::serial

#endif // UTCOUPE_ASSERV_SERIAL_ORDER_HPP

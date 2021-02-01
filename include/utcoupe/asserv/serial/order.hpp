#ifndef UTCOUPE_ASSERV_SERIAL_ORDER_HPP
#define UTCOUPE_ASSERV_SERIAL_ORDER_HPP

#include "utcoupe/asserv/serial/traits.hpp"

#include <concepts>
#include <functional>

namespace utcoupe::asserv::serial {    
    template<class CallbackT, Deserializable... CallbackArgsT>
    struct Order {
        using CallbackArgs = std::tuple<CallbackArgsT...>;
        
        char ch_order;
        CallbackT (callback);
    };
    
    template<typename CbReturnT, typename Executor, typename... CallbackArgsT>
    constexpr auto createOrder(char chOrder,CbReturnT(Executor::* callback)(CallbackArgsT...)) noexcept {
        return Order<CbReturnT(Executor::*)(CallbackArgsT...), CallbackArgsT...>{chOrder, callback};
    }
    
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

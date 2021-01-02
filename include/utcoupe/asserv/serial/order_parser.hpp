#ifndef UTCOUPE_ASSERV_SERIAL_ORDER_PARSER_HPP
#define UTCOUPE_ASSERV_SERIAL_ORDER_PARSER_HPP

#include <optional>
#include <string_view>
#include <tuple>

namespace utcoupe::asserv::serial {
    template<class Executor>
    class OrderParser {
    public:
        OrderParser(Executor& executor): m_executor(executor) {}
        
        template<class... OrdersT>
        constexpr std::optional<typename Executor::OrderReturnT> parseAndRunOrder(const std::tuple<OrdersT...>& orders, std::string_view serializedOrder) noexcept {
            auto correspondingOrderId = selectCorrespondingOrder<0>(serializedOrder.at(0), orders);
            if (!correspondingOrderId) {
                return std::nullopt;
            }
            // TODO
            return 0;
        }
        
    private:
        Executor& m_executor;
        
        template<std::size_t idx, class... OrdersT>
        constexpr std::optional<std::size_t> selectCorrespondingOrder(char chOrder, const std::tuple<OrdersT...>& orders) noexcept {
            if constexpr (idx >= std::tuple_size<typename std::tuple<OrdersT...>>::value) {
                return std::nullopt;
            } else {
                if (std::get<idx>(orders).ch_order == chOrder) {
                    return idx;
                } else {
                    return selectCorrespondingOrder<idx+1, OrdersT...>(chOrder, orders);
                }
            }
        }
    };
} // namespace utcoupe::asserv::serial

#endif // UTCOUPE_ASSERV_SERIAL_ORDER_PARSER_HPP

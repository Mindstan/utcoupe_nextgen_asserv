#ifndef UTCOUPE_ASSERV_SERIAL_ORDER_PARSER_HPP
#define UTCOUPE_ASSERV_SERIAL_ORDER_PARSER_HPP

#include "utcoupe/asserv/serial/parse_parameters.hpp"

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
            return selectAndRunOrder<0>(serializedOrder, orders);
        }
        
    private:
        Executor& m_executor;
        
        template<std::size_t idx, class... OrdersT>
        constexpr std::optional<typename Executor::OrderReturnT> selectAndRunOrder(std::string_view serializedOrder, const std::tuple<OrdersT...>& orders) noexcept {
            if constexpr (idx >= std::tuple_size<typename std::tuple<OrdersT...>>::value) {
                return std::nullopt;
            } else {
                auto& order = std::get<idx>(orders);
                if (order.ch_order == serializedOrder.at(0)) {
                    return runOrder(serializedOrder, order);
                } else {
                    return selectAndRunOrder<idx+1, OrdersT...>(serializedOrder, orders);
                }
            }
        }
        
        template<class OrderT>
        std::optional<typename Executor::OrderReturnT> runOrder(std::string_view serializedOrder, OrderT& order) {
            typename OrderT::CallbackArgs values;
            bool success = parseAndSetParameters(
                serializedOrder.substr(2, serializedOrder.size()),
                values
            );
            
            if (!success) {
                return std::nullopt;
            }
            
            return executeOrder(m_executor, order, std::move(values));
        }
        
    };
} // namespace utcoupe::asserv::serial

#endif // UTCOUPE_ASSERV_SERIAL_ORDER_PARSER_HPP

#include <boost/ut.hpp>

#include "utcoupe/asserv/serial/order.hpp"
#include "utcoupe/asserv/serial/order_parser.hpp"

#include <cmath>
#include <string_view>

using namespace boost::ut;
using namespace boost::ut::bdd;
using namespace utcoupe::asserv;

struct Tester {
    using OrderReturnT = int;
    
    constexpr int run1(int x) { return x; }
    constexpr int run2(float x) { return static_cast<int>(std::ceil(x)); }
    constexpr int compute1() { return 42; }
    constexpr int compute2() { return 43; }
};

suite orderParser = [] {
    tag ("serial") / tag ("serial-parser") /
    feature ("serial::OrderParser") = [] {
        Tester tester;
        serial::OrderParser parser{tester};
        
        scenario("Order exists") = [&] {
            auto allOrders = std::tuple{
                serial::createOrder('a', &Tester::run1),
                serial::createOrder('b', &Tester::run2),
                serial::createOrder('c', &Tester::compute1),
                serial::createOrder('d', &Tester::compute2)
            };
        
            given ("An existing order correctly formed") = [&] {
                std::string_view orderStr1 = "a;5;";
                std::string_view orderStr2 = "c;";
                
                then ("It should be executable")  = [&] {
                    expect (parser.parseAndRunOrder(allOrders, orderStr1).has_value());
                    expect (parser.parseAndRunOrder(allOrders, orderStr2).has_value());
                };
            };
            
            given ("An unknown order") = [&] {
                std::string_view orderStr = "x;";
                
                then ("It shouldn't be executable") = [&] {
                    expect (!parser.parseAndRunOrder(allOrders, orderStr));
                };
            };
        };
    };
};

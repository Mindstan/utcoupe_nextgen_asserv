#include <boost/ut.hpp>

#include "utcoupe/asserv/serial/order.hpp"

using namespace boost::ut;
using namespace boost::ut::bdd;
using namespace utcoupe::asserv;

struct Tester {
    constexpr int run(int x) { return x; }
    constexpr int compute() { return 42; }
};

suite orders = [] {
    tag("serial") / tag("order") /
    feature ("serial::Order") = [] {
        scenario ("Callback invocation") = [] {
            given ("An order without any arguments") = [] {
                Tester tester;
                
                auto order { serial::createOrder('c', &Tester::compute) };
                
                then ("Executing the order without parameters should work") = [&tester, &order] {
                    expect( serial::executeOrder(tester, order, {}) == 42_i);
                };
            };
            
            given ("An order with one int argument") = [] {
                Tester tester;
                
                auto order { serial::createOrder('c', &Tester::run) };
                
                then ("Executing the order with a int should work") = [&tester, &order] {
                    expect( serial::executeOrder(tester, order, {9}) == 9_i );
                };
            };
        };
    };
}; // This is a lambda function!

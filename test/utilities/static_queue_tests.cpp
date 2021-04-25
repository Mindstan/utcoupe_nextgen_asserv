#include <boost/ut.hpp>

#include "utcoupe/asserv/utilities/static_queue.hpp"

using namespace boost::ut;
using namespace boost::ut::bdd;
using namespace utcoupe::asserv;

suite staticQueue = [] {
    tag("utilities") / tag("static-queue") /
    feature ("StaticQueue") = [] {
        scenario("Simple use with one element") = [] {
            given("An empty int queue") = [] {
                const int VALUE1 = 42;
                
                const std::size_t QUEUE_CAPACITY = 10;
                utilities::StaticQueue<int, QUEUE_CAPACITY> sq{};
                
                then ("It is empty") = [&] {
                    expect(sq.size() == 0_u);
                    expect(sq.empty());
                };
                
                when ("We add an element by copy") = [&] {
                    
                    sq.push(VALUE1);
                    
                    then ("The size has grown") = [&] {
                        expect(sq.size() == 1_u);
                        expect(!sq.empty());
                    };
                    
                    then ("We can access it through front()") = [&]  {
                        auto* res = sq.front();
                        expect(that % res != nullptr) << "It should not be null";
                        expect(that % *res == VALUE1) << "It should have the correct value";
                    };
                    
                    then ("We can access it through back()") = [&]  {
                        auto* res = sq.back();
                        expect(that % res != nullptr) << "It should not be null";
                        expect(that % *res == VALUE1) << "It should have the correct value";
                    };
                };
                
                when ("We pop the element") = [&] {
                    auto res = sq.pop();
                    
                    then("The returned element has the correct value") = [&] {
                        expect(res.has_value());
                        expect(that % *res == VALUE1);
                    };
                    
                    then("The queue is now empty") = [&] {
                        expect(sq.size() == 0_u);
                        expect(sq.empty());
                    };
                };
            };
        };
    };
};

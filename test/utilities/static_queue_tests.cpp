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
                
                then ("We cannot obtain elements from front or back") = [&] {
                    expect(sq.front() == std::nullopt);
                    expect(sq.back() == std::nullopt);
                };
                
                when ("We add an element by copy") = [&] {
                    
                    sq.push(VALUE1);
                    
                    then ("The size has grown") = [&] {
                        expect(sq.size() == 1_u);
                        expect(!sq.empty());
                    };
                    
                    then ("We can access it through front()") = [&]  {
                        auto res = sq.front();
                        expect(res != std::nullopt) << "It should not be null";
                        expect(that % *res == VALUE1) << "It should have the correct value";
                    };
                    
                    then ("We can access it through back()") = [&]  {
                        auto res = sq.back();
                        expect(res != std::nullopt) << "It should not be null";
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
    
    tag("utilities") / tag("static-queue") /
    scenario("Simple use with multiple element") = [] {
        given("An empty int queue") = [] {
            const std::array VALUES {1, 1, 2, 3, 5, 8};
            
            const std::size_t QUEUE_CAPACITY = 10;
            utilities::StaticQueue<int, QUEUE_CAPACITY> sq{};
            
            then ("It is empty") = [&] {
                expect(sq.size() == 0_u);
                expect(sq.empty());
            };
            
            when("Values are pushed one-by-one") = [&] {
                for (std::size_t idx = 0; idx < VALUES.size(); idx++) {
                    expect(sq.push(VALUES[idx])) << "A value can be added";
                    expect(that % sq.size() == idx + 1) << "The size has been updated";
                    expect(sq.front() != std::nullopt);
                    expect(that % *sq.front() == VALUES[0]) << "Front is always the first element";
                    expect(sq.back() != std::nullopt);
                    expect(that % *sq.back() == VALUES[idx]) << "Back is always the last element";
                }
            };
            
            when ("Values are poped one-by-one") = [&] {
                for (std::size_t idx = 0; idx + 1 < VALUES.size(); idx++) {
                    auto res = sq.pop();
                    
                    expect(res.has_value()) << "A value can be poped";
                    expect(that % *res == VALUES[idx]) << "The correct value has been poped";
                    expect(that % sq.size() == VALUES.size() - (idx + 1)) << "The size has been updated";
                    expect(sq.front() != std::nullopt);
                    expect(that % *sq.front() == VALUES[idx+1]) << "Front is always the first element";
                    expect(sq.back() != std::nullopt);
                    expect(that % *sq.back() == VALUES[VALUES.size() - 1]) << "Back is always the last element";
                }
                
                auto res = sq.pop();
                
                expect(res.has_value()) << "A value can be poped";
                expect(that % *res == VALUES[VALUES.size() - 1]) << "The correct value has been poped";
                
                
                then ("The queue is now empty") = [&] {
                    expect(sq.empty());
                };
            };
            
            when ("The queue is full") = [&] {
                for (int i = 1; i <= static_cast<int>(sq.capacity()); i++) {
                    expect(sq.push(i));
                }
                
                expect(that % sq.size() == sq.capacity()) << "Its size is its capacity";
                expect(*sq.front() == 1_i);
                expect(that % *sq.back() == static_cast<int>(sq.capacity()));
                
                then ("We cannot add more elements") = [&] {
                    expect(!sq.push(0));
                    expect(*sq.front() == 1_i);
                    expect(that % *sq.back() == static_cast<int>(sq.capacity()));
                };
                
                then ("We can add an element after poping another") = [&] {
                    expect(sq.pop().has_value());
                    expect(*sq.front() == 2_i);
                    expect(sq.push(0));
                    expect(*sq.front() == 2_i);
                    expect(sq.back().has_value());
                    expect(*sq.back() == 0_i);
                };
            };
        };
    };
};

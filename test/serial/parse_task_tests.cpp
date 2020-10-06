#include <catch2/catch.hpp>

#include "utcoupe/asserv/serial/parse_task.hpp"

#include <array>
#include <string_view>

using namespace utcoupe::asserv;

class DummyDispatcher {
public:
    DummyDispatcher() {
        resetCallCounters();
    }
    
    enum class ActionType {
        WHOAMI = 0,
        _ENUM_LAST // sentinel
    };
    
    constexpr unsigned getNumberOfCalls(ActionType action) {
        return m_callCounters.at(static_cast<std::size_t>(action));
    }
    
    constexpr void resetCallCounters() {
        m_callCounters.fill(0);
    }
    
    // Requirements of the concept
    
    void doWhoAmi() { incrementCallCounter(ActionType::WHOAMI); }
    
    private:
        // Represent the number of possible values in ActionType, but _ENUM_LAST
        static const auto ENUM_ActionType_SIZE { static_cast<std::size_t>(ActionType::_ENUM_LAST) };
        std::array<unsigned, ENUM_ActionType_SIZE> m_callCounters;
        
        constexpr void incrementCallCounter(ActionType action) {
            ++ m_callCounters.at(static_cast<std::size_t>(action));
        }
};

SCENARIO("Tasks parsing", "[serial][order") {
    using namespace std::string_view_literals;
    GIVEN("A dummy implementation of task dispatcher") {
        DummyDispatcher dummyDisp;
        
        WHEN("A WHO_AMI message is parsed") {
            REQUIRE(serial::parseTask("w;", dummyDisp));
            
            THEN("The dispatcher function doWhoAmi is called") {
                REQUIRE(dummyDisp.getNumberOfCalls(DummyDispatcher::ActionType::WHOAMI) == 1);
            }
        }
    }
}

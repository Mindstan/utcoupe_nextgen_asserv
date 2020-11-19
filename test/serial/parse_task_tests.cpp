#include <catch2/catch.hpp>

#include "utcoupe/asserv/serial/parse_task.hpp"
#include "utcoupe/asserv/serial/protocol.hpp"

#include <array>
#include <iostream>
#include <string_view>
#include <utility>

using namespace utcoupe::asserv;

class DummyDispatcher {
public:
    DummyDispatcher() {
        resetAll();
    }
    
    enum class ActionType {
        CLEAN_GOALS = 0,
        GET_CODER,
        GET_LAST_ID,
        GET_POS,
        GET_POS_ID,
        GET_SPD,
        GET_TARGET_SPD,
        GOTO_WITH_ANGLE,
        HALT,
        KILL_GOAL,
        PAUSE,
        RESET_ID,
        RESUME,
        // SET_EMERGENCY_STOP,
        START,
        WHOAMI,
        _ENUM_LAST // sentinel
    };
    
    constexpr unsigned getNumberOfCalls(ActionType action) {
        return m_callCounters.at(static_cast<std::size_t>(action));
    }
    
    void resetAll() {
        resetCallCounters();
        resetSentinels();
    }
    
    constexpr void resetCallCounters() {
        m_callCounters.fill(0);
    }
    
    void resetSentinels() {
        gotoWithAngleSentinel = nullptr;
    }
    
    // Requirements of the concept
    
    void cleanGoals() { incrementCallCounter(ActionType::CLEAN_GOALS); }
    
    void getCoder() { incrementCallCounter(ActionType::GET_CODER); }
    
    void getLastID() { incrementCallCounter(ActionType::GET_LAST_ID); }
    
    void getPos() { incrementCallCounter(ActionType::GET_POS); }
    
    void getPosID() { incrementCallCounter(ActionType::GET_POS_ID); }
    
    void getSpeed() { incrementCallCounter(ActionType::GET_SPD); }
    
    void getTargetSpeed() { incrementCallCounter(ActionType::GET_TARGET_SPD); }
    
    std::function<void(int, int, float, int)> gotoWithAngleSentinel;
    void gotoWithAngle(int x, int y, float angle, int direction) {
        incrementCallCounter(ActionType::GOTO_WITH_ANGLE);
        if (gotoWithAngleSentinel) {
            gotoWithAngleSentinel(x, y, angle, direction);
        }
    }
    
    void halt() { incrementCallCounter(ActionType::HALT); }
    
    void killGoal() { incrementCallCounter(ActionType::KILL_GOAL); }
    
    void pause() { incrementCallCounter(ActionType::PAUSE); }
    
    void resetID() { incrementCallCounter(ActionType::RESET_ID); }
    
    void resume() { incrementCallCounter(ActionType::RESUME); }
    
    void start() { incrementCallCounter(ActionType::START); }
    
    void whoAmi() { incrementCallCounter(ActionType::WHOAMI); }
    
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
        
        // redundant with next section
        WHEN("A WHO_AMI message is parsed") {
            dummyDisp.resetCallCounters();
            REQUIRE(serial::parseTask("w;", dummyDisp));
            
            THEN("The dispatcher function doWhoAmi is called") {
                REQUIRE(dummyDisp.getNumberOfCalls(DummyDispatcher::ActionType::WHOAMI) == 1);
            }
        }
        
        WHEN("A task without arguments is parsed") {
            dummyDisp.resetCallCounters();
            
            std::array<std::pair<serial::TaskTypes, DummyDispatcher::ActionType>, 14> tasks{{
                { serial::TaskTypes::CLEAN_GOALS, DummyDispatcher::ActionType::CLEAN_GOALS },
                { serial::TaskTypes::GET_CODER, DummyDispatcher::ActionType::GET_CODER },
                { serial::TaskTypes::GET_LAST_ID, DummyDispatcher::ActionType::GET_LAST_ID },
                { serial::TaskTypes::GET_POS, DummyDispatcher::ActionType::GET_POS },
                { serial::TaskTypes::GET_POS_ID, DummyDispatcher::ActionType::GET_POS_ID },
                { serial::TaskTypes::GET_SPD, DummyDispatcher::ActionType::GET_SPD },
                { serial::TaskTypes::GET_TARGET_SPD, DummyDispatcher::ActionType::GET_TARGET_SPD },
                { serial::TaskTypes::HALT, DummyDispatcher::ActionType::HALT },
                { serial::TaskTypes::KILL_GOAL, DummyDispatcher::ActionType::KILL_GOAL },
                { serial::TaskTypes::PAUSE, DummyDispatcher::ActionType::PAUSE },
                { serial::TaskTypes::RESET_ID, DummyDispatcher::ActionType::RESET_ID },
                { serial::TaskTypes::RESUME, DummyDispatcher::ActionType::RESUME },
                { serial::TaskTypes::START, DummyDispatcher::ActionType::START },
                { serial::TaskTypes::WHO_AMI, DummyDispatcher::ActionType::WHOAMI }
            }};
            
            for (auto task: tasks) {
                std::array<char, 3> msg {{static_cast<char>(task.first), ';', '\0'}};
                REQUIRE(serial::parseTask({msg.data(), msg.size()}, dummyDisp));
            
                THEN("The corresponding dispatcher function is called once") {
                    REQUIRE(dummyDisp.getNumberOfCalls(task.second) == 1);
                }
            }
        }
        
        WHEN("A task whith arguments is parsed") {
            dummyDisp.resetAll();
            
            auto msg = "c;1000;1000;1.571;1;"sv;
            
            bool good = false;
            auto gotoWithAngleSentinel = [&good](int x, int y, float angle, int direction) {
                good = (x == 1000 && y == 1000 && angle == 1.571f && direction == 1);
            };
            dummyDisp.gotoWithAngleSentinel = gotoWithAngleSentinel;
            REQUIRE(serial::parseTask(msg, dummyDisp));
            REQUIRE(good);
        }
    }
}

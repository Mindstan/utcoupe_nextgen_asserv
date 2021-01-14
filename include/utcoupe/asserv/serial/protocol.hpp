#ifndef UTCOUPE_ASSERV_SERIAL_PROTOCOL_HPP
#define UTCOUPE_ASSERV_SERIAL_PROTOCOL_HPP

#include "utcoupe/asserv/serial/order.hpp"

namespace utcoupe::asserv::serial {
    
    enum class OrderTypes : char {
        ACC_MAX = 'l',
        CLEAN_GOALS = 'g',
        GET_CODER = 'j',
        GET_LAST_ID = 't',
        GET_POS = 'n',
        GET_POS_ID = 'o',
        GET_SPD = 'y',
        GET_TARGET_SPD = 'v',
        GOTO = 'd',
        GOTO_WITH_ANGLE = 'c',
        HALT = 'H',
        KILL_GOAL = 'f',
        PAUSE = 'q',
        PID_ALL = 'u',
        PID_LEFT = 'p',
        PID_RIGHT = 'i',
        PING_PING = 'z',
        PWM = 'k',
        RESET_ID = 's',
        RESUME = 'r',
        ROT = 'e',
        ROT_MODULO = 'a',
        SET_EMERGENCY_STOP = 'A',
        SET_POS = 'm',
        SPD = 'b',
        SPD_MAX = 'x',
        START = 'S',
        WHO_AMI = 'w'
    };
    
    template<class Executor>
    // FIXME maybe consteval ?
    constexpr auto createStateMachineOrders() noexcept {
        return std::tuple{
            createOrder(OrderTypes::CLEAN_GOALS, &Executor::cleanGoals),
            createOrder(OrderTypes::GET_LAST_ID, &Executor::getLastID),
            createOrder(OrderTypes::HALT, &Executor::halt),
            createOrder(OrderTypes::KILL_GOAL, &Executor::killGoal),
            createOrder(OrderTypes::PAUSE, &Executor::pause),
            createOrder(OrderTypes::PING_PING, &Executor::pingPing),
            createOrder(OrderTypes::RESET_ID, &Executor::resetID),
            createOrder(OrderTypes::RESUME, &Executor::resume),
            createOrder(OrderTypes::START, &Executor::start),
            createOrder(OrderTypes::WHO_AMI, &Executor::whoAmi)
        };
    }
    
    template<class Executor>
    // FIXME maybe consteval ?
    constexpr auto createConfigOrders() noexcept {
        return std::tuple{
            createOrder(OrderTypes::ACC_MAX, &Executor::setMaxAcc),
            createOrder(OrderTypes::PID_ALL, &Executor::setAllPID),
            createOrder(OrderTypes::PID_LEFT, &Executor::setLeftPID),
            createOrder(OrderTypes::PID_RIGHT, &Executor::setRightPID),
            createOrder(OrderTypes::SPD_MAX, &Executor::setMaxSpeed)
        };
    }
    
    template<class Executor>
    // FIXME maybe consteval ?
    constexpr auto createMoveOrders() noexcept {
        return std::tuple{
            createOrder(OrderTypes::GET_CODER, &Executor::getCoder),
            createOrder(OrderTypes::GET_POS, &Executor::getPos),
            createOrder(OrderTypes::GET_POS_ID, &Executor::getPosID),
            createOrder(OrderTypes::GET_SPD, &Executor::getSpeed),
            createOrder(OrderTypes::GET_TARGET_SPD, &Executor::getTargetSpeed),
            createOrder(OrderTypes::GOTO, &Executor::doGoto),
            createOrder(OrderTypes::GOTO_WITH_ANGLE, &Executor::doGotoWithAngle),
            createOrder(OrderTypes::PWM, &Executor::setPWM),
            createOrder(OrderTypes::ROT, &Executor::doRotation),
            createOrder(OrderTypes::ROT_MODULO, &Executor::doRotationModulo),
            createOrder(OrderTypes::SET_EMERGENCY_STOP, &Executor::setEmergencyStop),
            createOrder(OrderTypes::SET_POS, &Executor::setPos),
            createOrder(OrderTypes::SPD, &Executor::setSpeed)
        };
    }
    
    template<class Executor>
    // FIXME maybe consteval ?
    constexpr auto createAllOrders() noexcept {
        return std::tuple_cat(
            createMoveOrders<Executor>(),
            createConfigOrders<Executor>(),
            createStateMachineOrders<Executor>()
        );
    }
} // namespace utcoupe::asserv::serial

#endif // UTCOUPE_ASSERV_SERIAL_PROTOCOL_HPP

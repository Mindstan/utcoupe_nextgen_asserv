#ifndef UTCOUPE_ASSERV_SERIAL_PROTOCOL_HPP
#define UTCOUPE_ASSERV_SERIAL_PROTOCOL_HPP

namespace utcoupe::asserv::serial {
    enum class TaskTypes {
        CLEAN_GOALS = 'g',
        GET_CODER = 'j',
        GET_LAST_ID = 't',
        GET_POS = 'n',
        GET_POS_ID = 'o',
        GET_SPD = 'y',
        GET_TARGET_SPD = 'v',
        GOTO_WITH_ANGLE = 'c',
        HALT = 'H',
        KILL_GOAL = 'f',
        PAUSE = 'q',
        RESET_ID = 's',
        RESUME = 'r',
        // SET_EMERGENCY_STOP = 'A',
        START = 'S',
        WHO_AMI = 'w'
    };
} // namespace utcoupe::asserv::serial

#endif // UTCOUPE_ASSERV_SERIAL_PROTOCOL_HPP

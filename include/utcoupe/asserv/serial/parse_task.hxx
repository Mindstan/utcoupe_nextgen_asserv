#ifndef UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HXX
#define UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HXX

#include "utcoupe/asserv/serial/protocol.hpp"

namespace utcoupe::asserv::serial {
    
    template<TasksDispatcherLike TTasksDispatcher>
    bool parseTask(std::string_view msg, TTasksDispatcher& tasksDispatcher) {
        switch(static_cast<TaskTypes>(msg.at(0))) {
        case TaskTypes::CLEAN_GOALS:
            tasksDispatcher.cleanGoals();
            break;
            
        case TaskTypes::GET_CODER:
            tasksDispatcher.getCoder();
            break;
        
        case TaskTypes::GET_LAST_ID:
            tasksDispatcher.getLastID();
            break;
            
        case TaskTypes::GET_POS:
            tasksDispatcher.getPos();
            break;
        
        case TaskTypes::GET_POS_ID:
            tasksDispatcher.getPosID();
            break;
            
        case TaskTypes::GET_SPD:
            tasksDispatcher.getSpeed();
            break;
        
        case TaskTypes::GET_TARGET_SPD:
            tasksDispatcher.getTargetSpeed();
            break;
        
        case TaskTypes::HALT:
            tasksDispatcher.halt();
            break;
        
        case TaskTypes::KILL_GOAL:
            tasksDispatcher.killGoal();
            break;
        
        case TaskTypes::PAUSE:
            tasksDispatcher.pause();
            break;
        
        case TaskTypes::RESET_ID:
            tasksDispatcher.resetID();
            break;
            
        case TaskTypes::RESUME:
            tasksDispatcher.resume();
            break;
        
        case TaskTypes::START:
            tasksDispatcher.start();
            break;
        
        case TaskTypes::WHO_AMI:
            tasksDispatcher.whoAmi();
            break;
            
        default:
            return false;
        }

        return true;
    }
    
} // namespace utcoupe::serial

#endif // UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HXX

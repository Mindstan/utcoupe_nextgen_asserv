#ifndef UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HXX
#define UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HXX

namespace utcoupe::asserv::serial {
    
    template<TasksDispatcherLike TTasksDispatcher>
    bool parseTask(std::string_view /*msg*/, TTasksDispatcher& tasksDispatcher) {
        tasksDispatcher.doWhoAmi();
        return true;
    }
    
} // namespace utcoupe::serial

#endif // UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HXX

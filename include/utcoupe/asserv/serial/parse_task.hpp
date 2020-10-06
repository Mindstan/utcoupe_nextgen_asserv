#ifndef UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HPP
#define UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HPP

#include "utcoupe/asserv/tasks_dispatcher_like.concept.hpp"

#include <string_view>

namespace utcoupe::asserv::serial {
    template<TasksDispatcherLike TTasksDispatcher>
    [[nodiscard("Returns true if it fails!")]] bool parseTask(std::string_view msg, TTasksDispatcher& tasksDispatcher);
} // namespace utcoupe::asserv::serial

#include "utcoupe/asserv/serial/parse_task.hxx"

#endif // UTCOUPE_ASSERV_SERIAL_PARSE_TASK_HPP

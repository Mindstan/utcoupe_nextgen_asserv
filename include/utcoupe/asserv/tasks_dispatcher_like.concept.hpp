#ifndef UTCOUPE_ASSERV_TASKS_DISPATCHER_LIKE_CONCEPT_HPP
#define UTCOUPE_ASSERV_TASKS_DISPATCHER_LIKE_CONCEPT_HPP

namespace utcoupe::asserv {
    template <typename TTasksDispatcherImpl>
    concept TasksDispatcherLike = requires (TTasksDispatcherImpl tasksDispatcher) {
        tasksDispatcher.doWhoAmi();
    };
} // namespace utcoupe::asserv

#endif // UTCOUPE_ASSERV_TASKS_DISPATCHER_LIKE_CONCEPT_HPP

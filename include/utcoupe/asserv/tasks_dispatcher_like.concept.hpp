#ifndef UTCOUPE_ASSERV_TASKS_DISPATCHER_LIKE_CONCEPT_HPP
#define UTCOUPE_ASSERV_TASKS_DISPATCHER_LIKE_CONCEPT_HPP

namespace utcoupe::asserv {
    template <typename TTasksDispatcherImpl>
    concept TasksDispatcherLike = requires (TTasksDispatcherImpl tasksDispatcher) {
        tasksDispatcher.cleanGoals();
        tasksDispatcher.getCoder();
        tasksDispatcher.getLastID();
        tasksDispatcher.getPos();
        tasksDispatcher.getPosID();
        tasksDispatcher.getSpeed();
        tasksDispatcher.getTargetSpeed();
        tasksDispatcher.halt();
        tasksDispatcher.killGoal();
        tasksDispatcher.pause();
        tasksDispatcher.resetID();
        tasksDispatcher.resume();
        tasksDispatcher.start();
        tasksDispatcher.whoAmi();
    };
} // namespace utcoupe::asserv

#endif // UTCOUPE_ASSERV_TASKS_DISPATCHER_LIKE_CONCEPT_HPP

#include "taskManager.hpp"
#include <chrono>

int main(){
    TaskScheduler scheduler(4); // Using 4 worker threads

    auto taskFunction = [](int id, TaskPriority priority) {
        std::cout << "Task " << id << " with priority " << priority
                  << " is running on thread " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    };

    std::vector<Task> tasks = {
        {1, 1, std::chrono::steady_clock::now() + std::chrono::seconds(2), std::bind(taskFunction, 1, 1)},
        {2, 3, std::chrono::steady_clock::now() + std::chrono::seconds(4), std::bind(taskFunction, 2, 3)},
        {3, 5, std::chrono::steady_clock::now() + std::chrono::seconds(6), std::bind(taskFunction, 3, 5)},
        {4, 2, std::chrono::steady_clock::now() + std::chrono::seconds(1), std::bind(taskFunction, 4, 2)},
        {5, 4, std::chrono::steady_clock::now() + std::chrono::seconds(3), std::bind(taskFunction, 5, 4)},
        {6, 6, std::chrono::steady_clock::now() + std::chrono::seconds(5), std::bind(taskFunction, 6, 6)}
    };

    scheduler.addTasks(tasks); // Adding tasks in batch

    // Waiting for specific tasks to complete
    scheduler.waitForTask(4);
    scheduler.waitForTask(1);

    // Displaying current task queue size
    std::cout << "Pending tasks: " << scheduler.getPendingTasksCount() << std::endl;

    // Test increasing and decreasing worker threads
    scheduler.increaseWorkerThreads(2);
    std::cout << "Worker threads increased: " << scheduler.getWorkerThreadsCount() << std::endl;

    scheduler.decreaseWorkerThreads(1);
    std::cout << "Worker threads decreased: " << scheduler.getWorkerThreadsCount() << std::endl;

    return 0;
}

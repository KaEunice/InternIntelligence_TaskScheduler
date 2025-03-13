#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <chrono>
#include <map>

typedef int TaskPriority;                          // Lower number = higher priority

struct Task {
    int id;
    TaskPriority priority;
    std::chrono::steady_clock::time_point deadline;
    std::function<void()> function;

    bool operator<(const Task& other) const {
        return priority > other.priority || (priority == other.priority && deadline > other.deadline);
    }
};

class TaskScheduler {
private:
    std::priority_queue<Task> taskQueue;
    std::vector<std::thread> workerThreads;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<bool> stopScheduler;
    std::map<int, bool> taskCompletion;
    
    void worker();

public:
    TaskScheduler(size_t numThreads);
    ~TaskScheduler();
    void addTask(Task task);
    void addTasks(const std::vector<Task>& tasks); // Batch task addition
    bool cancelTask(int taskId);                   // Cancel a task before execution
    size_t getPendingTasksCount();                 // Get number of tasks waiting
    size_t getWorkerThreadsCount();                // Get number of worker threads
    void increaseWorkerThreads(size_t num);        // Increase worker threads
    void decreaseWorkerThreads(size_t num);        // Decrease worker threads
    void waitForTask(int taskId);
};

#endif // TASKMANAGER_HPP

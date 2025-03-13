#include "taskManager.hpp"

TaskScheduler::TaskScheduler(size_t numThreads) : stopScheduler(false){
    for (size_t i = 0; i < numThreads; ++i){
        workerThreads.emplace_back(&TaskScheduler::worker, this);
    }
}

TaskScheduler::~TaskScheduler(){
    stopScheduler = true;
    cv.notify_all();
    for (auto& thread : workerThreads){
        if (thread.joinable()){
            thread.join();
        }
    }
}

void TaskScheduler::worker(){
    while(true){
        Task task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this] { return !taskQueue.empty() || stopScheduler; });
            
            if (stopScheduler && taskQueue.empty()){
                break;
            }
            
            task = taskQueue.top();
            taskQueue.pop();
        }
        task.function();
        taskCompletion[task.id] = true;
    }
}

void TaskScheduler::addTask(Task task){
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push(task);
        taskCompletion[task.id] = false;
    }
    cv.notify_one();
}

void TaskScheduler::addTasks(const std::vector<Task>& tasks){
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        for (const auto& task : tasks){
            taskQueue.push(task);
            taskCompletion[task.id] = false;
        }
    }
    cv.notify_all();
}

bool TaskScheduler::cancelTask(int taskId){
    std::lock_guard<std::mutex> lock(queueMutex);
    std::priority_queue<Task> newQueue;
    bool found = false;
    
    while(!taskQueue.empty()){
        Task task = taskQueue.top();
        taskQueue.pop();
        if (task.id != taskId){
            newQueue.push(task);
        }else{
            found = true;
        }
    }
    taskQueue = std::move(newQueue);
    return found;
}

size_t TaskScheduler::getPendingTasksCount(){
    std::lock_guard<std::mutex> lock(queueMutex);
    return taskQueue.size();
}

size_t TaskScheduler::getWorkerThreadsCount(){
    return workerThreads.size();
}

void TaskScheduler::increaseWorkerThreads(size_t num){
    for (size_t i = 0; i < num; ++i){
        workerThreads.emplace_back(&TaskScheduler::worker, this);
    }
}

void TaskScheduler::decreaseWorkerThreads(size_t num){
    for (size_t i = 0; i < num && !workerThreads.empty(); ++i){
        workerThreads.back().detach();
        workerThreads.pop_back();
    }
}

void TaskScheduler::waitForTask(int taskId){
    while(!taskCompletion[taskId]){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

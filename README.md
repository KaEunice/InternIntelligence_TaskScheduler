# Task Manager

Task Manager is a C++ project that implements a task scheduler with a thread pool. It allows you to batch schedule tasks with various priorities and execute them concurrently. The project is designed to be simple and efficient.

## Features

- **Concurrent Task Execution:** Uses a thread pool to run tasks in parallel.
- **Task Prioritization:** Supports scheduling tasks with different priorities.
- **Flexible Scheduling:** Tasks are scheduled to run after a specified delay.
- **Dynamic Worker Management:** Increase or decrease worker threads at runtime.
- **Simple API:** Easily add tasks and monitor the task queue.

## Project Structure

```
Task_Manager/
├── bin/
│   └── taskManager        # Compiled executable
├── src/
│   ├── main.cpp           # Main function demonstrating TaskScheduler usage
│   ├── taskManager.cpp    # Implementation of the TaskScheduler class
│   └── taskManager.hpp    # Declarations of Task, TaskScheduler, and related types
├── README.md              # Project documentation
└── LICENSE                # MIT License
```

## Requirements

- **Compiler:** g++ (GNU Compiler Collection) with C++17 support recommended.
- **Library:** POSIX threads (pthread)

## Using Terminal

From the project root (Task_Manager folder), run:

```bash
g++ -fdiagnostics-color=always -g src/main.cpp src/taskManager.cpp -o bin/taskManager -I./src -lpthread -std=c++17
```

## Running the Project

After building, execute the compiled binary:

```bash
./bin/taskManager
```

## Example Usage

Below is an excerpt from `src/main.cpp` that demonstrates how to create the TaskScheduler, add tasks in a batch, wait for specific tasks, and adjust the number of worker threads:

```cpp
#include "taskManager.hpp"
#include <chrono>

int main(){
    // Create a TaskScheduler using 4 worker threads
    TaskScheduler scheduler(4);

    // Define the task function for demonstration
    auto taskFunction = [](int id, TaskPriority priority) {
        std::cout << "Task " << id << " with priority " << priority
                  << " is running on thread " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    };

    // Prepare tasks with various delays and priorities
    std::vector<Task> tasks = {
        {1, 1, std::chrono::steady_clock::now() + std::chrono::seconds(2), std::bind(taskFunction, 1, 1)},
        {2, 3, std::chrono::steady_clock::now() + std::chrono::seconds(4), std::bind(taskFunction, 2, 3)},
        {3, 5, std::chrono::steady_clock::now() + std::chrono::seconds(6), std::bind(taskFunction, 3, 5)},
        {4, 2, std::chrono::steady_clock::now() + std::chrono::seconds(1), std::bind(taskFunction, 4, 2)},
        {5, 4, std::chrono::steady_clock::now() + std::chrono::seconds(3), std::bind(taskFunction, 5, 4)},
        {6, 6, std::chrono::steady_clock::now() + std::chrono::seconds(5), std::bind(taskFunction, 6, 6)}
    };

    // Schedule tasks
    scheduler.addTasks(tasks);

    // Wait for specific tasks to complete
    scheduler.waitForTask(4);
    scheduler.waitForTask(1);

    // Display current queue state and test dynamic thread management
    std::cout << "Pending tasks: " << scheduler.getPendingTasksCount() << std::endl;
    scheduler.increaseWorkerThreads(2);
    std::cout << "Worker threads increased: " << scheduler.getWorkerThreadsCount() << std::endl;
    scheduler.decreaseWorkerThreads(1);
    std::cout << "Worker threads decreased: " << scheduler.getWorkerThreadsCount() << std::endl;

    return 0;
}
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
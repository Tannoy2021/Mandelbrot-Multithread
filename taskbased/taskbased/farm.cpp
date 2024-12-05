#include "farm.h"


void Farm::add_task(Task *task)
{
	task_queue.push(task);
}
void Farm::run()
{
	// FIXME - Implement this
	const int hardware_concurrency_cpu_threads = std::thread::hardware_concurrency();
	// if you want to change the number of threads change the above value to x amount of threads

	std::vector<std::thread*> threads;
	// this is a vector collection of threads;

	// this for loop will start as many threads as the const int: total_number_of_threads;
	
	for (int i = 0; i < total_number_of_threads; i++)
	{
		// this is the thread worker function created by using a lambda expression
		threads.push_back(new std::thread([&]
			{
				// this is an infinite loop that will 
				// remove a task from the task queue with appropriate protection for this part
				// run the task
				// delete the task 
				while (I_am_always_true)
				{
					// if the task queue is empty this will return from the function and it will end the thread
					if (task_queue.empty())
					{
						return;
					}
					queue_protection_mutex.lock();
					// This will take a task from the queue
					Task* task = task_queue.front();
					// This will remove the given task from the que
					task_queue.pop();
					// locking and unlocking will protect the tasks from multiple threads trying to get access to them
					queue_protection_mutex.unlock();

					// after receiving the task , run the given task
					task->run();
					// after running the given task delete the task
					delete task;
				}
			}));
	}
		for (int i = 0; i < total_number_of_threads; i++)
		{
			threads[i]->join();
		}
}

void Farm::threads(int threads)
{
	total_number_of_threads = threads;
}

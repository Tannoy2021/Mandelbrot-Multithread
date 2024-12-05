#ifndef FARM_H
#define FARM_H

#include "task.h"
#include "queue"
#include "thread"
#include "vector"
#include "mutex"
#include <iostream>


// FIXME - You will need to add #includes here (probably <mutex> at least)

/** A collection of tasks that should be performed in parallel. */
class Farm{
public:
	// DO NOT CHANGE the public interface of this class.
	// You only need to implement the existing methods.

	/** Add a task to the farm.
	    The task will be deleted once it has been run. */
	void add_task(Task *task);
	/** Run all the tasks in the farm.
	    This method only returns once all the tasks in the farm
		have been completed. */
	void run();
	void threads(int threads);
private:
	// FIXME - You will need to add private member variables here
	std::queue<Task*> task_queue;
	bool I_am_always_true = true;
	std::mutex queue_protection_mutex;
	int total_number_of_threads;
	int tester_ = 0;
};

#endif

#pragma once

#include <mutex>
#include <condition_variable>
#include <iostream>

using std::mutex;
using std::unique_lock;
using std::condition_variable;

class Semaphore
{
public:
	Semaphore(int count_ = 0)
		:count(count_)
	{}
	inline void notify()
	{
		unique_lock<mutex> lock(mutex_);
		count++;
		conditional_variable_.notify_one();
	}
	inline void wait()
	{
		unique_lock<mutex> lock(mutex_);
		while (count == 0)
		{
			conditional_variable_.wait(lock);
		}
		count--;
	}
private:
	mutex mutex_;
	condition_variable conditional_variable_;
	int count;
};
// The way to create the notify and wait functions were seen from the riptutorial.com website credit goes to them 


#pragma once

#include "task.h"
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <complex>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <barrier>
#include "Semaphore.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::complex;
using std::cout;
using std::endl;
using std::ofstream;
using std::thread;
using std::mutex;
using std::unique_lock;
using std::condition_variable;

typedef std::chrono::steady_clock the_clock;


class Mandelbrot : public Task
{
public:
	Mandelbrot(double left, double right, double top, double bottom, double start_slice, double end_slice,uint32_t(*pointer)[1200][1920],Semaphore* semaphore_pointer)
		: left_(left), right_(right), top_(top), bottom_(bottom), start_slice_(start_slice), end_slice_(end_slice),pointer_(pointer),semaphore_pointer_(semaphore_pointer)
	{
	}
		void run();
		const int total_threads = 8;
private:
	double left_; 
	double right_;
	double top_; 
	double bottom_;
	double start_slice_;
	double end_slice_;
	int threads = 0;
	int slices_ = 50;

	const int WIDTH = 1920;
	const int HEIGHT = 1200;
	const int MAX_ITERATIONS = 500;
	uint32_t(*pointer_)[1200][1920];
	Semaphore* semaphore_pointer_;
	mutex counter_mutex;
	
};


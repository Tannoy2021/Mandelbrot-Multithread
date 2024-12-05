#pragma once

#include "task.h"
#include "Mandelbrot.h"
#include "Semaphore.h"

class Write_Tga : public Task
{
public:
	Write_Tga(const char* filename, uint32_t(*pointer)[1200][1920],Semaphore* semaphore_pointer)
		: filename_(filename), pointer_(pointer), semaphore_pointer_(semaphore_pointer)
	{
	}
	void run();
private:
	const int WIDTH = 1920;
	const int HEIGHT = 1200;
	const char* filename_;
	uint32_t(*pointer_)[1200][1920];
	Semaphore* semaphore_pointer_;

};


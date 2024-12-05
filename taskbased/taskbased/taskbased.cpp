// Task-based parallelism example
// Adam Sampson <a.sampson@abertay.ac.uk>

#include <iostream>
#include <string>
#include <barrier>
#include <condition_variable>

#include "Semaphore.h"
#include "farm.h"
#include "task.h"
#include "messagetask.h"
#include "Mandelbrot.h"
#include "Write_Tga.h"



// Import things we need from the standard library
using std::cout;
using std::cin;
using std::to_string;

const int WIDTH = 1920;
const int HEIGHT = 1200;
int threads_test = 8;
int loops = 1;
//Since inside the for loop we are creating slices using the HEIGHT variable 
// The number of slices has to be able to divide the HEIGHT
// For example 1200/50 = 24
// But 1200/64 = 18.75 and because of that .75 there would be a missing slice at the very top of the image 
int slices = 10;
uint32_t image[HEIGHT][WIDTH];
Semaphore My_Semaphore(0);

void write_tga(const char* filename)
{
	//cout << "There is a barrier in front of me I need to wait..." << endl;
	//barrier_number.arrive_and_wait();
	ofstream outfile(filename, ofstream::binary);
	uint8_t header[18] = {
		0, // no image ID
		0, // no colour map
		2, // uncompressed 24-bit image
		0, 0, 0, 0, 0, // empty colour map specification
		0, 0, // X origin
		0, 0, // Y origin
		WIDTH & 0xFF, (WIDTH >> 8) & 0xFF, // width
		HEIGHT & 0xFF, (HEIGHT >> 8) & 0xFF, // height
		24, // bits per pixel
		0, // image descriptor
	};
	outfile.write((const char*)header, 18);

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			uint8_t pixel[3] = {
				image[y][x] & 0xFF, // blue channel
				(image[y][x] >> 8) & 0xFF, // green channel
				(image[y][x] >> 16) & 0xFF, // red channel
			};
			outfile.write((const char*)pixel, 3);
		}
	}

	outfile.close();
	if (!outfile)
	{
		// An error has occurred at some point since we opened the file.
		cout << "Error writing to " << filename << endl;
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	double left = -2.0; double right = 1.0; double top = 1.125; double bottom = -1.125;

	uint32_t(*pointer)[1200][1920];
	pointer = &image;
	Semaphore* semaphore_pointer;
	semaphore_pointer = &My_Semaphore;

	cout << "Enter number of threads : ";
	cin >> threads_test;
	cout << "Please input a number that is able to divide 1200" << endl;
	cout << "Enter number of slices(number of tasks) : ";
	cin >> slices;
	cout << "How many times do you want to loop? : ";
	cin >> loops;



	Farm Mandelbrot_Farm;
	Farm Write_Farm;
	// the value inside of threads() is the number of threads created for the mandelfrot_farm
	Mandelbrot_Farm.threads(threads_test);
	// The reason the start time is outside of the for loop is so I can get a total number for time which I can later divide by the amount of loops and get the average time it took to run it
	// This line can be put inside of the for loop and you will get the time it takes to render an entire mandelbrot only 1 time instead of x amount of loops
	//the_clock::time_point start_total = the_clock::now();
	for (int j = 0; j < loops; j++)
	{
		
		// each slice represents a task that a core can take
		the_clock::time_point start_total = the_clock::now();
		for (int i = 0; i < slices; i++)
		{
			Mandelbrot_Farm.add_task(new Mandelbrot(left, right, top, bottom, (HEIGHT / slices) * i, (HEIGHT / slices) * (i + 1), pointer,semaphore_pointer));
		}
		Write_Farm.add_task(new Write_Tga("output.tga", pointer, semaphore_pointer));
		Mandelbrot_Farm.run();
		Write_Farm.run();
		
		
		the_clock::time_point end_total = the_clock::now();
		auto time_taken = duration_cast<milliseconds>(end_total - start_total).count();
		cout << "Computing the Mandelbrot set took " << time_taken << " ms." << endl;

	}


	//Task *t = new MessageTask("hello, world!");
	//cout << "Running one task...\n";
	//t->run();
	//delete t;
	//// Example: run a load of tasks using a farm
	//Farm f;
	//for (int i = 0; i < 200; ++i)
	//{
	//	f.add_task(new MessageTask("I am task " + to_string(i)));
	//}
	//cout << "Running task farm...\n";
	//f.run();
	//cout << "Tasks complete!\n";

	return 0;
}
// f.add_task(new Mandelbrot(left, right, top, bottom, 600, 1200, pointer));
#include "Mandelbrot.h"

void Mandelbrot::run()
{
	for (int y = start_slice_; y < end_slice_; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(left_ + (x * (right_ - left_) / WIDTH),
				top_ + (y * (bottom_ - top_) / HEIGHT));

			// Start off z at (0, 0).
			complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
			{
				z = (z * z) + c;

				++iterations;
			}

			if (iterations == MAX_ITERATIONS)
			{
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.
				(*pointer_)[y][x] = 0x000000; // black
			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.
				(*pointer_)[y][x] = 0x1a6b17 + threads << iterations; // Colors
			}
		}
	}
	// after the threads have finished their work they will notify the waiting Write_Tga function task which will unblock it and allow for the image to be drawn into a .TGA file
	// if the line bellow is commented out we can see that we wont receive a .tga file with the mandelbrot image rendered because the write_tga thread is locked and waiting
	semaphore_pointer_->notify();
}

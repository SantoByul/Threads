
GCC = gcc -fopenmp
REMOVE = threads mandelbrot_dsob_openmp.pgm mandelbrot_dsob_pthreads1.pgm mandelbrot_dsob_pthreads2.pgm mandelbrot_dsob_serial.pgm timer.txt mandelbrot

all:
	$(GCC) main.c -o mandelbrot -lpthread
	./mandelbrot 800 800 1000 4

compile:
	$(GCC) main.c -o mandelbrot -lpthread

clean:
	rm -f $(REMOVE)
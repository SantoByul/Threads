
REMOVE = threads mandelbrot_dsob_openmp.pgm mandelbrot_dsob_pthreads1.pgm mandelbrot_dsob_serial.pgm

all:
	gcc main.c -o threads -lpthread
	./threads 1 1 1 1

compile:
	gcc main.c -o threads

clean:
	rm -f $(REMOVE)
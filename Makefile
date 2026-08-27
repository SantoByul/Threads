
REMOVE = threads mandelbrot_dsob_openmp.pgm mandelbrot_dsob_pthreads1.pgm mandelbrot_dsob_serial.pgm timer.txt

all:
	gcc main.c -o threads -lpthread
	./threads 800 800 1000 4

compile:
	gcc main.c -o threads -lpthread

clean:
	rm -f $(REMOVE)
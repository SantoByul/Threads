all:
	gcc main.c -o threads
	./threads 1 1 1 1

compile:
	gcc main.c -o threads

clean:
	rm -f threads
all:
	gcc main.c -o threads
	./threads

clean:
	rm -f threads
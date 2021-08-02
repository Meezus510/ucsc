output: httpserver.o 
	gcc httpserver.o -o httpserver -pthread

httpserver.o: httpserver.c
	gcc -c httpserver.c

clean:
	rm *.o httpserver
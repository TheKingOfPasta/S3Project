all: mnist_reader.o neural_network.o
	gcc -Wall -Wextra -fsanitize=address mnist_reader.o neural_network.o -o network

mnist_reader.o: mnist_reader.c
	gcc -Wall -Wextra mnist_reader.c -c

neural_network.o: neural_network.c
	gcc -Wall -Wextra neural_network.c -c

clean:
	rm network
	rm *.o
solver : solver.o parser.o
	gcc -o solver solver.o parser.o

solver.o : solver.c solver.h
	gcc solver.c solver.h -c

parser.o : parser.c parser.h
	gcc parser.c parser.h -c

clean:
	rm *.gch
	rm *.o
	rm solver
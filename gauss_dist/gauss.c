#include <err.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

double* Norm(int size, double pas, int* newSize) {
	if (!size)
		size = 1;
	//returns the gaussian distribution in "[-size,size]" with an increment of "pas"
	double* dist = malloc(((int)(2*size/pas)+1)*sizeof(double));
	int j = 0;
	for(double i = -size; i<size; i+=pas) {
		dist[j] = exp(-(i*i)/2)/sqrt(2*M_PI);
		j++;
	}
	*newSize = j;
	return dist;	
}

double toDouble(char* c) {
	double total = 0;
	double mult = 10.0;
	int t = 0;
	for(int i = 0; c[i]; i++) {
		if(c[i] == '.') {
			t = 2;
			i++;
		}
		if (t == 0) {
			total *= 10;
			total += c[i]-'0';
		}	
		else {
			total += (c[i]-'0')/mult;
			mult *= 10;
		}
	}
	return total;
}	

int main(int argc, char** argv) {
	if (argc != 3)
		errx(1,"Use : ./gauss [size of the array] [increment]\n");
	
	int newSize;
	double* t = Norm((int)toDouble(argv[1]),toDouble(argv[2]),&newSize);

	for(int i = 0; i<newSize; i++) {
		printf("gauss[%i] = %f\n",i,t[i]);
	}
	return 0;	
}

all:
	cd gui && make
	cd solver && make
	cd img_processing && make
	cd hough && make
	cd gauss_dist && make
	cd neuron_network && make

clean:
	cd solver && make clean
	cd hough && make clean
	cd gauss_dist && make clean
	cd gui && rm *.o gui
	cd splitting && rm *.o
	cd img_processing && rm img_action *.o
	cd neuron_network && make clean
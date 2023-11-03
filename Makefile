all:
	cd gui && make
	cd solver && make
	cd img_processing && make
	cd hough && make
	cd gauss_dist && make

clean:
	cd img_processing && make clean
	cd solver && make clean
	cd hough && make clean
	cd gauss_dist && make clean
	cd gui && make clean

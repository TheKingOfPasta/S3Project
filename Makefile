all:
	cd gui && make
	cd solver && make
	cd img_processing && make

clean:
	cd img_processing && make clean
	cd solver && make clean
	cd gui && make clean
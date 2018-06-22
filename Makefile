all:
	g++ -o bin/mean_shift.o src/mean_shift.cpp src/main.cpp -g --std=c++11 -I/usr/include/python2.7 -lpython2.7

latex:
	pdflatex -output-directory=./build/ report.tex

tests:
	g++ -o bin/mean_shift_test1.o src/mean_shift.cpp src/test/main.cpp src/test/test1.cpp -g --std=c++11 -I/usr/include/python2.7 -lpython2.7

tests-visual:
	g++ -o bin/mean_shift_test_visual1.o src/mean_shift.cpp src/test/main.cpp src/test/test1.cpp -g --std=c++11 -I/usr/include/python2.7 -lpython2.7 -DMS_VISUAL=true

clean:
	rm bin/mean_shift*.o || true
	rm build/report.* || true
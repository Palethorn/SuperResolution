INCLUDE=-I/usr/include/opencv
FLAGS=-lml -lcvaux -lhighgui -lcv -lcxcore
LIBS=-L/usr/lib
main:
	g++ -o main *.cpp ${INCLUDE} ${LIBS} ${FLAGS} 

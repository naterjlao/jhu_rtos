CC = g++
FLAGS = -DDEBUG=0 -DRPI=1
OBJ = udp.o
LIBS = -lopencv_core -lopencv_imgcodecs -lopencv_videoio
INCLUDES = -I../include/
COMMON=../common

all: flypi-cam.d

flypi-cam.d: cam.cpp $(OBJ)
	$(CC) cam.cpp $(FLAGS) $(OBJ) $(LIBS) $(INCLUDES) -o flypi-cam.d

udp.o: $(COMMON)/udp.cpp
	$(CC) $(FLAGS) $(INCLUDES) -c $(COMMON)/udp.cpp

clean:
	rm -rvf *.o
	rm -rvf *.out
	rm -rvf *.d

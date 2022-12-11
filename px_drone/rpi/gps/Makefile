CC = g++
FLAGS = -DDEBUG=0 -DRPI=1
OBJ = udp.o
LIBS = -lwiringPi
INCLUDES = -I../include/
COMMON=../common

all: flypi-gps.d

flypi-gps.d: gps.cpp $(OBJ)
	$(CC) gps.cpp $(FLAGS) $(OBJ) $(LIBS) $(INCLUDES) -o flypi-gps.d

udp.o: $(COMMON)/udp.cpp
	$(CC) $(FLAGS) $(INCLUDES) -c $(COMMON)/udp.cpp

clean:
	rm -rvf *.o
	rm -rvf *.out
	rm -rvf *.d
CC = g++
FLAGS = -DDEBUG=0 -DRPI=1
OBJ = serial_usb.o udp.o
LIBS = wiringPi
INCLUDES = -I../imu/include/ -Iinclude/

all: flypi-imu.d

imu: imu.cpp $(OBJ)
	$(CC) imu.cpp $(FLAGS) $(OBJ) -l$(LIBS) $(INCLUDES) -o flypi-imu.d

serial_usb.o: serial_usb.cpp
	$(CC) $(FLAGS) $(INCLUDES) -c serial_usb.cpp

udp.o: udp.cpp
	$(CC) $(FLAGS) $(INCLUDES) -c udp.cpp

clean:
	rm -rvf *.o
	rm -rvf *.out
	rm -rvf imu

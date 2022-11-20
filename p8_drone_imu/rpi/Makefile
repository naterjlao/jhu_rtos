CC = g++
FLAGS = -DDEBUG=1
OBJ = serial_usb.o
LIBS = wiringPi
INCLUDES = -I../imu/include/ -Iinclude/

all: imu

imu: imu.cpp serial_usb.o
	$(CC) imu.cpp $(FLAGS) $(OBJ) -l$(LIBS) $(INCLUDES) -o imu

serial_usb.o: serial_usb.cpp
	$(CC) $(FLAGS) $(INCLUDES) -c serial_usb.cpp

clean:
	rm -rvf *.o
	rm -rvf *.out
	rm -rvf imu
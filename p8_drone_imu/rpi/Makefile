CC = g++
FLAGS = -DDEBUG=1
OBJ = serial_usb.o
LIBS = wiringPi
INCLUDES = -I../imu/include/ -Iinclude/

all: flypi

flypi: flypi.cpp serial_usb.o
	$(CC) flypi.cpp $(FLAGS) $(OBJ) -l$(LIBS) $(INCLUDES) -o flypi

serial_usb.o: serial_usb.cpp
	$(CC) $(FLAGS) $(INCLUDES) -c serial_usb.cpp

clean:
	rm -rvf *.o
	rm -rvf *.out
	rm -rvf flypi
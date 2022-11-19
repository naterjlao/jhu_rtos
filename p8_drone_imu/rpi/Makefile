LIBS = wiringPi
INCLUDES = -I../imu/include/ -Iinclude/

all: flypi

flypi: flypi.cpp serial_usb.o
	g++ flypi.cpp -l$(LIBS) $(INCLUDES) -o flypi

serial_usb.o:
	g++ -c serial_usb.cpp $(INLCUDES)

clean:
	rm -rvf *.o
	rm -rvf flypi
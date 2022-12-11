# CLEAN SLATE
rm -rvf ~/flypi.d
mkdir ~/flypi.d

# CAMERA DAEMON
cd cam
make clean
make all
mv -v flypi-cam.d ~/flypi.d/
cd ..

# GPS DAEMON
cd gps

cd ..

# IMU DAEMON
cd imu
make clean
make all
mv -v flypi-imu.d ~/flypi.d/
cd ..
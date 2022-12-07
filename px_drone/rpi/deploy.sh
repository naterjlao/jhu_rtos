# CAMERA DAEMON
cd cam

cd ..


# IMU DAEMON
cd imu
make clean
make all
rm -rvf ~/flypi.d
mkdir ~/flypi.d
mv -v flypi-imu.d ~/flypi.d/
cd ..
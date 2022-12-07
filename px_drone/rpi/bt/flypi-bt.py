'''
@file flypi-bt.py
@author Nate Lao (nlao1@jh.edu)
@brief Simple Bluetooth Server processing for IMU/Bluetooth client interface.
'''

import serial
import struct
import bluedot.btcomm

# Must match the device to send to - raspberrypi refers to the ground station host
BT_SERVER = "raspberrypi"

# setup bluetooth interface
bt_socket = bluedot.btcomm.BluetoothClient("raspberrypi", None)

with serial.Serial('/dev/ttyACM1', 9600) as ser:
        while True:
                input = ser.read(16) # read in serial data from IMU
                _, yaw, pitch, roll = struct.unpack('Lfff',input) # unpack payload
                output = "YAW={:.3f}\tPITCH={:.3f}\tROLL={:.3f}".format(yaw, pitch, roll)
                bt_socket.send(output) # transmit formatted data

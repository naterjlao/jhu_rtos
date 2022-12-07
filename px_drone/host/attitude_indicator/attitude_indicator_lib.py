# Attitude Indicator Monitor Libraries
# Author: Nate Lao (nlao1@jh.edu)
# Designed for Windows OS
import socket
import struct
import time

# TODO create superclass
class ETHERNET_ADAPTER:
    # The Multicast IP and Port of the Rx Payload
    BROADCAST = "239.100.100.250"
    PORT = 8250
    # NOTE: This is windows dependent for connecting to the correct interface
    # For Windows, use ipconfig and enter the host's current IP
    INTERFACE = "192.168.4.35"

    PAYLOAD_SIZE = 16
    PAYLOAD_FORM = '!fff1B3b'

    # TODO parameterize arguments
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(('',ETHERNET_ADAPTER.PORT))
        group = socket.inet_aton(ETHERNET_ADAPTER.BROADCAST)
        iface = socket.inet_aton(ETHERNET_ADAPTER.INTERFACE) 
        self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, group+iface)
        self.sock.settimeout(0.1)

    def recv(self):
        try:
            payload = self.sock.recv(ETHERNET_ADAPTER.PAYLOAD_SIZE)
            success = True
        except socket.timeout:
            success = False
        if success:
            # Unpack payload
            payload_fields = struct.unpack(ETHERNET_ADAPTER.PAYLOAD_FORM, payload)
            yaw, pitch, roll, validity, spare_1, spare_2, spare_3 = payload_fields
            return yaw, pitch, roll, validity
        else:
            return None

    def send(self, payload):
        
        return self.sock.sendto(payload, (ETHERNET_ADAPTER.BROADCAST, ETHERNET_ADAPTER.PORT))

import sys
if __name__ == "__main__":
    error_msg = lambda : print("ERROR: Invalid arguments expect --recv or --send")
    if len(sys.argv) == 2:
        receiver = ETHERNET_ADAPTER()
        if (sys.argv[1].strip() == "--recv"):
            while True:
                print(receiver.recv())
                time.sleep(0.1)
        elif (sys.argv[1].strip() == "--send"):

            yaw = 0.0
            pitch = 0.0
            roll = 0.0
            yaw_inc = True
            pitch_inc = True
            roll_inc = True
            while True:
                payload = struct.pack(ETHERNET_ADAPTER.PAYLOAD_FORM,yaw,pitch,roll,0xff,0,0,0)
                receiver.send(payload)
                yaw += 0.1 if yaw_inc else -0.1
                pitch += 0.5 if pitch_inc else -0.5
                roll += 0.2 if roll_inc else -0.2
                if yaw < -45.0: yaw_inc = True
                elif yaw > 45.0: yaw_inc = False
                if roll < -45.0: roll_inc = True
                elif roll > 45.0: roll_inc = False
                if pitch < -45.0: pitch_inc = True
                elif pitch > 45.0: pitch_inc = False
                print(f"sending yaw={yaw} pitch={pitch} roll={roll}")
                time.sleep(0.001)
        else:
            error_msg()
    else:
        error_msg()
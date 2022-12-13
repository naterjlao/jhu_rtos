# Attitude Indicator Monitor Libraries
# Author: Nate Lao (nlao1@jh.edu)
# Designed for Windows OS
import socket
import time

# TODO create superclass
class ETHERNET_ADAPTER_CAM:
    # The Multicast IP and Port of the Rx Payload
    BROADCAST = "239.100.100.250"
    PORT = 8251
    # NOTE: This is windows dependent for connecting to the correct interface
    # For Windows, use ipconfig and enter the host's current IP
    INTERFACE = "192.168.4.35"

    PAYLOAD_SIZE = 50000


    # TODO parameterize arguments
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(('',ETHERNET_ADAPTER_CAM.PORT))
        group = socket.inet_aton(ETHERNET_ADAPTER_CAM.BROADCAST)
        iface = socket.inet_aton(ETHERNET_ADAPTER_CAM.INTERFACE) 
        self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, group+iface)
        self.sock.settimeout(0.1)

    def recv(self):
        try:
            payload = self.sock.recv(ETHERNET_ADAPTER_CAM.PAYLOAD_SIZE)
            success = True
        except socket.timeout:
            success = False
        if success:
            return payload
        else:
            return None

import sys
if __name__ == "__main__":
    error_msg = lambda : print("ERROR: Invalid arguments expect --recv or --send")
    if len(sys.argv) == 2:
        receiver = ETHERNET_ADAPTER_CAM()
        if (sys.argv[1].strip() == "--recv"):
            while True:
                print(len(receiver.recv()))
                time.sleep(0.1)
        else:
            error_msg()
    else:
        error_msg()
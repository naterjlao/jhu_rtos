# Attitude Indicator Monitor Libraries
# Author: Nate Lao (nlao1@jh.edu)
# Designed for Windows OS
import socket
import struct
class RECEIVER:
    # The Multicast IP and Port of the Rx Payload
    BROADCAST = "239.100.100.250"
    PORT = 8250
    # NOTE: This is windows dependent for connecting to the correct interface
    INTERFACE = "192.168.12.248"#192.168.4.35"

    PAYLOAD_SIZE = 16
    PAYLOAD_FORM = '!fff1b3b'
    TRAFFIC_ID = 0x45535057
    
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(('',RECEIVER.PORT))
        group = socket.inet_aton(RECEIVER.BROADCAST)
        iface = socket.inet_aton(RECEIVER.INTERFACE) 
        self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, group+iface)
        self.sock.settimeout(0.1)

    def recv(self):
        try:
            payload = self.sock.recv(RECEIVER.PAYLOAD_SIZE)
            success = True
        except socket.timeout:
            success = False
        if success:
            # Unpack payload
            payload_fields = struct.unpack(RECEIVER.PAYLOAD_FORM, payload)
            yaw, pitch, roll, validity, spare_1, spare_2, spare_3 = payload_fields
            return yaw, pitch, roll, validity
        else:
            return None
if __name__ == "__main__":
    receiver = RECEIVER()
    while True:
        print(receiver.recv())
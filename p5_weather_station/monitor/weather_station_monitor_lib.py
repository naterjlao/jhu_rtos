# Weather Station Monitor Libraries
# Author: Nate Lao (nlao1@jh.edu)
# Designed for Windows OS
import socket
import struct
class RECEIVER:
    # The Multicast IP and Port of the Rx Payload
    BROADCAST = "224.0.1.3"
    PORT = 8266
    # NOTE: This is windows dependent see thread
    INTERFACE = "192.168.4.35"
    # Defined by protocol. Total 16 bytes:
    # TRAFFIC ID = 4 bytes
    # SAMPLE NUM = 4 bytes
    # TEMPERATURE= 4 bytes
    # HUMIDITY = 4 bytes
    PAYLOAD_SIZE = 16
    PAYLOAD_FORM = '!llff'
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
            traffic_id, sample_n, temp, humidity = struct.unpack(RECEIVER.PAYLOAD_FORM,
            payload)
            # Verify with expected Traffic ID
            return (traffic_id, sample_n, temp, humidity) if traffic_id == RECEIVER.TRAFFIC_ID else None
        else:
            return None
if __name__ == "__main__":
    receiver = RECEIVER()
    while True:
        print(receiver.recv())
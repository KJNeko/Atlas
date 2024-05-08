#Used in conjunction with the server. Will listen on port 5555 for console output.

import socket
import os
from utilities.console import *

#Clear screen to clear all garbage. This also fixes bug in CMD
os.system('cls')

def udp():
        multicast_group = "127.0.0.1"
        console.output("UDP Debug Terminal Started")
        server_address = ('', 5555)
        # Create the socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, True)
        if hasattr(socket, 'SO_REUSEPORT'):
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, True)

        # Bind to the server address
        sock.bind(server_address)
        # Tell the operating system to add the socket to the multicast group
        # on all interfaces.


        while True:
            data, addr = sock.recvfrom(1024)
            message = data.decode(('utf-8'))
            if len(message) > 0:
                console.output(message)

udp()

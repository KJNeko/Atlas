import socket

class multicast(object):
    ttl = 2

    def send(message):
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
            sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL,multicast.ttl)
            sock.sendto(message.encode('utf-8'), ("127.0.0.1", 5555))

        except Exception as e:
            print(e)
            return 0

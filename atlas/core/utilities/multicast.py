from socket import socket, AF_INET, SOCK_DGRAM, IPPROTO_UDP, IPPROTO_IP, IP_MULTICAST_TTL

class multicast(object):
    ttl = 2

    def send(message):
        try:
            sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)
            sock.setsockopt(IPPROTO_IP, IP_MULTICAST_TTL,multicast.ttl)
            sock.sendto(message.encode('utf-8'), ("127.0.0.1", 5555))

        except Exception as e:
            print(e)
            return 0

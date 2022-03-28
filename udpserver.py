from socket import *
from time import ctime

HOST = "0.0.0.0"
PORT = 9999
BUFSIZ = 1024
ADDR = (HOST, PORT)

udpSerSock = socket(AF_INET, SOCK_DGRAM)
udpSerSock.bind(ADDR)

while True:
    print("waiting for message...")
    data, addr = udpSerSock.recvfrom(BUFSIZ)

    content = "[%s] %s" % (bytes(ctime(), "utf-8"), data)
    udpSerSock.sendto(content.encode("utf-8"), addr)
    print("...received from and returned to:", addr)
    print("...data:", data)

udpSerSock.close()

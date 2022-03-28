import socket
from urllib import response

host = "172.20.239.170"
port = 8888

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((host, port))

while True:
    data = input("> ")
    client.send(bytes(data, "utf-8"))
    if not data:
        break
    response = client.recv(1024)
    if not response:
        break
    print(str(response))
client.close()

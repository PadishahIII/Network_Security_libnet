import socket
import threading

server_ip = "0.0.0.0"
server_port = 8888

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server.bind((server_ip, server_port))

server.listen(5)

print("[*] Listening on %s:%d" % (server_ip, server_port))


def handle_client(client_socket, addr):
    while True:
        request = client_socket.recv(1024)
        if not request:
            break
        print("[*] Received: %s" % str(request))
        client_socket.send(bytes("ACK", "utf-8"))
    client_socket.close()
    print("[*] Disconnect:%s:%d" % (addr[0], addr[1]))


while True:
    client, addr = server.accept()
    print("[*] Connection from: %s:%d" % (addr[0], addr[1]))
    client_thread = threading.Thread(target=handle_client, args=(client, addr))
    client_thread.start()

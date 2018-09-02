import sys
import socket
import threading
import console_colors

all_clients = []

def handle_client(client_socket, address):
    while True:
        try:
            byte = client_socket.recv(1)
        except:
            break  # an exception here means the socket was abruptly closed by the other side
        if len(byte) == 0:
            break  # this means the connection was closed gracefully by the other side
    
        all_clients_copy = all_clients[:]
        for s in all_clients_copy: 
            s.send(byte)

        print(byte.decode('ascii'), end='', flush=True)
    all_clients.remove(client_socket)
    print(" ** connection lost", address, "count=", len(all_clients))


def main(argv):
    term = console_colors.Terminal()

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('', 666))  # configure the socket listening port and address
    server_socket.settimeout(0.5)
    server_socket.listen(5)  # transition socket to LISTENING, backlog connections (how many are allowed to be pending)
    while 1:
        try:
            (client_socket, address) = server_socket.accept()   # wait for new connection
        except socket.timeout:
            continue
        all_clients.append(client_socket)
        print("** Got connection from", address, "count=", len(all_clients))

        t = threading.Thread(target=handle_client, args=(client_socket, address), daemon=True)
        t.start()




if __name__ == "__main__":
    main(sys.argv)
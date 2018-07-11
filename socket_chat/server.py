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
            break  # an exception here means the socket was abruptly closed
        if len(byte) == 0:
            break

        for s in all_clients:
            try:
                s.send(byte)
            except:
                pass # an exception here means this socket was already closed

        print(byte.decode('ascii'), end='', flush=True)
    print(" ** connection lost", address)


def main(argv):
    term = console_colors.Terminal()

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('', 666))
    server_socket.listen(5)  # backlog connections (how many are allowed to be pending)
    server_socket.settimeout(0.5)
    while 1:
        try:
            (client_socket, address) = server_socket.accept()
        except socket.timeout:
            continue
        print("** Got connection from", address)

        all_clients.append(client_socket)
        t = threading.Thread(target=handle_client, args=(client_socket, address), daemon=True)
        t.start()




if __name__ == "__main__":
    main(sys.argv)
import sys
import socket
import msvcrt  # this works on windows, on linux do pip install getch and replace with import getch
import console_colors
import threading


def handle_recv(s):
    while True:
        byte = s.recv(1)
        if len(byte) == 0:
            break
        print(byte.decode('ascii'), end='', flush=True)



def main(argv):
    color = int(argv[1])
    term = console_colors.Terminal()
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    #s.settimeout(0.5)
    s.connect(("127.0.0.1", 666))

    t = threading.Thread(target=handle_recv, args=(s,), daemon=True)
    t.start()

    while True:
        inp = msvcrt.getch()
        #print(ch)

        #decode some special characters
        if inp == b'\x03':
            break
        if inp == b'\r':
            inp = b'\n'
        if inp == b'\x08':
            inp = b'\x08 \x08'

        inp = term.col_n(color, inp)

        #print(inp.decode('ascii'), end='', flush=True)
        s.send(inp)

if __name__ == "__main__":
    main(sys.argv)
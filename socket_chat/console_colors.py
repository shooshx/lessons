import sys

class Terminal(object):
    def __init__(self):
        if sys.platform == 'win32':
            try:
                import ctypes
                import ctypes.wintypes
                h = ctypes.windll.kernel32.GetStdHandle(-11)  # STD_OUTPUT_HANDLE
                x = ctypes.wintypes.DWORD(0)
                ctypes.windll.kernel32.GetConsoleMode(h, ctypes.wintypes.LPDWORD(x))
                ctypes.windll.kernel32.SetConsoleMode(h, x.value | 0x04) # ENABLE_VIRTUAL_TERMINAL_PROCESSING
            except:
                pass



    def col_n(self, n, b):
        return (b"\033[%dm" % (91+n)) + b + b"\033[0m"


#t = Terminal()
#print(t.col_n(1,b"aa").decode('ascii'))
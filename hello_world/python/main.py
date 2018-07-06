
import sys
import os
import tkinter.messagebox

def main():
    print("Hello World!")

    with open("../test_file.txt") as f:
        buf = f.read()
    print("File:", buf)

    tkinter.messagebox.showinfo("Title", "Hello World!")

if __name__ == "__main__":
    sys.exit(main())
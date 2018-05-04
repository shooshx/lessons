import re

def main():
    exe = open(r"C:\projects\Gvahim\substr\ConsoleApplication1\Debug\ConsoleApplication1.exe", "rb").read()

    check = re.search("__S__.*__E__", exe)

    for i in xrange(*check.span()):
        print exe[i]
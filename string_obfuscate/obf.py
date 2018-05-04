import re

def main():
    exe = open(r"C:\projects\Gvahim\substr\ConsoleApplication1\Debug\ConsoleApplication1.exe", "rb").read()

    check = re.search("__S__.*__E__", exe)
    start, end = check.span()
    
    lexe = list(exe)
    for i in xrange(start, end-5):
        lexe[i] = chr(ord(exe[i]) ^ 0xFF)

    mexe = "".join(lexe)
    open(r"C:\projects\Gvahim\substr\ConsoleApplication1\Debug\obj_app.exe", "wb").write(mexe)
    print "saved!"
from ctypes import *

class StringStruct(Structure):
    _fields_ = [ ('ob_refcnt', c_size_t),
                 ('ob_type', c_void_p),
                 ('ob_size', c_size_t),
                 ('ob_shash', c_long),
                 ('ob_sstate', c_int),
                 ]
                 
def wrapString(s):
    ws = StringStruct.from_address(id(s))
    sz = ws.ob_size
    class TmpStringStruct(Structure):
        _fields_ = StringStruct._fields_ + [('ob_sval', c_byte*sz)]
    return TmpStringStruct.from_address(id(s))
    
    
                 
def main():
    s = "HelloWorld!"
    ws = wrapString(s)
    print ws.ob_sval
    ws.ob_sval[3] = ord('L')
    print s
    
          

if __name__ == "__main__":
    main()
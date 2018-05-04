import win32api #activestate
import sys

from ctypes import *


class DOS_HEADER(Structure):
    _fields_ = [("e_magic", c_ushort),
                ("e_cblp", c_ushort),
                ("e_cp", c_ushort),
                ("e_crlc", c_ushort),
                ("e_cparhdr", c_ushort),
                ("e_minalloc", c_ushort),
                ("e_maxalloc", c_ushort),
                ("e_ss", c_ushort),
                ("e_sp", c_ushort),
                ("e_csum", c_ushort),
                ("e_ip", c_ushort),
                ("e_cs", c_ushort),
                ("e_lfarlc", c_ushort),
                ("e_pvno", c_ushort),
                ("e_res", ARRAY(c_ushort, 4)),
                ("e_oemid", c_ushort),
                ("e_oeminfo", c_ushort),
                ("e_res2", ARRAY(c_ushort, 10)),
                ("e_lfanew", c_uint),
                ]

class FILE_HEADER(Structure):
    _fields_ = [("Machine", c_ushort),
                ("NumberOfSections", c_ushort),
                ("TimeDateStamp", c_uint),
                ("PointerToSymbolTable", c_uint),
                ("NumberOfSymbols", c_uint),
                ("SizeOfOptionalHeader", c_ushort),
                ("Characteristics", c_ushort),
                ]

class DATA_DIRECTORY(Structure):
    _fields_ = [("VirtualAddress", c_uint),
                ("Size", c_uint),
                ]             

class OPTIONAL_HEADER(Structure):
    _fields_ = [("Magic", c_ushort),
                ("MajorLinkerVersion", c_ubyte),
                ("MinorLinkerVersion", c_ubyte),
                ("SizeOfCode", c_uint),
                ("SizeOfInitializedData", c_uint),
                ("SizeOfUninitializedData", c_uint),
                ("AddressOfEntryPoint", c_uint),
                ("BaseOfCode", c_uint),
                ("BaseOfData", c_uint), 
                ("ImageBase", c_uint),   # in 64bit, this one is size_t and BaseOfData doesn't exist
                ("SectionAlignment", c_uint),
                ("FileAlignment", c_uint),
                ("MajorOperatingSystemVersion", c_ushort),
                ("MinorOperatingSystemVersion", c_ushort),
                ("MajorImageVersion", c_ushort),
                ("MinorImageVersion", c_ushort),
                ("MajorSubsystemVersion", c_ushort),
                ("MinorSubsystemVersion", c_ushort),
                ("Win32VersionValue", c_uint),
                ("SizeOfImage", c_uint),
                ("SizeOfHeaders", c_uint),
                ("CheckSum", c_uint),
                ("Subsystem", c_ushort),
                ("DllCharacteristics", c_ushort),
                ("SizeOfStackReserve", c_size_t),
                ("SizeOfStackCommit", c_size_t),
                ("SizeOfHeapReserve", c_size_t),
                ("SizeOfHeapCommit", c_size_t),
                ("LoaderFlags", c_uint),
                ("NumberOfRvaAndSizes", c_uint),
                ("DataDirectory", ARRAY(DATA_DIRECTORY, 16)), #IMAGE_NUMBEROF_DIRECTORY_ENTRIES
               ]

class NT_HEADERS(Structure):
     _fields_ = [("Signature", c_uint),
                 ("FileHeader", FILE_HEADER),
                 ("OptionalHeader", OPTIONAL_HEADER)
                ]
                 
class IMPORT_DESCRIPTOR(Structure):
    _fields_ = [("OriginalFirstThunk", c_uint),
                ("TimeDateStamp", c_uint),
                ("ForwarderChain", c_uint),
                ("Name", c_uint),
                ("FirstThunk", c_uint),
                ]


def parse(hmod):
    print hex(hmod)
    dh = DOS_HEADER.from_address(hmod)
    nth = NT_HEADERS.from_address(hmod + dh.e_lfanew)
    imp_offset = nth.OptionalHeader.DataDirectory[1].VirtualAddress
    import_dlls = []
    # go over dlls
    while True:
        imp = IMPORT_DESCRIPTOR.from_address(hmod + imp_offset)
        if imp.OriginalFirstThunk == 0:
            break;
        import_dlls.append(imp)
        name = c_char_p(hmod + imp.Name)
        print name.value
        imp_offset += sizeof(imp)

    print ''
    user32imp = import_dlls[0]
    # go over its functions
    index = 0
    funcNamesArray = hmod + user32imp.OriginalFirstThunk
    while True:
        nameAddr = c_size_t.from_address(funcNamesArray).value # skip hint 
        if nameAddr == 0:
            break;
        name = c_char_p(hmod + nameAddr + 2)
        print name.value
        funcNamesArray += sizeof(c_size_t)
        if name.value == 'MessageBoxExA':
            msgBoxIndex = index
        index += 1
            
    print msgBoxIndex
    funcImportAddr = hmod + user32imp.FirstThunk + msgBoxIndex * sizeof(c_size_t)
    funcAddr = c_size_t.from_address(funcImportAddr)
    
    global origFunc
    origFunc = MyFuncType(funcAddr.value)

    print hex(funcImportAddr), "->", hex(funcAddr.value)
    unprotect(funcImportAddr)
    #funcAddr.value = 0
    return funcAddr
    

def unprotect(addr):
    import win32con
    op = c_ulong()
    opp = byref(op)
    windll.kernel32.VirtualProtect(addr, 8, win32con.PAGE_EXECUTE_READWRITE, opp)


MyFuncType = WINFUNCTYPE(c_int, c_size_t, c_char_p, c_char_p, c_uint, c_ushort)    
origFunc = None
    
def myFunc(a,b,c,d,e):
    print "HELLO!!", a,b,c,d,e
    return origFunc(a,"XXX" + b + "XXX",c,d,e)    
    
def do():
    import win32api
    hmod = win32api.GetModuleHandle('win32api.pyd')
    x = parse(hmod)
    
    
    cfunc = MyFuncType(myFunc)
    funcptr = cast(cfunc, c_void_p)
    print hex( funcptr.value)
    x.value = funcptr.value
    
    
    win32api.MessageBox(0, "hello", "caption", 0)
    
    
    
if __name__ == "__main__":
    do()

#windll.kernel32.VirtualProtect(x.value, 8, win32con.PAGE_EXECUTE_READWRITE, 0)


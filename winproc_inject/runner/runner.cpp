#include <Windows.h>

#include <iostream>

using namespace std;



int main()
{
    HWND npHwnd = FindWindowEx(NULL, NULL, L"Notepad", NULL);
    if (npHwnd == NULL) {
        cout << "Process not found" << endl;
        return -1;
    }
    DWORD pid = 0;
    GetWindowThreadProcessId(npHwnd, &pid);


    HANDLE proc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
    if (proc == NULL) {
        cout << "Failed OpenProcess" << endl;
        return -1;
    }

    void* remote_addr = VirtualAllocEx(proc, 0, 1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (remote_addr == NULL) {
        cout << "Failed VirtualAllocEx" << endl;
        return -1;
    }

    string dll_name = "C:\\Projects\\kygit\\my_lsn\\winproc_inject\\x64_Debug\\remote_dll.dll";
    
    SIZE_T written = 0;

    if (WriteProcessMemory(proc, remote_addr, dll_name.data(), dll_name.size() + 1, &written) == 0) {
        cout << "Failed WriteProcessMemory" << endl;
        return -1;
    }

    DWORD tid = 0;
    HANDLE th = CreateRemoteThread(proc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, remote_addr, 0, &tid);
    if (th == NULL) {
        cout << "Failed CreateRemoteThread" << endl;
        return -1;
    }

    CloseHandle(proc);

    cout << "done!" << endl;


    return 0;
}


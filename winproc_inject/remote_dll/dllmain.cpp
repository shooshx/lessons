
#include <windows.h>

#include <fstream>

using namespace std;


LONG_PTR prev_ptr = 0;
ifstream file;

LRESULT CALLBACK myWindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) 
{
    if (uMsg == WM_CHAR) {
        char c = 0;
        file.read(&c, 1);
        wParam = c;
    }
    //fprintf(log, "%x -- %d %d\n", (int)uMsg, (int)wParam, (int)lParam);
    //fflush(log);
    return CallWindowProcA((WNDPROC)prev_ptr, hwnd, uMsg, wParam, lParam);
}



void hijackMessages()
{
    file.open("C:\\Projects\\kygit\\my_lsn\\winproc_inject\\messages.h");
    //log = fopen("C:\\Projects\\lessons\\kygit\\test\\remote_dll\\log.txt", "w");

    HWND npHwnd = FindWindowEx(NULL, NULL, L"Notepad", NULL);;
    HWND hEditWnd = GetWindow(npHwnd, GW_CHILD);

    prev_ptr = GetWindowLongPtrA(hEditWnd, GWLP_WNDPROC);
    SetWindowLongPtrA(hEditWnd, GWLP_WNDPROC, (LONG_PTR)myWindowProc);

}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        hijackMessages();
    }
    return TRUE;
}


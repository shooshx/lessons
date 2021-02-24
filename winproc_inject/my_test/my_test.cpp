
#include <windows.h>
#include <iostream>

using namespace std;


LRESULT CALLBACK myWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    cout << "Got message " << uMsg << "  params: " << wParam << "," << lParam << endl;

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}



int main()
{
    cout << "Creating window" << endl;
    HWND hwnd = CreateWindowA("STATIC", "Hello World!", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 500, 100, NULL, NULL, NULL, NULL);

    SetWindowLongPtrA(hwnd, GWLP_WNDPROC, (LONG_PTR)myWindowProc);

    while(true)
    {
        MSG msg;
        GetMessage(&msg, NULL, 0, 0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;

}


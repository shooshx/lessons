#include <windows.h>
#include <iostream>

using namespace std;

LRESULT CALLBACK myWindowProc(HWND hwnd, UINT msg,  WPARAM wParam, LPARAM lParam) {
    //cout << hex << "0x" << msg << endl;
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main(int argc, char* argv[])
{
    HWND h = CreateWindowExA(0, (const char*)WC_DIALOG, "Hello", 
                             WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION, // | WS_SIZEBOX, 
                             CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, NULL, NULL);
    SetWindowLongPtr(h, GWLP_WNDPROC, (LONG)myWindowProc);

    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255,255,255));
    while (true) {
        MSG msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) {
                return 0;
            }

            //TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            Sleep(100);
            HDC hdc = GetDC(h);
            SelectObject(hdc, GetStockObject(BLACK_BRUSH));
            Rectangle(hdc, 0, 0, 800, 600);
            SelectObject(hdc, pen);

            MoveToEx(hdc, 400, 300, NULL);
            LineTo(hdc, 500, 300);

            ReleaseDC(h, hdc);
        }
    }

	return 0;
}


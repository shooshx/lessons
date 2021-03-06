#include <windows.h>


int main()
{
	DWORD err = 0;
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteFile(hout, "Hello World!\n", 13, NULL, NULL);

	HANDLE hf = CreateFileA("../test_file.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hf == INVALID_HANDLE_VALUE) {
		err = GetLastError();
		return 1;
	}
	
	DWORD sz = GetFileSize(hf, NULL);
	char* buf = (char*)LocalAlloc(0, sz);
	DWORD numRead = 0;
	if (!ReadFile(hf, buf, sz, &numRead, NULL)) {
		err = GetLastError();
		goto done;
	}
	WriteFile(hout, "File:", 5, NULL, NULL);
	WriteFile(hout, buf, sz, NULL, NULL);

	MessageBoxA(NULL, "Hello World", "Hello", MB_OK);

  done:
	LocalFree(buf);
	CloseHandle(hf);

	return err;
}


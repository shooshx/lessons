#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma warning (disable: 4104) 
#define DLL_EXP extern "C" HRESULT __declspec(dllexport) __stdcall 
// define them first with the correct linkage since they are defined incorrectly in the following includes
DLL_EXP DllCanUnloadNow();
DLL_EXP DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppReturn);
DLL_EXP DllRegisterServer();
DLL_EXP DllUnregisterServer();

#include <shlobj.h>
#include <olectl.h>


#define GUID_STR L"{1F151065-B5A0-4A89-95B8-6DCCD0918D54}"
GUID CLSID_HelloExtNoAtl = { 0x1f151065, 0xb5a0, 0x4a89, 0x95, 0xb8, 0x6d, 0xcc, 0xd0, 0x91, 0x8d, 0x54 };

#define EXT_NAME L"MyHelloExt"



// data
HINSTANCE   g_hInst;
UINT        g_DllRefCount;


DWORD WINAPI threadMain(LPVOID lpParameter);

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) 
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			g_hInst = (HINSTANCE)hModule;
            CreateThread(NULL, 0, threadMain, 0, 0, NULL);
            break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


class CShellExt : public IShellExtInit //, IContextMenu
{
protected:
    DWORD m_ObjRefCount;
public:
    CShellExt::CShellExt() {
    }

    virtual CShellExt::~CShellExt() {
    }

    // IUnknown methods
    STDMETHODIMP_(DWORD) Release(void) {
        if (--m_ObjRefCount == 0) {
            delete this;
            return 0;
        }
        return m_ObjRefCount;
    }

    STDMETHODIMP_(DWORD) AddRef(void) {
        return ++m_ObjRefCount;
    }

    STDMETHODIMP QueryInterface(REFIID riid, LPVOID *ppReturn)
    {
        *ppReturn = NULL;
        if (IsEqualIID(riid, IID_IUnknown))
            *ppReturn = this;
        else if (IsEqualIID(riid, IID_IClassFactory))
            *ppReturn = (IClassFactory*)this;
        else if (IsEqualIID(riid, IID_IShellExtInit))
            *ppReturn = (IShellExtInit*)this;
        else 
            return E_NOINTERFACE;

        LPUNKNOWN pUnk = (LPUNKNOWN)(*ppReturn);
        pUnk->AddRef();
        return S_OK;
    }

    // IShellExtInit
    STDMETHODIMP Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID)
    {
        return S_OK;
    }
};


class CClassFactory : public IClassFactory
{
protected:
    DWORD m_ObjRefCount;

public:
    CClassFactory() {
        m_ObjRefCount = 1;
        g_DllRefCount++;
    }

    virtual ~CClassFactory() {
        g_DllRefCount--;
    }

    //IUnknown methods
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID *ppReturn)
    {
        *ppReturn = NULL;
        if (IsEqualIID(riid, IID_IUnknown))
            *ppReturn = this;
        else if (IsEqualIID(riid, IID_IClassFactory))
            *ppReturn = (IClassFactory*)this;
        else
            return E_NOINTERFACE;

        LPUNKNOWN pUnk = (LPUNKNOWN)(*ppReturn);
        pUnk->AddRef();
        return S_OK;
    }

    STDMETHODIMP_(DWORD) AddRef() {
        return ++m_ObjRefCount;
    }

    STDMETHODIMP_(DWORD) Release() {
        if (--m_ObjRefCount == 0) {
            delete this;
            return 0;
        }
        return m_ObjRefCount;
    }

    //IClassFactory methods
    STDMETHODIMP CreateInstance(LPUNKNOWN pUnknown, REFIID riid, LPVOID *ppObject)
    {
        *ppObject = NULL;
        if (pUnknown != NULL)
            return CLASS_E_NOAGGREGATION;

        // creates the namespace's main class
        CShellExt *pShellExt = new CShellExt();
        if (NULL == pShellExt)
            return E_OUTOFMEMORY;

        // query interface for the return value
        HRESULT hResult = pShellExt->QueryInterface(riid, ppObject);
        pShellExt->Release();
        return hResult;
    }

    STDMETHODIMP LockServer(BOOL) {
        return E_NOTIMPL;
    };

};


DLL_EXP DllCanUnloadNow() {
	return (g_DllRefCount ? S_FALSE : S_OK);
}



DLL_EXP DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppReturn)
{
	*ppReturn = NULL;

	// do we support the CLSID?
	if( !IsEqualCLSID(rclsid, CLSID_HelloExtNoAtl) )
	   return CLASS_E_CLASSNOTAVAILABLE;
   
	// call the factory
	CClassFactory *pClassFactory = new CClassFactory();
	if (pClassFactory == NULL)
	   return E_OUTOFMEMORY;
   
	// query interface for the a pointer
	HRESULT hResult = pClassFactory->QueryInterface(riid, ppReturn);
	pClassFactory->Release();
	return hResult;
}



struct REGSTRUCT{
   HKEY  hRootKey;
   LPWSTR lpszSubKey;
   LPWSTR lpszValueName;
   LPWSTR lpszData;
};

REGSTRUCT ClsidEntries[] = {  
	HKEY_CLASSES_ROOT, L"CLSID\\" GUID_STR,                      NULL,               EXT_NAME,
//	HKEY_CLASSES_ROOT, L"CLSID\\" GUID_STR,                      L"InfoTip",         EXT_NAME L".",
	HKEY_CLASSES_ROOT, L"CLSID\\" GUID_STR L"\\InprocServer32",  NULL,               L"%s",
    HKEY_CLASSES_ROOT, L"CLSID\\" GUID_STR L"\\InprocServer32",  L"ThreadingModel",  L"Apartment",
//  HKEY_CLASSES_ROOT, L"CLSID\\" GUID_STR L"\\DefaultIcon",     NULL,               L"%s,0",
    HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers\\" EXT_NAME, NULL,     GUID_STR,
    HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved",                       GUID_STR, EXT_NAME
};

//"txtfile\\ShellEx\\ContextMenuHandlers\\" EXT_NAME

DLL_EXP DllRegisterServer()
{
	WCHAR szModule[MAX_PATH];
	GetModuleFileNameW(g_hInst, szModule, ARRAYSIZE(szModule) );

	for(int i = 0; i < _countof(ClsidEntries); ++i) {
        HKEY hKey;
        DWORD dwDisp;
        LRESULT lResult = RegCreateKeyExW(ClsidEntries[i].hRootKey, ClsidEntries[i].lpszSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisp);
		if (lResult == 0) {
			 WCHAR szData[MAX_PATH];
			 wsprintfW(szData, ClsidEntries[i].lpszData, szModule);

			 lResult = RegSetValueExW(hKey, ClsidEntries[i].lpszValueName, 0, REG_SZ, (LPBYTE)szData, (lstrlenW(szData) + 1) * sizeof(WCHAR));       
			 RegCloseKey(hKey);
		}
		else
			return SELFREG_E_CLASS;
    }

   return S_OK;
}


DLL_EXP DllUnregisterServer()
{
	WCHAR szModule[MAX_PATH];
	GetModuleFileNameW(g_hInst, szModule, ARRAYSIZE(szModule) );

    LRESULT lResult;
	for(int i = 0; i < _countof(ClsidEntries) - 1; ++i) {
		lResult = RegDeleteKeyW(ClsidEntries[i].hRootKey, ClsidEntries[i].lpszSubKey);
    }

    // only remove value, not key
    int entryIndex = _countof(ClsidEntries) - 1;
    HKEY hKey;
    lResult = RegOpenKeyExW(ClsidEntries[entryIndex].hRootKey, ClsidEntries[entryIndex].lpszSubKey, 0, KEY_WRITE, &hKey);
    if (lResult == 0) {
        lResult = RegDeleteValueW(hKey, ClsidEntries[entryIndex].lpszValueName);
        RegCloseKey(hKey);
    }

	return S_OK;
}

#ifndef DLLFROMMEM_H
#define DLLFROMMEM_H

typedef BOOL(WINAPI *myDllMain)(
	HINSTANCE hinstDLL,  // handle to the DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpvReserved   // reserved
	);
class CDllFromMem
{
public:
	CDllFromMem();
	virtual ~CDllFromMem();
	HANDLE LoadLibraryFromRs(HINSTANCE hInstance, LPCTSTR lpType, LPCTSTR lpName);
	FARPROC GetProcAddressFromRs(LPCSTR lpProcName);
private:
	HANDLE m_hBaseAddress;
	HINSTANCE m_hInstance;
	IMAGE_NT_HEADERS *m_ntHead;
	myDllMain m_dllMain;
	void PatchData();
};
#endif 
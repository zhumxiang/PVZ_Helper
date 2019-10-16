#include "stdafx.h"
#include "windows.h"
#include "DllFromMem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDllFromMem::CDllFromMem()
{
	m_hBaseAddress = NULL;
	m_hInstance = NULL;
	m_ntHead = NULL;
}
CDllFromMem::~CDllFromMem()
{
	if (m_hBaseAddress)
	{

		m_dllMain(m_hInstance, DLL_PROCESS_DETACH, NULL);
		VirtualFree(m_hBaseAddress, 0, MEM_RELEASE);
	}

}
HANDLE CDllFromMem::LoadLibraryFromRs(HINSTANCE hInstance, LPCTSTR lpType, LPCTSTR lpName)
{
	m_hInstance = hInstance;
	HRSRC hrec = FindResource(NULL, lpName, lpType);
	HRSRC hResLoad = (HRSRC)LoadResource(NULL, hrec);
	unsigned char *LockRes = (unsigned char *)LockResource(hResLoad);
	IMAGE_DOS_HEADER *dosHead;
	dosHead = (IMAGE_DOS_HEADER*)LockRes;
	m_ntHead = (IMAGE_NT_HEADERS *)(dosHead->e_lfanew + (int)dosHead);
	if (dosHead->e_magic != 0x5a4d && m_ntHead->Signature != 0x4550)
	{
		MessageBox(NULL, "指定的资源不是有效的DLL文件!", "装入动态链接库出错", MB_OK | MB_ICONERROR);
		return NULL;
	}
	int SizeOfImage = m_ntHead->OptionalHeader.SizeOfImage;
	m_hBaseAddress = VirtualAlloc((LPVOID)(m_ntHead->OptionalHeader.ImageBase), SizeOfImage,/*MEM_COMMIT*/MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!m_hBaseAddress)
	{
		m_hBaseAddress = VirtualAlloc(NULL, SizeOfImage, MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	}
	VirtualAlloc(m_hBaseAddress, SizeOfImage, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	int SizeOfHeaders = m_ntHead->OptionalHeader.SizeOfHeaders;
	int FileAlignment = m_ntHead->OptionalHeader.FileAlignment;
	memcpy(m_hBaseAddress, LockRes, SizeOfHeaders);
	IMAGE_SECTION_HEADER *sectionHead = (IMAGE_SECTION_HEADER *)(sizeof(IMAGE_NT_HEADERS)+(int)m_ntHead);
	int NumberOfSections = m_ntHead->FileHeader.NumberOfSections;
	LPVOID desc, src;
	int iSize;
	for (int i = 0; i < NumberOfSections; i++) //拷贝区段
	{
		src = LockRes + sectionHead[i].PointerToRawData;
		if (sectionHead[i].SizeOfRawData == 0) continue;
		desc = (LPVOID)((DWORD)m_hBaseAddress + sectionHead[i].VirtualAddress);
		if (i < NumberOfSections - 1)
			iSize = sectionHead[i + 1].PointerToRawData - sectionHead[i].PointerToRawData;
		else
			iSize = sectionHead[i].SizeOfRawData;
		memcpy(desc, src, iSize);
	}
	//以下是重定位

	DWORD *tmp;
	if (m_ntHead->OptionalHeader.DataDirectory[5].VirtualAddress)  //如果没有重定位表表示不用重定位，跳过重定位代码
	{
		DWORD relocation = (DWORD)m_hBaseAddress - m_ntHead->OptionalHeader.ImageBase;
		IMAGE_BASE_RELOCATION  *relocationAddress = (IMAGE_BASE_RELOCATION*)(m_ntHead->OptionalHeader.DataDirectory[5].VirtualAddress + (DWORD)m_hBaseAddress);

		while (relocationAddress->VirtualAddress != 0)
		{
			LPVOID rva = (LPVOID)((DWORD)m_hBaseAddress + relocationAddress->VirtualAddress);
			DWORD BlockNum = (relocationAddress->SizeOfBlock - 8) / 2;
			if (BlockNum == 0) break;
			WORD *Offset = (WORD *)((DWORD)relocationAddress + 8);
			for (int i = 0; i < (int)BlockNum; i++)
			{
				if ((Offset[i] & 0xF000) != 0x3000) continue;
				tmp = (DWORD*)((Offset[i] & 0xFFF) + (DWORD)rva);
				*tmp = (*tmp) + relocation;
			}
			relocationAddress = (IMAGE_BASE_RELOCATION*)((DWORD)relocationAddress + relocationAddress->SizeOfBlock);

		}

	}
	//重定位结束

	//以下是重建IAT
	char MsgError[256] = { 0 };
	if (m_ntHead->OptionalHeader.DataDirectory[1].VirtualAddress) //如果没用导入表则跳过
	{
		HMODULE lib;
		IMAGE_THUNK_DATA *IAT, *INTable;
		IMAGE_IMPORT_BY_NAME *IatByName;
		IMAGE_IMPORT_DESCRIPTOR *importAdd = (IMAGE_IMPORT_DESCRIPTOR *)((DWORD)m_hBaseAddress + m_ntHead->OptionalHeader.DataDirectory[1].VirtualAddress);

		while (importAdd->Name)//(importAdd->FirstThunk)
		{
			lib = LoadLibrary((char *)(importAdd->Name + (DWORD)m_hBaseAddress));
			if (lib == NULL) //装载库出错
			{
				wsprintf(MsgError, "装入动态链接库%s出错!", (char *)(importAdd->Name + (DWORD)m_hBaseAddress));
				MessageBox(NULL, MsgError, "错误", MB_OK | MB_ICONERROR);
				return NULL;
			}
			IAT = (IMAGE_THUNK_DATA *)(importAdd->FirstThunk + (DWORD)m_hBaseAddress);
			INTable = (IMAGE_THUNK_DATA *)((importAdd->OriginalFirstThunk ? importAdd->OriginalFirstThunk : importAdd->FirstThunk) + (DWORD)m_hBaseAddress);
			while (INTable->u1.AddressOfData)
			{
				if (((DWORD)INTable->u1.Function & 0x800000) == 0)
				{
					IatByName = (IMAGE_IMPORT_BY_NAME *)((DWORD)INTable->u1.AddressOfData + (DWORD)m_hBaseAddress);
					IAT->u1.Function = (DWORD)GetProcAddress(lib, (char *)(IatByName->Name));
				}
				else
				{
					IAT->u1.Function = (DWORD)GetProcAddress(lib, (LPCSTR)(INTable->u1.Ordinal ^ 0x7FFFFF));
				}
				INTable++;
				IAT++;
			}
			importAdd++;
		}
	}
	//重建IAT结束
	//调用dll的入口函数
	m_dllMain = (myDllMain)(m_ntHead->OptionalHeader.AddressOfEntryPoint + (DWORD)m_hBaseAddress);
	m_dllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL);
	//PatchData();//给dll打补丁，如果不需要打补丁，这行可以注释掉;
	return m_hBaseAddress;
}
FARPROC CDllFromMem::GetProcAddressFromRs(LPCSTR lpProcName)
{
	IMAGE_EXPORT_DIRECTORY *ExportTable = (IMAGE_EXPORT_DIRECTORY *)(m_ntHead->OptionalHeader.DataDirectory[0].VirtualAddress + (DWORD)m_hBaseAddress);
	DWORD *ExportNameAddress = (DWORD*)(ExportTable->AddressOfNames + (DWORD)m_hBaseAddress);
	WORD *AddressOfNameOrdinals = (WORD*)(ExportTable->AddressOfNameOrdinals + (DWORD)m_hBaseAddress);
	DWORD *AddressOfFunction = (DWORD*)(ExportTable->AddressOfFunctions + (DWORD)m_hBaseAddress);
	char *ExportName;

	DWORD j = 0;//要判断是否是以序号查找的
	DWORD i = ExportTable->NumberOfNames;
	if ((DWORD)lpProcName > 0xFFFF) //以名字查找
	{
		for (j = 0; j < i; j++)
		{
			ExportName = (char *)((DWORD)*ExportNameAddress + (DWORD)m_hBaseAddress);
			if (!strcmpi(lpProcName, ExportName))
				return (FARPROC)(AddressOfFunction[AddressOfNameOrdinals[j]] + (DWORD)m_hBaseAddress);
			ExportNameAddress++;
		}
		return NULL;
	}
	else //以序号查找;
	{
		j = (DWORD)lpProcName - ExportTable->Base;
		if (j >= ExportTable->NumberOfFunctions) return NULL;
		return (FARPROC)(AddressOfFunction[j] + (DWORD)m_hBaseAddress);
	}
	return NULL;
}
void CDllFromMem::PatchData()
{
	DWORD PatchContent[][2] = { 0x0002CA94, 0x0 };//rva,patchdata;
	int i = sizeof(PatchContent) / (sizeof(DWORD)* 2);
	for (int j = 0; j < i; j++)
	{
		*(char*)((char*)m_hBaseAddress + PatchContent[j][0]) = (char)PatchContent[j][1];
	}
}
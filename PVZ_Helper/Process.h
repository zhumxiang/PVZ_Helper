#pragma once

#include "windows.h"
#include <functional>

enum { NOT_FOUND, OPEN_FAILED, SUCCESS };

class Process
{
public:
	Process() { hpro = NULL; }
	Process(DWORD pid){ OpenByPid(pid); }
	Process(const Process& p)
	{
		if (p.IsValid())
		{ 
			DuplicateHandle(GetCurrentProcess(), p.GetHandle(), GetCurrentProcess(), &hpro, 0, TRUE, DUPLICATE_SAME_ACCESS);
		}
		else
		{
			hpro = NULL;
		}
	}
	int OpenByPid(DWORD pid);
	int OpenByWindow(const char* Title, const char* Class = NULL);
	int OpenByName(const char* Name);
	~Process(){ CloseHandle(hpro); hpro = NULL; }
	bool IsValid()const { return hpro ? true : false; }
	DWORD ReadMemory(DWORD size, DWORD base, int argnum = 0, ...);
	void WriteMemory(DWORD data, DWORD size, DWORD base, int argnum = 0, ...);
	void WriteArrayMemory(void* data, DWORD size, DWORD addr);
	template<class T, int size>
	void WriteArrayMemory(T(&array_t)[size], DWORD addr){ WriteArrayMemory(&array_t[0], sizeof(array_t), addr); }
	int pid;
	HANDLE GetHandle()const { return hpro; }
	static DWORD GetProcessId(const char* Title, const char* Class = NULL);
	static DWORD LookupProcess(std::function<bool(const char*, DWORD)> forOne);
private:
	HANDLE hpro;
};


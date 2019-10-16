#include "stdafx.h"
#include "Asm.h"

static const BYTE offset[] = { 0, 3, 1, 2, 6, 7, 5 };

void Asm::runThread(HANDLE hpro, DWORD ThreadAddr)
{
	bool NeedFree;
	if (ThreadAddr == 0)
	{
		ThreadAddr = (DWORD)VirtualAllocEx(hpro, NULL, index, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		NeedFree = true;
	}
	else
	{
		NeedFree = false;
	}
	if (ThreadAddr)
	{
		WriteProcessMemory(hpro, (LPVOID)ThreadAddr, code, index, 0);
		HANDLE hth = CreateRemoteThread(hpro, NULL, 0, LPTHREAD_START_ROUTINE(ThreadAddr), NULL, 0, NULL);
		WaitForSingleObject(hth, INFINITE);
		CloseHandle(hth);
		if (NeedFree)
			VirtualFreeEx(hpro, (LPVOID)ThreadAddr, 0, MEM_RELEASE);
	}
}

void Asm::push_dword(DWORD val)
{
	add_byte(0x68);
	add_dword(val);
}
void Asm::push_byte(BYTE val)
{
	add_byte(0x6a);
	add_byte(val);
}
void Asm::mov_exx(RegType exx, DWORD val)
{
	add_byte(0xb8 + offset[int(exx)]);
	add_dword(val);
}
void Asm::mov_exx_dword_ptr(RegType exx, DWORD val)
{
	add_byte(0x8b);
	add_byte(5 + offset[int(exx)] * 8);
	add_dword(val);
}

void Asm::mov_exx_dword_ptr_eyy_add_byte(RegType exx, RegType eyy, BYTE val)
{
	add_byte(0x8b);
	add_byte(0x40 + offset[int(exx)] * 8 + offset[int(eyy)]);
	add_dword(val);
}

void Asm::mov_exx_dword_ptr_eyy_add_dword(RegType exx, RegType eyy, DWORD val)
{
	add_byte(0x8b);
	add_byte(0x80 + offset[int(exx)] * 8 + offset[int(eyy)]);
	add_dword(val);
}
void Asm::push_exx(RegType exx)
{
	add_byte(0x50 + offset[int(exx)]);
}
void Asm::pop_exx(RegType exx)
{
	add_byte(0x58 + offset[int(exx)]);
}
void Asm::ret()
{
	add_byte(0xc3);
}
void Asm::call(DWORD addr)
{
	add_byte(0xe8);
	add_dword(2);
	add_word(0x06eb);
	push_dword(addr);
	ret();
}
void Asm::mov_exx_eyy(RegType exx, RegType eyy)
{
	push_exx(eyy);
	pop_exx(exx);
}
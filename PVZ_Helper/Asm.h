#pragma once
#include "windows.h"
#include "assert.h"

enum class RegType
{
	EAX,
	EBX,
	ECX,
	EDX,
	ESI,
	EDI,
	EBP,
};

class Asm
{
public:
	Asm(int page = 1)
	{
		maxLength = 4096 * page;
		code = new BYTE[maxLength];
		index = 0;
	}
	~Asm(){ delete[] code; }
	void clear(){ index = 0; }
	int getLength(){ return index; }
	void runThread(HANDLE hpro, DWORD ThreadAddr);
	void add_byte(BYTE val)
	{
		assert(index + 1 < maxLength);
		code[index++] = val;
	}
	void add_word(WORD val)
	{
		assert(index + 2 < maxLength);
		(WORD&)code[index] = val, index += 2;
	}
	void add_dword(DWORD val)
	{
		assert(index + 4 < maxLength);
		(DWORD&)code[index] = val, index += 4;
	}
	void push_dword(DWORD val);
	void push_byte(BYTE val);
	void mov_exx(RegType exx, DWORD val);
	void mov_exx_dword_ptr(RegType exx, DWORD val);
	void mov_exx_dword_ptr_exx_add_byte(RegType exx, BYTE val){ mov_exx_dword_ptr_eyy_add_byte(exx, exx, val); }
	void mov_exx_dword_ptr_eyy_add_byte(RegType exx, RegType eyy, BYTE val);
	void mov_exx_dword_ptr_exx_add_dword(RegType exx, DWORD val){mov_exx_dword_ptr_eyy_add_dword(exx, exx, val);}
	void mov_exx_dword_ptr_eyy_add_dword(RegType exx, RegType eyy, DWORD val);
	void push_exx(RegType exx);
	void pop_exx(RegType exx);
	void call(DWORD addr);
	void ret();
	void mov_exx_eyy(RegType exx, RegType eyy);
private:
	BYTE* code;
	int index;
	DWORD maxLength;
};
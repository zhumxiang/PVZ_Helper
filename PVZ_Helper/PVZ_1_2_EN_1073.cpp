#include "stdafx.h"
#include "PVZ_1_2_EN_1073.h"

#define DATA_ADDR 0x00729670
#define OFFSET_1 0x868
#define OFFSET_2 0x178
#define MODE_OFFSET 0x918
#define GARDON_OFFSET 0x94c
#define EXCODE_SAVE_ADDR 0x00429a64

void PVZ_1_2_EN_1073::RunAsm()
{
	if (getLength() > 0)
	{
		WriteMemory(-2, 1, 0x005d6c6e);
		Sleep(10);
		ret();
		runThread(GetHandle(), eCode.Thread);
		WriteMemory(0xc8, 1, 0x005d6c6e);
	}
}
void PVZ_1_2_EN_1073::GetPlants()
{
	vp.clear();
	Plant one;
	one.addr = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0xc4);
	int pnum = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0xd4);
	for (int i = 0; i < pnum; one.addr += 0x14c)
	{
		if (ReadMemory(1, one.addr + 0x141) == 0)
		{
			one.Row = ReadMemory(4, one.addr + 0x1c);
			one.Col = ReadMemory(4, one.addr + 0x28);
			one.Type = ReadMemory(4, one.addr + 0x24);
			one.Sleeping = ReadMemory(1, one.addr + 0x143) == 1;
			vp.push_back(one);
			++i;
		}
	}
}
void PVZ_1_2_EN_1073::GetItems()
{
	vi.clear();
	Item one;
	one.addr = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x134);
	int inum = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x144);
	for (int i = 0; i < inum; one.addr += 0xec)
	{
		if (ReadMemory(1, one.addr + 0x20) == 0)
		{
			one.Row = ReadMemory(4, one.addr + 0x14);
			one.Col = ReadMemory(4, one.addr + 0x10);
			one.Type = ReadMemory(4, one.addr + 8);
			vi.push_back(one);
			++i;
		}
	}
}
void PVZ_1_2_EN_1073::GetCars()
{
	vc.clear();
	Car one;
	one.addr = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x118);
	int cnum = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x128);
	for (int i = 0; i < cnum; one.addr += 0x48)
	{
		if (ReadMemory(1, one.addr + 0x30) == 0)
		{
			one.Row = ReadMemory(4, one.addr + 0x14);
			vc.push_back(one);
			++i;
		}
	}
}
void PVZ_1_2_EN_1073::GetZombies()
{
	vz.clear();
	Zombie one;
	one.addr = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0xa8);
	int znum = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0xb8);
	for (int i = 0; i < znum; one.addr += 0x168)
	{
		if (ReadMemory(1, one.addr + 0xec) == 0)
		{
			one.Row = ReadMemory(4, one.addr + 0x1c);
			one.Type = ReadMemory(4, one.addr + 0x24);
			vz.push_back(one);
			++i;
		}
	}
}
int PVZ_1_2_EN_1073::GetMapType()
{
	return ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x5564); 
}
int PVZ_1_2_EN_1073::GetBlockType(int row, int col)/*1.草地2.裸地3.水池*/
{
	return ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x180 + row * 4 + col * 24);
}
void PVZ_1_2_EN_1073::SetBlockType(int row, int col, int type)/*1.草地2.裸地3.水池*/
{
	WriteMemory(type, 4, DATA_ADDR, 2, OFFSET_1, 0x180 + row * 4 + col * 24);
}
int PVZ_1_2_EN_1073::GetRowType(int row)/*0.不出怪1.草地2.水路*/
{
	return ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x5f0 + row * 4);
}
void PVZ_1_2_EN_1073::SetRowType(int row, int type)/*0.不出怪1.草地2.水路*/
{
	WriteMemory(type, 4, DATA_ADDR, 2, OFFSET_1, 0x5f0 + row * 4);
}
bool PVZ_1_2_EN_1073::InGame()
{
	return ReadMemory(4, DATA_ADDR, 1, OFFSET_1) != 0;
}

int PVZ_1_2_EN_1073::GetMode()
{
	return ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET);
}

void PVZ_1_2_EN_1073::SetSun(int sun)
{
	WriteMemory(sun, 4, DATA_ADDR, 2, OFFSET_1, 0x5578);
}

void PVZ_1_2_EN_1073::SetMoney(int money)
{
	WriteMemory(money, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x50);
}

void PVZ_1_2_EN_1073::IgnoreSun(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0041e842);
		WriteMemory(0x39, 1, 0x0041e846);
		WriteMemory(0x91, 1, 0x0041e890);
		WriteMemory(0x80, 1, 0x0042a502);
		WriteMemory(0x80, 1, 0x0042a854);
		WriteMemory(0xeb, 1, 0x0042e1a1);
	}
	else
	{
		WriteMemory(0x7f, 1, 0x0041e842);
		WriteMemory(0x89, 1, 0x0041e846);
		WriteMemory(0x9e, 1, 0x0041e890);
		WriteMemory(0x8f, 1, 0x0042a502);
		WriteMemory(0x8f, 1, 0x0042a854);
		WriteMemory(0x75, 1, 0x0042e1a1);
	}
}

void PVZ_1_2_EN_1073::AutoPickup(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x004342f2);
	}
	else
	{
		WriteMemory(0x75, 1, 0x004342f2);
	}
}

void PVZ_1_2_EN_1073::StartCar()
{
	if (InGame())
	{
		clear();
		GetCars();
		for (auto& rc : vc)
		{
			mov_exx(RegType::ESI, rc.addr);
			call(0x0045c660);
		}
		RunAsm();
	}
}

void PVZ_1_2_EN_1073::ResetCar()
{
	if (InGame())
	{
		clear();
		GetCars();
		for (auto& rc : vc)
		{
			mov_exx(RegType::EAX, rc.addr);
			call(0x0045c5d0);
		}
		WriteMemory(0x60eb, 2, 0x0040e52f);
		WriteMemory(1, 1, 0x0040e5b7);
		WriteMemory(0xe1e4, 2, 0x0045b8d2);
		mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
		push_exx(RegType::EAX);
		call(0x0040e510);
		RunAsm();
		WriteMemory(0x0975, 2, 0x0040e52f);
		WriteMemory(0, 1, 0x0040e5b7);
		WriteMemory(0xe3c8, 2, 0x0045b8d2);
	}
}

void PVZ_1_2_EN_1073::asm_plant(int row, int col, int type, bool copy)
{
	if (copy)
	{
		push_byte(type);
		push_byte(48);
		mov_exx(RegType::EAX, row);
		push_byte(col);
		mov_exx_dword_ptr(RegType::EBP, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::EBP, OFFSET_1);
		push_exx(RegType::EBP);
		call(0x0040fa10);
		mov_exx_eyy(RegType::ESI, RegType::EAX);
		push_exx(RegType::EAX);
		call(0x0046a630);
		pop_exx(RegType::EAX);
	}
	else
	{
		push_byte(-1);
		push_byte(type);
		mov_exx(RegType::EAX, row);
		push_byte(col);
		mov_exx_dword_ptr(RegType::EBP, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::EBP, OFFSET_1);
		push_exx(RegType::EBP);
		call(0x0040fa10);
	}
}

void PVZ_1_2_EN_1073::asm_plant_paper(int row, int col, int type)
{
	asm_plant(row, col, type, false);
	push_exx(RegType::EAX);
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_2);
	call(0x0042d0d0);
}

void PVZ_1_2_EN_1073::NoPause(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x0045272a);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0045272a);
	}
}

void PVZ_1_2_EN_1073::asm_put_zombie(int row, int col, int type)
{
	push_byte(col);
	push_byte(type);
	mov_exx(RegType::EAX, row);
	mov_exx_dword_ptr(RegType::ECX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_2);
	call(0x0042cc90);
}

void PVZ_1_2_EN_1073::asm_put_boss()
{
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	push_byte(0);
	push_byte(25);
	call(0x00410700);
}

void PVZ_1_2_EN_1073::asm_put_ladder(int row, int col)
{
	mov_exx(RegType::EDI, row);
	push_byte(col);
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	call(0x0040b870);
}

void PVZ_1_2_EN_1073::asm_put_grave(int row, int col)
{
	mov_exx_dword_ptr(RegType::ECX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_2);
	push_exx(RegType::ECX);
	mov_exx(RegType::EDI, row);
	mov_exx(RegType::EBX, col);
	call(0x00429120);
}

// static void asm_put_rake(int row, int col)
// {
// 	push_byte(row);
// 	push_byte(col);
// 	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
// 	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
// 	//push_exx(RegType::EAX);
// 	call(0x0040e270);
// 	add_esp(8);
// }
// 
// void PVZ_1_2_EN_1073::PutRake(int row, int col)
// {
// 	if (InGame())
// 	{
// 		Map_1_2_EN_1073 map(process);
// 		int Rows = GetRowCount();
// 		int Cols = 9;
// 		clear();
// 		if (row == -1 && col == -1)
// 		{
// 			for (int i = 0; i < Rows; ++i)
// 			{
// 				for (int j = 0; j < Cols; ++j)
// 				{
// 					asm_put_rake(i, j);
// 				}
// 			}
// 		}
// 		else if (row != -1 && col == -1)
// 		{
// 			for (int i = 0; i < Cols; ++i)
// 			{
// 				asm_put_rake(row, i);
// 			}
// 		}
// 		else if (row == -1 && col != -1)
// 		{
// 			for (int i = 0; i < Rows; ++i)
// 			{
// 				asm_put_rake(i, col);
// 			}
// 		}
// 		else
// 		{
// 			asm_put_rake(row, col);
// 		}
// 		WriteMemory(0x0020810f, 4, 0x0040e291);
// 		WriteMemory(0x39, 1, 0x0040e3ca);
// 		WriteMemory(0x900c7d8b, 4, 0x0040e3d9);
// 		WriteMemory(0x90084d8b, 4, 0x0040e3e2);
// 		RunAsm();
// 		WriteMemory(0x026c840f, 4, 0x0040e291);
// 		WriteMemory(-1, 1, 0x0040e3ca);
// 		WriteMemory(0x1c247c89, 4, 0x0040e3d9);
// 		WriteMemory(0x10244c8b, 4, 0x0040e3e2);
// 	}
// }

void PVZ_1_2_EN_1073::NoDelete(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x005cd993);
	}
	else
	{
		WriteMemory(0x74, 1, 0x005cd993);
	}
}

void PVZ_1_2_EN_1073::NoSave(bool b)
{
	if (b)
	{
		WriteMemory(0xb1, 1, 0x0048cd63);
	}
	else
	{
		WriteMemory(0x89, 1, 0x0048cd63);
	}
}

void PVZ_1_2_EN_1073::Anywhere(bool b)
{
	if (b)
	{
		WriteMemory(0x81, 1, 0x004127f0);
		WriteMemory(0xeb, 1, 0x0043c030);
		WriteMemory(0x81, 1, 0x0042ce79);
	}
	else
	{
		WriteMemory(0x84, 1, 0x004127f0);
		WriteMemory(0x74, 1, 0x0043c030);
		WriteMemory(0x84, 1, 0x0042ce79);
	}
}

void PVZ_1_2_EN_1073::FastBelt(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x004257ff);
		WriteMemory(0x33, 1, 0x004948ae);
	}
	else
	{
		WriteMemory(0x8f, 1, 0x004257ff);
		WriteMemory(0x85, 1, 0x004948ae);
	}
}

void PVZ_1_2_EN_1073::ShowHide(bool b)
{
	if (b)
	{
		WriteMemory(0x38, 1, 0x00430c80);
	}
	else
	{
		WriteMemory(0x88, 1, 0x00430c80);
	}
}

void PVZ_1_2_EN_1073::FullFog(bool b)
{
	if (b)
	{
		WriteMemory(0xc3c033, 3, 0x0041ef80);
		WriteMemory(0x3feb, 2, 0x0041cf66);
		WriteMemory(0, 1, 0x0041cfaa);
	}
	else
	{
		DWORD FogAddr = ReadMemory(4, DATA_ADDR, 1, OFFSET_1) + 0x4e0;
		for (int i = 0; i < 63; ++i)
		{
			WriteMemory(0, 4, FogAddr + i * 4);
		}
		WriteMemory(0xa4818b, 3, 0x0041ef80);
		WriteMemory(0x0575, 2, 0x0041cf66);
		WriteMemory(4, 1, 0x0041cfaa);
	}
}

void PVZ_1_2_EN_1073::SeeFog(bool b)
{
	if (b)
	{
		WriteMemory(0xd231, 2, 0x0041d17d);
	}
	else
	{
		WriteMemory(0xf23b, 2, 0x0041d17d);
	}
}

void PVZ_1_2_EN_1073::SeeVase(bool b)
{
	if (b)
	{
		static unsigned char code[] = { 0xb8, 0x32, 0, 0, 0 };
		WriteArrayMemory(code, 0x004518aa);
	}
	else
	{
		static unsigned char code[] = { 0x85, 0xc0, 0x7e, 4, 0x48 };
		WriteArrayMemory(code, 0x004518aa);
	}
}

void PVZ_1_2_EN_1073::JmpLevel(int level)
{
	WriteMemory(level, 4, DATA_ADDR, 3, OFFSET_1, OFFSET_2, 0x6c);
}

void PVZ_1_2_EN_1073::MixMode(int mode, int level)
{
	if (mode == 0)
	{
		WriteMemory(level, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x4c);
		WriteMemory(level, 4, DATA_ADDR, 2, OFFSET_1, 0x5568);
	}
	WriteMemory(mode, 4, DATA_ADDR, 1, MODE_OFFSET);
}

void PVZ_1_2_EN_1073::Unlock(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x0044cdf0);
		WriteMemory(0xeb, 1, 0x00456fa1);
		WriteMemory(0xc301b0, 3, 0x00404c50);
		WriteMemory(0, 4, 0x0071bdb0);
		WriteMemory(0xc301b0, 3, 0x00404d50);
		WriteMemory(0xeb, 1, 0x0044ce1c);
		WriteMemory(0xeb, 1, 0x00495163);
		WriteMemory(0xeb, 1, 0x004980a0);
		WriteMemory(0xc3c030, 3, 0x004956c0);
		WriteMemory(0xc301b0, 3, 0x00497050);
		WriteMemory(0x91, 1, 0x004575af);
		WriteMemory(0x70, 1, 0x0044d468);
		WriteMemory(0xc3c031, 3, 0x00431150);
	}
	else
	{
		WriteMemory(0x7f, 1, 0x0044cdf0);
		WriteMemory(0x7f, 1, 0x00456fa1);
		WriteMemory(0x8b5351, 3, 0x00404c50);
		WriteMemory(40, 4, 0x0071bdb0);
		WriteMemory(0x84808b, 3, 0x00404d50);
		WriteMemory(0x7f, 1, 0x0044ce1c);
		WriteMemory(0x7d, 1, 0x00495163);
		WriteMemory(0x7d, 1, 0x004980a0);
		WriteMemory(0xf98b57, 3, 0x004956c0);
		WriteMemory(0xb78b56, 3, 0x00497050);
		WriteMemory(0x9d, 1, 0x004575af);
		WriteMemory(0x7e, 1, 0x0044d468);
		WriteMemory(0x565351, 3, 0x00431150);
	}
}

void PVZ_1_2_EN_1073::Win()
{
	int mode = GetMode();
	if (!InGame() ||
		mode >= 1 && mode <= 15 && ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET+4) != 3)
	{
		return;
	}
	clear();
	mov_exx_dword_ptr(RegType::ECX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_1);
	call(0x0040ecd0);
	RunAsm();
}

void PVZ_1_2_EN_1073::Lock(bool b, int mode)
{
	if (b)
	{
		static const int val[] = { 24, -658, 73, 100, 122, 190, 249, 360 };
		WriteMemory(0x81, 1, 0x0042dd2c);
		WriteMemory(val[mode], 4, 0x0042dd2d);
	}
	else
	{
		WriteMemory(0x85, 1, 0x0042dd2c);
		WriteMemory(144, 4, 0x0042dd2d);
	}
}

void PVZ_1_2_EN_1073::Little(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x00533ed9);
	}
	else
	{
		WriteMemory(0x74, 1, 0x00533ed9);
	}

}

void PVZ_1_2_EN_1073::Rain(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x004295fe);
		WriteMemory(0x70, 1, 0x00419813);
		WriteMemory(0xeb, 1, 0x0042721b);
	}
	else
	{
		WriteMemory(0x75, 1, 0x004295fe);
		WriteMemory(0x75, 1, 0x00419813);
		WriteMemory(0x74, 1, 0x0042721b);
	}
}

void PVZ_1_2_EN_1073::Storm(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x004197fb);
		WriteMemory(0xeb, 1, 0x0042961c);
		WriteMemory(0xeb, 1, 0x004295e5);
		WriteMemory(0xeb, 1, 0x004271cf);
		WriteMemory(0xeb, 1, 0x0042722f);
	}
	else
	{
		WriteMemory(0x74, 1, 0x004197fb);
		WriteMemory(0x74, 1, 0x0042961c);
		WriteMemory(0x74, 1, 0x004295e5);
		WriteMemory(0x74, 1, 0x004271cf);
		WriteMemory(0x74, 1, 0x0042722f);
	}
}

void PVZ_1_2_EN_1073::Force(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00471bf6);
		WriteMemory(0x70, 1, 0x00472301);
		WriteMemory(0x70, 1, 0x0053628d);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00471bf6);
		WriteMemory(0x75, 1, 0x00472301);
		WriteMemory(0x75, 1, 0x0053628d);
	}
}

void PVZ_1_2_EN_1073::LikeCol(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x0043c24a);
		WriteMemory(0x80, 1, 0x004134a8);
	}
	else
	{
		WriteMemory(0x85, 1, 0x0043c24a);
		WriteMemory(0x85, 1, 0x004134a8);
	}
}

void PVZ_1_2_EN_1073::RunFast(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x004273b8);
	}
	else
	{
		WriteMemory(0x75, 1, 0x004273b8);
	}
}

void PVZ_1_2_EN_1073::SetCardNum(int num)
{
	if (InGame())
	{
		WriteMemory(0xc3, 1, 0x0041eca0);
		clear();
		mov_exx(RegType::EAX, num);
		mov_exx_dword_ptr(RegType::ESI, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::ESI, OFFSET_1);
		mov_exx_dword_ptr_exx_add_dword(RegType::ESI, 0x15c);
		call(0x004948d0);
		RunAsm();
		WriteMemory(0x56, 1, 0x0041eca0);
	}
}

void PVZ_1_2_EN_1073::SetCard(int cid, int type, bool copy)
{
	if (copy)
	{
		WriteMemory(48, 4, DATA_ADDR, 3, OFFSET_1, 0x15c, 0x5c + cid * 0x50);
		WriteMemory(type, 4, DATA_ADDR, 3, OFFSET_1, 0x15c, 0x60 + cid * 0x50);
	}
	else
	{
		WriteMemory(type, 4, DATA_ADDR, 3, OFFSET_1, 0x15c, 0x5c + cid * 0x50);
	}
}

void PVZ_1_2_EN_1073::FSBoom(bool b)
{
	if (b)
	{
		WriteMemory(0x20eb, 2, 0x004206d3);
		WriteMemory(0x70, 1, 0x00469f62);
	}
	else
	{
		WriteMemory(0x5d7f, 2, 0x004206d3);
		WriteMemory(0x75, 1, 0x00469f62);
	}
}

void PVZ_1_2_EN_1073::FSSquash(bool b)
{
	if (b)
	{
		WriteMemory(0x5feb, 2, 0x004642cf);
		WriteMemory(0x80, 1, 0x004641f8);
		WriteMemory(0x5feb, 2, 0x00464107);
	}
	else
	{
		WriteMemory(0x047f, 2, 0x004642cf);
		WriteMemory(0x85, 1, 0x004641f8);
		WriteMemory(0x7475, 2, 0x00464107);
	}
}

void PVZ_1_2_EN_1073::FSMagnet(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x00465a83);
		WriteMemory(0x70, 1, 0x00465b04);
	}
	else
	{
		WriteMemory(0x87, 1, 0x00465a83);
		WriteMemory(0x74, 1, 0x00465b04);
	}
}

void PVZ_1_2_EN_1073::FSSputter(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00471b03);
	}
	else
	{
		WriteMemory(0x74, 1, 0x00471b03);
	}
}

void PVZ_1_2_EN_1073::FSMouth(bool b)
{
	if (b)
	{
		WriteMemory(1, 1, eCodeStart + EXCODE_VAR_FS_MOUTH);
	}
	else
	{
		WriteMemory(0, 1, eCodeStart + EXCODE_VAR_FS_MOUTH);
	}
}

void PVZ_1_2_EN_1073::FSPrick(bool b)
{
	if (b)
	{
		WriteMemory(1, 1, eCodeStart + EXCODE_VAR_FS_PRICK);
	}
	else
	{
		WriteMemory(0, 1, eCodeStart + EXCODE_VAR_FS_PRICK);
	}
}

void PVZ_1_2_EN_1073::FSSpout(bool b)
{
	if (b)
	{
		WriteMemory(1, 1, eCodeStart + EXCODE_VAR_FS_SPOUT);
		WriteMemory(0x39eb, 2, 0x0046b14d);
		WriteMemory(0x16eb, 2, 0x0046268c);
	}
	else
	{
		WriteMemory(0, 1, eCodeStart + EXCODE_VAR_FS_SPOUT);
		WriteMemory(0x8c0f, 2, 0x0046b14d);
		WriteMemory(0x8c0f, 2, 0x0046268c);
	}
}

void PVZ_1_2_EN_1073::FSCat(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0046b2af);
		WriteMemory(0x75, 1, eCode.CatR + 0xc3 - 0x9b);
		WriteMemory(0x74, 1, eCode.CatC + 0xd1 - 0xc8);
		WriteMemory(0x1beb, 2, 0x004684a0);
		static unsigned char code1[] = { 0x33, 0xc0, 0xb0, 0x03, 0xeb, 0x0d };
		WriteArrayMemory(code1, sizeof(code1), 0x00462440);
		WriteMemory(0x3f, 1, 0x004684af);
		static unsigned char _3_code[] = { 0xE9, 0x8f, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x12, 0x75, 0xE9, 0x8B, 0x87, 0x90, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x09, 0x74, 0xBE, 0x83, 0xF8, 0x05, 0x74, 0xB9, 0x83, 0xF8, 0x01, 0x74, 0xB4, 0xE9, 0x74, 0xFE, 0xFF, 0xFF };
		WriteArrayMemory(_3_code, sizeof(_3_code), 0x004684ea);
		WriteMemory(0xeb, 1, 0x0046adb2);
		WriteMemory(0x70, 1, 0x0046ae27);
		WriteMemory(0x70, 1, eCode.Split);
		WriteMemory(0xe9, 1, 0x00468608);
		WriteMemory(0xfffffeb0, 4, 0x00468609);
		WriteMemory(0x70, 1, eCode.Throw);
		WriteMemory(0xffffffcd, 4, 0x0046ae73);
		WriteMemory(0x80, 1, 0x0046ad8f);
		WriteMemory(0x80, 1, 0x0046ad98);
		WriteMemory(0x80, 1, 0x0046ada1);
		WriteMemory(0x80, 1, 0x0046adaa);
		WriteMemory(0x80, 1, 0x0046ae11);
		WriteMemory(0x80, 1, 0x0046ae1a);
		WriteMemory(0xeb, 1, 0x0046ae2c);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0046b2af);
		WriteMemory(0x70, 1, eCode.CatR + 0xc3 - 0x9b);
		WriteMemory(0xeb, 1, eCode.CatC + 0xd1 - 0xc8);
		WriteMemory(0x4f8b, 2, 0x004684a0);
		static unsigned char code1[] = { 0x8B, 0x40, 0x24, 0x83, 0xF8, 0x1A };
		WriteArrayMemory(code1, sizeof(code1), 0x00462440);
		WriteMemory(0x2d, 1, 0x004684af);
		static unsigned char _3_code[] = { 0x83, 0xF8, 0x12, 0x0F, 0x85, 0x8B, 0x00, 0x00, 0x00, 0x8B, 0x5F, 0x1C, 0x8B, 0x37, 0x8D, 0x53, 0xFF, 0x89, 0x54, 0x24, 0x14, 0x8B, 0x97, 0x9C, 0x00, 0x00, 0x00, 0x8B, 0xC6, 0xE8, 0x54, 0xCF, 0xFE, 0xFF, 0x8B, 0x97 };
		WriteArrayMemory(_3_code, sizeof(_3_code), 0x004684ea);
		WriteMemory(0x75, 1, 0x0046adb2);
		WriteMemory(0x74, 1, 0x0046ae27);
		WriteMemory(0xeb, 1, eCode.Split);
		WriteMemory(0x8b, 1, 0x00468608);
		WriteMemory(0x006a1c4f, 4, 0x00468609);
		WriteMemory(0xeb, 1, eCode.Throw);
		WriteMemory(0x00000193, 4, 0x0046ae73);
		WriteMemory(0x84, 1, 0x0046ad8f);
		WriteMemory(0x84, 1, 0x0046ad98);
		WriteMemory(0x84, 1, 0x0046ada1);
		WriteMemory(0x84, 1, 0x0046adaa);
		WriteMemory(0x84, 1, 0x0046ae11);
		WriteMemory(0x84, 1, 0x0046ae1a);
		WriteMemory(0x75, 1, 0x0046ae2c);
	}
}

void PVZ_1_2_EN_1073::CatTouch(bool b)
{
	if (b)
	{
		WriteMemory(0x81, 1, 0x0047157c);
	}
	else
	{
		WriteMemory(0x85, 1, 0x0047157c);
	}
}

static void __declspec(naked) DoorCode()
{
	_asm{
		jmp fend;
		mov[eax + 0x14], ecx;
		mov dword ptr[eax + 0x1c], 0x00054b78;
		ret;
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) RowCode()
{
	_asm {
		jmp fend;
		je allow;
		mov edx, [ebp + 8];
		mov edx, [edx + 0x24];
		mov eax, ds:[EXCODE_SAVE_ADDR];
		cmp edx, 6;
		jne not_mouth;
		cmp byte ptr[eax + EXCODE_VAR_FS_MOUTH], 1;
		je allow;
		jmp not_allow;
	not_mouth:
		cmp edx, 10;
		jne not_spout;
		cmp byte ptr[eax + EXCODE_VAR_FS_SPOUT], 1;
		je allow;
		jmp not_allow;
	not_spout:
		cmp edx, 21;
		je is_prick;
		cmp edx, 46;
		je is_prick;
		jmp not_prick;
	is_prick:
		cmp byte ptr[eax + EXCODE_VAR_FS_PRICK], 1;
		je allow;
		jmp not_allow;
	not_prick:
	not_allow:
		pop edx;
		_jmp(0x0046b344);
	allow:
		ret;
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) ColCode()
{
	_asm {
		jmp fend;
		jge allow;
		mov edi, [ebp + 8];
		mov edi, [edi + 0x24];
		mov eax, ds:[EXCODE_SAVE_ADDR];
		cmp edi, 6;
		jne not_mouth;
		cmp byte ptr[eax + EXCODE_VAR_FS_MOUTH], 1;
		je allow;
		jmp not_allow;
	not_mouth:
		cmp edi, 10;
		je is_spout;
		cmp edi, 42;
		je is_spout;
		jmp not_spout;
	is_spout:
		cmp byte ptr[eax + EXCODE_VAR_FS_SPOUT], 1;
		je allow;
		jmp not_allow;
	not_spout:
		cmp edi, 21;
		je is_prick;
		cmp edi, 46;
		je is_prick;
		jmp not_prick;
	is_prick:
		cmp byte ptr[eax + EXCODE_VAR_FS_PRICK], 1;
		je allow;
		jmp not_allow;
	not_prick:
	not_allow:
		pop edi;
		_jmp(0x0046b341);
	allow:
		ret;
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) RHurtCode()
{
	_asm {
		jmp fend;
		je allow;
		mov edx, [ebp + 0x24];
		mov eax, ds:[EXCODE_SAVE_ADDR];
		cmp edx, 10;
		jne not_spout;
		cmp byte ptr[eax + EXCODE_VAR_FS_SPOUT], 1;
		je allow;
		jmp not_allow;
	not_spout:
		cmp edx, 21;
		je is_prick;
		cmp edx, 46;
		je is_prick;
		jmp not_prick;
	is_prick:
		cmp byte ptr[eax + EXCODE_VAR_FS_PRICK], 1;
		je allow;
		jmp not_allow;
	not_prick:
	not_allow:
		pop edx;
		_jmp(0x004627a6);
	allow:
		ret;
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) CHurtCode()
{
	_asm {
		jmp fend;
		jg allow;
		mov edx, [ebp + 0x24];
		mov eax, ds:[EXCODE_SAVE_ADDR];
		cmp edx, 6;
		jne not_mouth;
		cmp byte ptr[eax + EXCODE_VAR_FS_MOUTH], 1;
		je allow;
		jmp not_allow;
	not_mouth:
		cmp edx, 10;
		je is_spout;
		cmp edx, 42;
		je is_spout;
		jmp not_spout;
	is_spout:
		cmp byte ptr[eax + EXCODE_VAR_FS_SPOUT], 1;
		je allow;
		jmp not_allow;
	not_spout:
		cmp edx, 21;
		je is_prick;
		cmp edx, 46;
		je is_prick;
		jmp not_prick;
	is_prick:
		cmp byte ptr[eax + EXCODE_VAR_FS_PRICK], 1;
		je allow;
		jmp not_allow;
	not_prick:
	not_allow:
		pop edx;
		_jmp(0x004627a6);
	allow:
		ret;
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) CatRCode()
{
	_asm {
		jmp fend;
		cmp eax, 43;
		jne short _1;
	_2:
		pop eax;
		_jmp(0x0046B188);
	_1:
		cmp eax, 4;
		je short label;
		cmp eax, 6;
		je short label;
		cmp eax, 10;
		je short label;
		cmp eax, 42;
		je short label;
		cmp eax, 21;
		je short label;
		cmp eax, 46;
		jo short _2;
	label:
		ret;
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) CatCCode()
{
	_asm {
		jmp fend;
		mov ecx, [ebp + 8];
		mov ecx, [ecx + 0x24];
		cmp ecx, 4;
		jmp short label;
		cmp ecx, 6;
		je short label;
		cmp ecx, 10;
		je short label;
		cmp ecx, 42;
		je short label;
		cmp ecx, 21;
		je short label;
		cmp ecx, 46;
		je short label;
		pop ecx;
		push 0x0046B2A0;
	label:
		cmp byte ptr[esp + 0x17], 0;
		ret;
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) ThrowCode()
{
	_asm {
		jmp fend;
		jmp short _1;
		cmp dword ptr[edi + 0x24], 47;
		je _2;
		test eax, eax;
		jne _2;
		_jmp(0x00468382);
	_2:
		mov ecx, [edi + 0x1c];
		push esi;
		_jmp(0x004684D6);
	_1:
		mov ecx, [edi + 0x1c];
		push esi;
		push ecx;
		_jmp(0x00468679);
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) CatDirCode()
{
	_asm {
		jmp fend;
		cmp eax, 52;
		jne short _1;
	_3:
		fld dword ptr ds : [0x006FE1D0];
		ret;
	_1:
		cmp eax, 28;
		jne short _2;
		cmp edx, 1;
		je short _3;
	_2:
		fld dword ptr ds : [0x006FDBF8];
		ret;
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) SplitCode()
{
	_asm {
		jmp fend;
		jmp short _1;
		mov ecx, [edi + 0x1c];
		push ecx;
		push edi;
		xor ecx, ecx;
		_call(0x0046B080);
		test eax, eax;
		jne _2;
		_jmp(0x00468382);
	_2:
		push eax;
		mov ecx, [edi + 0x1c];
		push 0;
		push ecx;
		push eax;
		push edi;
		_call(0x0046A8C0);
		pop eax;
		mov edx, [edi + 0x1c];
		push 1;
		push edx;
		push eax;
		push edi;
		_call(0x0046A8C0);
		_jmp(0x00468382);
	_1:
		mov esi, 3;
		_jmp(0x004685A6);
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) ZTimerCode()
{
	_asm{
		jmp fend;
		mov ecx, 0xffffffff;
		cmp[edi + 0x55b4], ecx;
		ja _1;
		dec dword ptr ds : [edi + 0x55b4];
		ret;
	_1:
		mov[edi + 0x55b4], ecx;
		ret;
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) GetHeadRowsCode()
{
	_asm{
		jmp fend;
		mov eax, ds:[DATA_ADDR];
		mov eax, [eax + OFFSET_1];
		mov eax, [eax + 0x5564];
		cmp eax, 2;
		je _6;
		cmp eax, 3;
		jne _5;
	_6:
		mov eax, 6;
		jmp label;
	_5:
		mov eax, 5;
	label:
		mov dword ptr[edi + 0x28], 0x59;
		ret;
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) GetRVRowsCode()
{
	_asm{
		jmp fend;
		mov eax, ds:[DATA_ADDR];
		mov eax, [eax + OFFSET_1];
		mov eax, [eax + 0x5564];
		cmp eax, 2;
		je _6;
		cmp eax, 3;
		jne _5;
	_6:
		mov eax, 5;
		jmp label;
	_5:
		mov eax, 4;
	label:
		mov dword ptr[esi + 0x28], 0x55;
		ret;
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) DoorRowsCode()
{
	_asm{
		jmp fend;
		mov eax, ds:[DATA_ADDR];
		mov eax, [eax + OFFSET_1];
		mov eax, [eax + 0x5564];
		cmp eax, 2;
		je _6;
		cmp eax, 3;
		jne _5;
	_6:
		mov eax, 6;
		jmp label;
	_5:
		mov eax, 5;
	label:
		cmp edi, eax;
		jge label2;
		pop eax;
		_jmp(0x00429fb0);
	label2:
		ret;
		int 3;
		int 3;
	fend:
	}
}

void PVZ_1_2_EN_1073::InitExtraCode()
{
#define WRITE_CODE(f) \
	WriteArrayMemory(GET_ADDR(f) + HEAD_OF(f), SIZE_OF(f), temp), \
	WriteArrayMemory(CallCode, sizeof(CallCode), Caller)

	DWORD temp = ReadMemory(4, EXCODE_SAVE_ADDR);
	bool b = (temp != 0xcccccccc);
	if (!b)
	{
		temp = (DWORD)VirtualAllocEx(GetHandle(), NULL, 4096 * 2, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (temp)
			WriteMemory(temp, 4, EXCODE_SAVE_ADDR);
	}
	eCodeStart = temp;
	//开关变量
	unsigned char switch_vars[EXCODE_VAR_COUNT + 2] = {};
	(unsigned short&)switch_vars[EXCODE_VAR_COUNT] = 0xcccc;
	WriteArrayMemory(switch_vars, temp);
	temp += sizeof(switch_vars);

	DWORD Caller;
	/*传送门*/
	if (!b)
	{
		Caller = 0x00429b1c;
		static unsigned char CallCode[] = { 0xb9, 4, 0, 0, 0, 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[6] = temp - Caller - 10;
		WRITE_CODE(DoorCode);
	}
	temp += SIZE_OF(DoorCode);
	/*行判定*/
	if (!b)
	{
		Caller = 0x0046b182;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(RowCode);
	}
	temp += SIZE_OF(RowCode);
	/*列判定*/
	if (!b)
	{
		Caller = 0x0046b29a;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ColCode);
	}
	temp += SIZE_OF(ColCode);
	/*行伤害判定*/
	if (!b)
	{
		Caller = 0x0046269e;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(RHurtCode);
	}
	temp += SIZE_OF(RHurtCode);
	/*列伤害判定*/
	if (!b)
	{
		Caller = 0x00462712;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CHurtCode);
	}
	temp += SIZE_OF(CHurtCode);
	/*香蒲免行判定*/
	eCode.CatR = temp;
	if (!b)
	{
		Caller = 0x0046b140;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatRCode);
	}
	temp += SIZE_OF(CatRCode);
	/*香蒲免列判定*/
	eCode.CatC = temp;
	if (!b)
	{
		Caller = 0x0046b280;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatCCode);
	}
	temp += SIZE_OF(CatCCode);
	/*投手特殊处理*/
	eCode.Throw = temp;
	if (!b)
	{
		Caller = 0x00468674;
		static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ThrowCode);
	}
	temp += SIZE_OF(ThrowCode);
	/*猫式全屏子弹方向处理*/
	eCode.CatDir = temp;
	if (!b)
	{
		Caller = 0x0046ae44;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatDirCode);
	}
	temp += SIZE_OF(CatDirCode);
	/*裂荚处理*/
	eCode.Split = temp;
	if (!b)
	{
		Caller = 0x004685a1;
		static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(SplitCode);
	}
	temp += SIZE_OF(SplitCode);
	/*刷怪倒计时*/
	eCode.ZTimer = temp;
	if (!b)
	{
		Caller = 0x004167a5;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ZTimerCode);
	}
	temp += SIZE_OF(ZTimerCode);
	/*僵博吐球行*/
	if (!b)
	{
		Caller = 0x00545ec8;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x50, 0x90, 0x90, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(GetHeadRowsCode);
	}
	temp += SIZE_OF(GetHeadRowsCode);
	/*僵博摔车行*/
	if (!b)
	{
		Caller = 0x0054554b;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x50, 0x90, 0x90, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(GetRVRowsCode);
	}
	temp += SIZE_OF(GetRVRowsCode);
	/*传送门行*/
	if (!b)
	{
		Caller = 0x00429fe4;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(DoorRowsCode);
	}
	temp += SIZE_OF(DoorRowsCode);

	eCode.Thread = temp;
	return;
}

void PVZ_1_2_EN_1073::StartDoor(bool b)
{
	if (b)
	{
		if (InGame() && ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET+4) > 2)
		{
			bool HasDoor = false;
			GetItems();
			for (auto& ri : vi)
			{
				if (ri.Type == 4 || ri.Type == 5)
				{
					HasDoor = true;
					break;
				}
			}
			if (!HasDoor)
			{
				SetDoor(0, 2, 1, 9, 3, 9, 4, 2);
			}
		}
		WriteMemory(0x70, 1, 0x0046b125);
		WriteMemory(0x70, 1, 0x004273dd);
		WriteMemory(0x70, 1, 0x00422af9);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0046b125);
		WriteMemory(0x75, 1, 0x004273dd);
		WriteMemory(0x75, 1, 0x00422af9);
	}
}

void PVZ_1_2_EN_1073::StableDoor(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x0042a14a);
	}
	else
	{
		WriteMemory(-1, 1, 0x0042a14a);
	}
}

void PVZ_1_2_EN_1073::SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c)
{
	if (InGame())
	{
		clear();
		GetItems();
		for (auto& ri : vi)
		{
			if (ri.Type == 4 || ri.Type == 5)
			{
				mov_exx(RegType::ESI, ri.addr);
				call(0x004502b0);
			}
		}
		WriteMemory(b1c, 1, 0x00429a92);
		WriteMemory(b1r, 1, 0x00429a99);
		WriteMemory(b2c, 1, 0x00429abd);
		WriteMemory(b2r, 1, 0x00429ac4);
		WriteMemory(w1c, 1, 0x00429aed);
		WriteMemory(w1r, 1, 0x00429af4);
		WriteMemory(w2c, 1, 0x00429b18);
		WriteMemory(w2r, 1, 0x00429b1d);
		mov_exx_dword_ptr(RegType::EDI, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::EDI, OFFSET_1);
		mov_exx_dword_ptr_exx_add_dword(RegType::EDI, OFFSET_2);
		call(0x00429a70);
		RunAsm();
		WriteMemory(2, 1, 0x00429a92);
		WriteMemory(0, 1, 0x00429a99);
		WriteMemory(9, 1, 0x00429abd);
		WriteMemory(1, 1, 0x00429ac4);
		WriteMemory(9, 1, 0x00429aed);
		WriteMemory(3, 1, 0x00429af4);
		WriteMemory(2, 1, 0x00429b18);
		WriteMemory(4, 1, 0x00429b1d);
	}
}

void PVZ_1_2_EN_1073::CornNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x00464a0b);
	}
	else
	{
		WriteMemory(0x85, 1, 0x00464a0b);
	}
}

void PVZ_1_2_EN_1073::MagnetNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x00465817);
	}
	else
	{
		WriteMemory(0x85, 1, 0x00465817);
	}
}

void PVZ_1_2_EN_1073::PotatoNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x00463824);
	}
	else
	{
		WriteMemory(0x85, 1, 0x00463824);
	}
}

void PVZ_1_2_EN_1073::MouthNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00464f25);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00464f25);
	}
}

void PVZ_1_2_EN_1073::PStrong()
{
	WriteMemory(0x004046, 3, 0x00540681);
	WriteMemory(0x909090, 3, 0x0047169b);
	WriteMemory(0x909090, 3, 0x00471e59);
	WriteMemory(0xeb, 1, 0x0053f2bb);
	WriteMemory(0xeb, 1, 0x0041f9ef);
	WriteMemory(0xeb, 1, 0x00537d7a);
	WriteMemory(0x70, 1, 0x0046273a);
	WriteMemory(0, 1, 0x00462596);
}

void PVZ_1_2_EN_1073::PNormal()
{
	WriteMemory(0xfc4046, 3, 0x00540681);
	WriteMemory(0x405029, 3, 0x0047169b);
	WriteMemory(0x404e29, 3, 0x00471e59);
	WriteMemory(0x74, 1, 0x0053f2bb);
	WriteMemory(0x74, 1, 0x0041f9ef);
	WriteMemory(0x75, 1, 0x00537d7a);
	WriteMemory(0x75, 1, 0x0046273a);
	WriteMemory(-32, 1, 0x00462596);
}

void PVZ_1_2_EN_1073::PWeak()
{
	WriteMemory(0x004066, 3, 0x00540681);
	WriteMemory(0x404029, 3, 0x0047169b);
	WriteMemory(0x407629, 3, 0x00471e59);
	WriteMemory(0x74, 1, 0x0053f2bb);
	WriteMemory(0x74, 1, 0x0041f9ef);
	WriteMemory(0x75, 1, 0x00537d7a);
	WriteMemory(0x70, 1, 0x0046273a);
	WriteMemory(-32, 1, 0x00462596);
}

void PVZ_1_2_EN_1073::NoMark(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0042057d);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0042057d);
	}
}

void PVZ_1_2_EN_1073::NoSleep(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x004617c2);
		if (InGame())
		{
			clear();
			GetPlants();
			for (auto& rp : vp)
			{
				if (rp.Sleeping)
				{
					mov_exx(RegType::EDI, rp.addr);
					push_byte(0);
					call(0x00462190);
				}
			}
			RunAsm();
		}
	}
	else
	{
		WriteMemory(0x74, 1, 0x004617c2);
	}
}

void PVZ_1_2_EN_1073::StrongWind(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x0046a071);
	}
	else
	{
		WriteMemory(0x74, 1, 0x0046a071);
	}
}

void PVZ_1_2_EN_1073::StrongBean(bool b)
{
	if (b)
	{
		WriteMemory(0xc3, 1, 0x00472af9);
	}
	else
	{
		WriteMemory(0xe9, 1, 0x00472af9);
	}
}

void PVZ_1_2_EN_1073::Umbrella(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x004201b8);
	}
	else
	{
		WriteMemory(0x75, 1, 0x004201b8);
	}
}

void PVZ_1_2_EN_1073::NoCD(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00491e55);
		WriteMemory(0xeb, 1, 0x00492e70);
	}
	else
	{
		WriteMemory(0x7e, 1, 0x00491e55);
		WriteMemory(0x75, 1, 0x00492e70);
	}
}

void PVZ_1_2_EN_1073::LockScoop(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x00414d53);
		WriteMemory(6, 4, DATA_ADDR, 3, OFFSET_1, 0x150, 0x30);
	}
	else
	{
		WriteMemory(0x89, 1, 0x00414d53);
	}
}

void PVZ_1_2_EN_1073::AnyMove(bool b)
{
	if (b)
	{
		WriteMemory(0x81, 1, 0x00423427);
		WriteMemory(0x81, 1, 0x00423071);
	}
	else
	{
		WriteMemory(0x85, 1, 0x00423427);
		WriteMemory(0x85, 1, 0x00423071);
	}
}

void PVZ_1_2_EN_1073::NoPurple(bool b)
{
	if (b)
	{
		WriteMemory(0xc301b0, 3, 0x004205a0);
		WriteMemory(0xeb, 1, 0x00410dea);
	}
	else
	{
		WriteMemory(0x575351, 3, 0x004205a0);
		WriteMemory(0x74, 1, 0x00410dea);
	}
}

void PVZ_1_2_EN_1073::LockButter(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00468622);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00468622);
	}
}

void PVZ_1_2_EN_1073::LockCorn(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x00468622);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00468622);
	}
}

void PVZ_1_2_EN_1073::BoomIm()
{
	WriteMemory(0x80, 1, 0x0053718c);
	WriteMemory(0x80, 1, 0x00537c6d);
}

void PVZ_1_2_EN_1073::BoomNormal()
{
	WriteMemory(0x8f, 1, 0x0053718c);
	WriteMemory(0x85, 1, 0x00537c6d);
}

void PVZ_1_2_EN_1073::BoomNever()
{
	WriteMemory(0x81, 1, 0x0053718c);
	WriteMemory(0x81, 1, 0x00537c6d);
}

void PVZ_1_2_EN_1073::VisiableNo()
{
	WriteMemory(0x70, 1, 0x0053ecde);
	WriteMemory(0x70, 1, 0x00544a5b);
}

void PVZ_1_2_EN_1073::VisiableNormal()
{
	WriteMemory(0x75, 1, 0x0053ecde);
	WriteMemory(0x75, 1, 0x00544a5b);
}

void PVZ_1_2_EN_1073::VisiableYes()
{
	WriteMemory(0xeb, 1, 0x0053ecde);
	WriteMemory(0xeb, 1, 0x00544a5b);
}

void PVZ_1_2_EN_1073::ZStrong()
{
	WriteMemory(0xc0, 1, 0x005419f5);
	WriteMemory(0x90909090, 4, 0x00541cda);
	WriteMemory(0xd233c033, 4, 0x0054164b);
	WriteMemory(0x81, 1, 0x00543571);
	WriteMemory(0xeb, 1, 0x00464e08);
}

void PVZ_1_2_EN_1073::ZNormal()
{
	WriteMemory(0xc8, 1, 0x005419f5);
	WriteMemory(0x20246c2b, 4, 0x00541cda);
	WriteMemory(0xc28b027c, 4, 0x0054164b);
	WriteMemory(0x8d, 1, 0x00543571);
	WriteMemory(0x74, 1, 0x00464e08);
}

void PVZ_1_2_EN_1073::ZWeak()
{
	WriteMemory(0xc9, 1, 0x005419f5);
	WriteMemory(0x9090ed2b, 4, 0x00541cda);
	WriteMemory(0xc28bd08b, 4, 0x0054164b);
	WriteMemory(0x80, 1, 0x00543571);
	WriteMemory(0x74, 1, 0x00464e08);
}

void PVZ_1_2_EN_1073::NoSlow(bool b)
{
	if (b)
	{
		WriteMemory(0x90c033, 3, 0x0053bd2f);
	}
	else
	{
		WriteMemory(0xffc083, 3, 0x0053bd2f);
	}
}

void PVZ_1_2_EN_1073::NoButter(bool b)
{
	if (b)
	{
		WriteMemory(0x90c033, 3, 0x0053bd4b);
	}
	else
	{
		WriteMemory(0xffc083, 3, 0x0053bd4b);
	}
}

void PVZ_1_2_EN_1073::No3zGroup(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x0042940c);
		WriteMemory(0xeb, 1, 0x00415a02);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0042940c);
		WriteMemory(0x75, 1, 0x00415a02);
	}
}

void PVZ_1_2_EN_1073::NoIMP(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x00537895);
	}
	else
	{
		WriteMemory(0x7d, 1, 0x00537895);
	}
}

void PVZ_1_2_EN_1073::NoICE(bool b)
{
	if (b)
	{
		WriteMemory(0xc3, 1, 0x0053b0b0);
		WriteMemory(0xeb, 1, 0x004222ea);
		for (int i = 0; i < 6; ++i)
		{
			WriteMemory(1, 4, DATA_ADDR, 2, OFFSET_1, 0x63c + i * 4);
		}
	}
	else
	{
		WriteMemory(0x51, 1, 0x0053b0b0);
		WriteMemory(0x75, 1, 0x004222ea);
	}
}

void PVZ_1_2_EN_1073::ZEatable(bool b)
{
	if (b)
	{
		WriteMemory(0, 1, 0x00464e15);
	}
	else
	{
		WriteMemory(1, 1, 0x00464e15);
	}
}

void PVZ_1_2_EN_1073::StopZTimer(bool b)
{
	if (b)
	{
		WriteMemory(0xc3, 1, eCode.ZTimer);
	}
	else
	{
		WriteMemory(0xb9, 1, eCode.ZTimer);
	}
}

void PVZ_1_2_EN_1073::StopZombies(bool b)
{
	if (b)
	{
		WriteMemory(0x54, 1, 0x0053b433);
		WriteMemory(0x54, 1, 0x0053b43c);
	}
	else
	{
		WriteMemory(0x64, 1, 0x0053b433);
		WriteMemory(0x44, 1, 0x0053b43c);
	}
}

void PVZ_1_2_EN_1073::StopBoss(bool b)
{
	if (b)
	{
		WriteMemory(0xc3, 1, 0x00546af0);
	}
	else
	{
		WriteMemory(0x51, 1, 0x00546af0);
	}
}

void PVZ_1_2_EN_1073::BalloonBoom(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0053634b);
		WriteMemory(0x70, 1, 0x00536366);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0053634b);
		WriteMemory(0x74, 1, 0x00536366);
	}
}

void PVZ_1_2_EN_1073::ZTimerSpeed(int value)
{
	WriteMemory(value, 4, eCode.ZTimer + 1);
}

void PVZ_1_2_EN_1073::GatherZombie(bool b, float pos)
{
	if (b)
	{
		static unsigned char code[] = { 0xc7, 0x46, 0x2c, 0, 0, 0, 0, 0xeb, 0x23 };
		(DWORD&)code[3] = (DWORD&)pos;
		WriteMemory(0x32, 1, 0x0053b34a);
		WriteArrayMemory(code, 0x0053b420);
	}
	else
	{
		static unsigned char code[] = { 0xe8, 0xab, 0x13, 0, 0, 0x84, 0xc0, 0x75, 0x0f };
		WriteMemory(0x84, 1, 0x0053b34a);
		WriteArrayMemory(code, 0x0053b420);
	}
}

void PVZ_1_2_EN_1073::KillAllZombies()
{
	GetZombies();
	for (auto& rz : vz)
	{
		WriteMemory(3, 4, rz.addr + 0x28);
	}
}

void PVZ_1_2_EN_1073::ConfuseZombies()
{
	GetZombies();
	for (auto& rz : vz)
	{
		WriteMemory(1, 1, rz.addr + 0xb8);
	}
}

void PVZ_1_2_EN_1073::NaturalSeed(int* zombies, int znum)
{
	if (InGame())
	{
		int minzid = zombies[0];
		int minindex = 0;
		bool LowZ = (minzid == 0 || minzid == 26);
		for (int i = 1; i < znum; ++i)
		{
			if (zombies[i] < minzid)
			{
				minzid = zombies[i];
				minindex = i;
			}
			if (zombies[i] == 0 || zombies[i] == 26)
				LowZ = true;
		}
		if (!LowZ)
		{
			zombies[minindex] = 26;
		}
		DWORD SeedAddr = ReadMemory(4, DATA_ADDR, 1, OFFSET_1) + 0x54ec;
		DWORD ListAddr = ReadMemory(4, DATA_ADDR, 1, OFFSET_1) + 0x6cc;
		for (int i = 0; i < 34; ++i)
		{
			WriteMemory(0, 1, SeedAddr + i);
		}
		for (int i = 0; i < znum; ++i)
		{
			WriteMemory(1, 1, SeedAddr + zombies[i]);
		}
		clear();
		asm_init_zlist();
		RunAsm();
		if (!LowZ)
		{
			for (int i = 0; i < 2000; ++i)
			{
				if (ReadMemory(4, ListAddr + i * 4) == 26)
					WriteMemory(minzid, 4, ListAddr + i * 4);
			}
		}
		if (ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET+4) == 2)
		{
			clear();
			asm_destroy_model();
			asm_create_model();
			WriteMemory(0x80, 1, 0x0043d363);
			RunAsm();
			WriteMemory(0x85, 1, 0x0043d363);
		}
	}
}
void PVZ_1_2_EN_1073::asm_destroy_model()
{
	mov_exx_dword_ptr(RegType::EBX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EBX, OFFSET_1);
	call(0x004108e0);
}
void PVZ_1_2_EN_1073::asm_create_model()
{
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, 0x174);
	push_exx(RegType::EAX);
	call(0x0043d350);
}
void PVZ_1_2_EN_1073::LimitSeed(int* zombies, int znum, bool thief, bool red)
{
	if (InGame())
	{
		DWORD buffer[2000];
		memset(buffer, -1, sizeof(buffer));
		bool HasThief = false;
		bool HasRed = false;
		for (int i = 0; i < znum; ++i)
		{
			if (zombies[i] == 20)
				HasThief = true;
			if (zombies[i] == 32)
				HasRed = true;
		}
		int j = 0;
		for (int i = 0; i < 2000; ++i)
		{
			if (HasThief && thief && znum > 1)
			{
				if (i >= 450 && i <= 453
					|| i >= 950 && i <= 953
					|| i >= 1450 && i <= 1453
					|| i >= 1950 && i <= 1953)
				{
					buffer[i] = 20;
					continue;
				}
				if (znum > 1 && zombies[j] == 20)
					++j;
				if (j >= znum)
					j = 0;
			}
			if (HasRed && red && zombies[j] == 32 && znum > 1)
			{
				if (i >= 500 && i <= 950
					|| i >= 1500 && i <= 1950)
					++j;
				if (j >= znum)
					j = 0;
				if (zombies[j] == 20 && thief)
					++j;
				if (j >= znum)
					j = 0;
			}
			buffer[i] = zombies[j++];
			if (j >= znum)
				j = 0;
		}
		DWORD ListAddr = ReadMemory(4, DATA_ADDR, 1, OFFSET_1) + 0x6cc;
		WriteArrayMemory(buffer, ListAddr);
		if (ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET+4) == 2)
		{
			clear();
			asm_destroy_model();
			asm_create_model();
			WriteMemory(0x80, 1, 0x0043d363);
			RunAsm();
			WriteMemory(0x85, 1, 0x0043d363);
		}
	}
}
void PVZ_1_2_EN_1073::asm_init_seed()
{
	mov_exx_dword_ptr(RegType::EDI, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EDI, OFFSET_1);
	add_word(0xc781);
	add_dword(0x54ec);//add edi,0x54ec
	mov_exx(RegType::ECX, 34);
	add_word(0xc030);//xor al,al
	add_byte(0xfc);//cld
	add_word(0xaaf3);// rep stos byte ptr es : [edi]
	mov_exx_dword_ptr(RegType::ESI, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::ESI, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::ESI, OFFSET_2);
	call(0x00428370);
}

void PVZ_1_2_EN_1073::asm_init_zlist()
{
	mov_exx_dword_ptr(RegType::EDI, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EDI, OFFSET_1);
	call(0x0040bc20);
}
void PVZ_1_2_EN_1073::ResetSeed()
{
	if (InGame())
	{
		clear();
		asm_init_seed();
		asm_init_zlist();
		if (ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET+4) == 2)
		{
			asm_destroy_model();
			asm_create_model();
			WriteMemory(0x80, 1, 0x0043d363);
		}
		RunAsm();
		WriteMemory(0x85, 1, 0x0043d363);
	}
}

void PVZ_1_2_EN_1073::SeeLeftZombies(bool(&zombies)[33])
{
	if (InGame())
	{
		DWORD Base = ReadMemory(4, DATA_ADDR, 1, OFFSET_1);
		DWORD WaveCount = ReadMemory(4, Base + 0x557c);
		DWORD CurWave = ReadMemory(4, Base + 0x5594);
		if (WaveCount == 0)
			return;
		DWORD LastAddr = Base + 0x6cc + (WaveCount - 1) * 200;
		DWORD CurAddr = Base + 0x6cc + CurWave * 200;
		if (CurWave == 40)
		{
			CurAddr = LastAddr;
		}
		while (CurAddr <= LastAddr)
		{
			for (int i = 0; i < 50; ++i)
			{
				int one = ReadMemory(4, CurAddr + i * 4);
				if (one < 0)
					break;
				else if (one < 33)
					zombies[one] = true;
			}
			CurAddr += 200;
		}
	}
}

void PVZ_1_2_EN_1073::FlowerNPK(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x0052992b);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x220);
	}
	else
	{
		WriteMemory(-1, 1, 0x0052992b);
	}
}

void PVZ_1_2_EN_1073::Medicine(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x00529a23);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x224);
	}
	else
	{
		WriteMemory(-1, 1, 0x00529a23);
	}
}

void PVZ_1_2_EN_1073::Chocolate(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x00529545);
		WriteMemory(0x39, 1, 0x005295c7);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x250);
	}
	else
	{
		WriteMemory(-1, 1, 0x00529545);
		WriteMemory(-1, 1, 0x005295c7);
	}
}

void PVZ_1_2_EN_1073::TreeNPK(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x0043018d);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x258);
	}
	else
	{
		WriteMemory(-1, 1, 0x0043018d);
	}
}

void PVZ_1_2_EN_1073::TreeHeight(int h)
{
	if (GetMode() == 50)
	{
		WriteMemory(h - 1, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x11c);
		clear();
		mov_exx_dword_ptr(RegType::ESI, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::ESI, OFFSET_1);
		mov_exx_dword_ptr_exx_add_dword(RegType::ESI, OFFSET_2);
		call(0x0042ff00);
		RunAsm();
	}
	else
		WriteMemory(h, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x11c);
}

void PVZ_1_2_EN_1073::FlowerNum(int n)
{
	WriteMemory(n, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x378);
}

void PVZ_1_2_EN_1073::ChangeFlower(int fid, int type)
{
	WriteMemory(type, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x380 + fid * 0x58);
}

void PVZ_1_2_EN_1073::ChangeFSize(int fid, int size)
{
	WriteMemory(size, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x3a4 + fid * 0x58);
}

void PVZ_1_2_EN_1073::ChangeFDir(int fid, int dir)
{
	WriteMemory(dir, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x390 + fid * 0x58);
}

void PVZ_1_2_EN_1073::ChangeFPlace(int fid, int place)
{
	WriteMemory(place, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x384 + fid * 0x58);
}

void PVZ_1_2_EN_1073::Patch()
{
	WriteMemory(0x23, 1, 0x00545751);
	WriteMemory(0x23, 1, 0x00545efe);
}

std::string PVZ_1_2_EN_1073::getUserdataDir(char systemDir)
{
	return PVZ::getUserdataDir(systemDir);
}
#include "stdafx.h"
#include "PVZ_1_2_CN_REPAIR.h"

#define DATA_ADDR 0x007578f8
#define OFFSET_1 0x868
#define OFFSET_2 0x178
#define MODE_OFFSET 0x918
#define GARDON_OFFSET 0x94c

void PVZ_1_2_CN_REPAIR::RunAsm()
{
	if (getLength() > 0)
	{
		WriteMemory(-2, 1, 0x627ade);
		Sleep(10);
		ret();
		runThread(GetHandle(), eCode.Thread);
		WriteMemory(0xc8, 1, 0x627ade);
	}
}
void PVZ_1_2_CN_REPAIR::GetPlants()
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
void PVZ_1_2_CN_REPAIR::GetItems()
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
void PVZ_1_2_CN_REPAIR::GetCars()
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
void PVZ_1_2_CN_REPAIR::GetZombies()
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
int PVZ_1_2_CN_REPAIR::GetMapType()
{
	return ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x5564);
}
int PVZ_1_2_CN_REPAIR::GetBlockType(int row, int col)/*1.草地2.裸地3.水池*/
{
	return ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x180 + row * 4 + col * 24);
}
void PVZ_1_2_CN_REPAIR::SetBlockType(int row, int col, int type)/*1.草地2.裸地3.水池*/
{
	WriteMemory(type, 4, DATA_ADDR, 2, OFFSET_1, 0x180 + row * 4 + col * 24);
}
int PVZ_1_2_CN_REPAIR::GetRowType(int row)/*0.不出怪1.草地2.水路*/
{
	return ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x5f0 + row * 4);
}
void PVZ_1_2_CN_REPAIR::SetRowType(int row, int type)/*0.不出怪1.草地2.水路*/
{
	WriteMemory(type, 4, DATA_ADDR, 2, OFFSET_1, 0x5f0 + row * 4);
}
bool PVZ_1_2_CN_REPAIR::InGame()
{
	return ReadMemory(4, DATA_ADDR, 1, OFFSET_1) != 0;
}

int PVZ_1_2_CN_REPAIR::GetMode()
{
	return ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET);
}

void PVZ_1_2_CN_REPAIR::SetSun(int sun)
{
	WriteMemory(sun, 4, DATA_ADDR, 2, OFFSET_1, 0x5578);
}

void PVZ_1_2_CN_REPAIR::SetMoney(int money)
{
	WriteMemory(money, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x50);
}

void PVZ_1_2_CN_REPAIR::IgnoreSun(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00422eb2);
		WriteMemory(0x39, 1, 0x00422eb6);
		WriteMemory(0x91, 1, 0x00422f00);
		WriteMemory(0x80, 1, 0x0042f2b2);
		WriteMemory(0x80, 1, 0x0042f630);
		WriteMemory(0xeb, 1, 0x00434818);
	}
	else
	{
		WriteMemory(0x7f, 1, 0x00422eb2);
		WriteMemory(0x89, 1, 0x00422eb6);
		WriteMemory(0x9e, 1, 0x00422f00);
		WriteMemory(0x8f, 1, 0x0042f2b2);
		WriteMemory(0x8f, 1, 0x0042f630);
		WriteMemory(0x7d, 1, 0x00434818);
	}
}

void PVZ_1_2_CN_REPAIR::AutoPickup(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x0043b3a2);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0043b3a2);
	}
}

void PVZ_1_2_CN_REPAIR::StartCar()
{
	if (InGame())
	{
		clear();
		GetCars();
		for (auto& rc : vc)
		{
			mov_exx(RegType::EBX, rc.addr);
			call(0x0046ade0);
		}
		RunAsm();
	}
}

void PVZ_1_2_CN_REPAIR::ResetCar()
{
	if (InGame())
	{
		clear();
		GetCars();
		for (auto& rc : vc)
		{
			mov_exx(RegType::EAX, rc.addr);
			call(0x0046ad50);
		}
		WriteMemory(0x60eb, 2, 0x00411068);
		WriteMemory(1, 1, 0x004110e6);
		WriteMemory(0xa8dc, 2, 0x0046a119);
		mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
		push_exx(RegType::EAX);
		call(0x00411040);
		RunAsm();
		WriteMemory(0x0975, 2, 0x00411068);
		WriteMemory(0, 1, 0x004110e6);
		WriteMemory(0xad30, 2, 0x0046a119);
	}
}

void PVZ_1_2_CN_REPAIR::asm_plant(int row, int col, int type, bool copy)
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
		call(0x00412370);
		mov_exx_eyy(RegType::ESI, RegType::EAX);
		push_exx(RegType::EAX);
		call(0x00479720);
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
		call(0x00412370);
	}
}

void PVZ_1_2_CN_REPAIR::asm_plant_paper(int row, int col, int type)
{
	asm_plant(row, col, type, false);
	push_exx(RegType::EAX);
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_2);
	call(0x00433700);
}

void PVZ_1_2_CN_REPAIR::NoPause(bool b)
{
	if (b)
	{
		WriteMemory(0xc3, 1, 0x0045ff30);
	}
	else
	{
		WriteMemory(0x56, 1, 0x0045ff30);
	}
}

void PVZ_1_2_CN_REPAIR::asm_put_zombie(int row, int col, int type)
{
	push_byte(type);
	mov_exx_dword_ptr(RegType::ECX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_2);
	push_exx(RegType::ECX);
	mov_exx(RegType::EAX, row);
	mov_exx(RegType::ECX, col);
	call(0x00433330);
}

void PVZ_1_2_CN_REPAIR::asm_put_boss()
{
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	push_byte(0);
	push_byte(25);
	call(0x00413430);
}

void PVZ_1_2_CN_REPAIR::asm_put_ladder(int row, int col)
{
	mov_exx(RegType::EDI, row);
	push_byte(col);
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	call(0x0040e290);
}

void PVZ_1_2_CN_REPAIR::asm_put_grave(int row, int col)
{
	mov_exx_dword_ptr(RegType::ECX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_2);
	push_exx(RegType::ECX);
	mov_exx(RegType::EDI, row);
	mov_exx(RegType::EBX, col);
	call(0x0042dda0);
}

//static void asm_put_rake(int row, int col)
//{
//	push_byte(row);
//	push_byte(col);
//	mov_exx_dword_ptr(RegType::ECX, DATA_ADDR);
//	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_1);
//	push_exx(RegType::ECX);
//	call(0x00410df0);
//	add_esp(8);
//}
//
//void PVZ_1_2_CN_REPAIR::PutRake(int row, int col)
//{
//	if (InGame())
//	{
//		Map_1_2_CN_REPAIR map(process);
//		int Rows = map.GetRowCount();
//		int Cols = 9;
//		clear();
//		if (row == -1 && col == -1)
//		{
//			for (int i = 0; i < Rows; ++i)
//			{
//				for (int j = 0; j < Cols; ++j)
//				{
//					asm_put_rake(i, j);
//				}
//			}
//		}
//		else if (row != -1 && col == -1)
//		{
//			for (int i = 0; i < Cols; ++i)
//			{
//				asm_put_rake(row, i);
//			}
//		}
//		else if (row == -1 && col != -1)
//		{
//			for (int i = 0; i < Rows; ++i)
//			{
//				asm_put_rake(i, col);
//			}
//		}
//		else
//		{
//			asm_put_rake(row, col);
//		}
//		WriteMemory(0x0020810f, 4, 0x00410e12);
//		WriteMemory(1, 1, 0x00410f55);
//		WriteMemory(0xeb107d8b, 4, 0x00410f51);
//		WriteMemory(0x900c758b, 4, 0x00410f65);
//		RunAsm();
//		WriteMemory(0x026c840f, 4, 0x00410e12);
//		WriteMemory(0, 1, 0x00410f55);
//		WriteMemory(0x024888ff, 4, 0x00410f51);
//		WriteMemory(0x1024748b, 4, 0x00410f65);
//	}
//}

void PVZ_1_2_CN_REPAIR::NoDelete(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0061e7f3);
	}
	else
	{
		WriteMemory(0x74, 1, 0x0061e7f3);
	}
}

void PVZ_1_2_CN_REPAIR::NoSave(bool b)
{
	if (b)
	{
		WriteMemory(0x41, 1, 0x0049c733);
	}
	else
	{
		WriteMemory(0x19, 1, 0x0049c733);
	}
}

void PVZ_1_2_CN_REPAIR::Anywhere(bool b)
{
	if (b)
	{
		WriteMemory(0x810fc933, 4, 0x00413737);
		WriteMemory(0x81, 1, 0x004334d8);
	}
	else
	{
		WriteMemory(0x850fc985, 4, 0x00413737);
		WriteMemory(0x84, 1, 0x004334d8);
	}
}

void PVZ_1_2_CN_REPAIR::FastBelt(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x0042ac2f);
		WriteMemory(0x33, 1, 0x004a411e);
	}
	else
	{
		WriteMemory(0x8f, 1, 0x0042ac2f);
		WriteMemory(0x85, 1, 0x004a411e);
	}
}

void PVZ_1_2_CN_REPAIR::ShowHide(bool b)
{
	if (b)
	{
		WriteMemory(0x38, 1, 0x004374da);
	}
	else
	{
		WriteMemory(0x88, 1, 0x004374da);
	}
}

void PVZ_1_2_CN_REPAIR::FullFog(bool b)
{
	if (b)
	{
		WriteMemory(0xc3c033, 3, 0x00423810);
	}
	else
	{
		DWORD FogAddr = ReadMemory(4, DATA_ADDR, 1, OFFSET_1) + 0x4e0;
		for (int i = 0; i < 63; ++i)
		{
			WriteMemory(0, 4, FogAddr + i * 4);
		}
		WriteMemory(0xa4818b, 3, 0x00423810);
	}
}

void PVZ_1_2_CN_REPAIR::SeeFog(bool b)
{
	if (b)
	{
		WriteMemory(0xd231, 2, 0x0041fcbd);
	}
	else
	{
		WriteMemory(0xf23b, 2, 0x0041fcbd);
	}
}

void PVZ_1_2_CN_REPAIR::SeeVase(bool b)
{
	if (b)
	{
		static unsigned char code[] = { 0xb8, 0x32, 0, 0, 0 };
		WriteArrayMemory(code, 0x0045c66a);
	}
	else
	{
		static unsigned char code[] = { 0x85, 0xc0, 0x7e, 4, 0x48 };
		WriteArrayMemory(code, 0x0045c66a);
	}
}

void PVZ_1_2_CN_REPAIR::JmpLevel(int level)
{
	WriteMemory(level, 4, DATA_ADDR, 3, OFFSET_1, OFFSET_2, 0x6c);
}

void PVZ_1_2_CN_REPAIR::MixMode(int mode, int level)
{
	if (mode == 0)
	{
		WriteMemory(level, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x4c);
		WriteMemory(level, 4, DATA_ADDR, 2, OFFSET_1, 0x5568);
	}
	WriteMemory(mode, 4, DATA_ADDR, 1, MODE_OFFSET);
}

void PVZ_1_2_CN_REPAIR::Unlock(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x00457843);
		WriteMemory(0xeb, 1, 0x00464ca1);
		WriteMemory(0xc301b0, 3, 0x004058f0);
		WriteMemory(0, 4, 0x00749d08);
		WriteMemory(0xc301b0, 3, 0x00405a70);
		WriteMemory(0xeb, 1, 0x00457866);
		WriteMemory(0xeb, 1, 0x004a4f08);
		WriteMemory(0xeb, 1, 0x004a81ca);
		WriteMemory(0xc3c030, 4, 0x004a54f0);
		WriteMemory(0xc301b0, 3, 0x004a6f30);
		WriteMemory(0x70, 1, 0x00465303);
		WriteMemory(0x70, 1, 0x00457f67);
		WriteMemory(0x2febc031, 4, 0x00437a10);
	}
	else
	{
		WriteMemory(0x7f, 1, 0x00457843);
		WriteMemory(0x7f, 1, 0x00464ca1);
		WriteMemory(0x08ec83, 3, 0x004058f0);
		WriteMemory(40, 4, 0x00749d08);
		WriteMemory(0x575551, 3, 0x00405a70);
		WriteMemory(0x7f, 1, 0x00457866);
		WriteMemory(0x7d, 1, 0x004a4f08);
		WriteMemory(0x7d, 1, 0x004a81ca);
		WriteMemory(0xf98b57, 3, 0x004a54f0);
		WriteMemory(0xb78b56, 3, 0x004a6f30);
		WriteMemory(0x7e, 1, 0x00465303);
		WriteMemory(0x7e, 1, 0x00457f67);
		WriteMemory(0x6c8b5553, 4, 0x00437a10);
	}
}

void PVZ_1_2_CN_REPAIR::Win()
{
	int mode = GetMode();
	if (!InGame() ||
		mode >= 1 && mode <= 15 && ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET+4) != 3)
	{
		return;
	}
	clear();
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	push_exx(RegType::EAX);
	call(0x00411880);
	RunAsm();
}

void PVZ_1_2_CN_REPAIR::Lock(bool b, int mode)
{
	if (b)
	{
		static const int val[] = { 26, -660, 79, 106, 124, 194, 253, 364 };
		WriteMemory(0x81, 1, 0x0043439b);
		WriteMemory(val[mode], 4, 0x0043439c);
	}
	else
	{
		WriteMemory(0x85, 1, 0x0043439b);
		WriteMemory(146, 4, 0x0043439c);
	}
}

void PVZ_1_2_CN_REPAIR::Little(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x005480ca);
	}
	else
	{
		WriteMemory(0x74, 1, 0x005480ca);
	}

}

void PVZ_1_2_CN_REPAIR::Rain(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0041bfb3);
		WriteMemory(0x70, 1, 0x0042e290);
		WriteMemory(0xeb, 1, 0x0042c005);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0041bfb3);
		WriteMemory(0x75, 1, 0x0042e290);
		WriteMemory(0x74, 1, 0x0042c005);
	}
}

void PVZ_1_2_CN_REPAIR::Storm(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x0041bf9b);
		WriteMemory(0xeb, 1, 0x0042e2ae);
		WriteMemory(0xeb, 1, 0x0042e277);
		WriteMemory(0xeb, 1, 0x0042bfbd);
		WriteMemory(0xeb, 1, 0x0042c019);
	}
	else
	{
		WriteMemory(0x74, 1, 0x0041bf9b);
		WriteMemory(0x74, 1, 0x0042e2ae);
		WriteMemory(0x74, 1, 0x0042e277);
		WriteMemory(0x74, 1, 0x0042bfbd);
		WriteMemory(0x74, 1, 0x0042c019);
	}
}

void PVZ_1_2_CN_REPAIR::Force(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00480f67);
		WriteMemory(0x70, 1, 0x004815f0);
		WriteMemory(0x70, 1, 0x00549e9c);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00480f67);
		WriteMemory(0x75, 1, 0x004815f0);
		WriteMemory(0x75, 1, 0x00549e9c);
	}
}

void PVZ_1_2_CN_REPAIR::LikeCol(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x004439ad);
		WriteMemory(0x80, 1, 0x004163ad);
	}
	else
	{
		WriteMemory(0x85, 1, 0x004439ad);
		WriteMemory(0x85, 1, 0x004163ad);
	}
}

void PVZ_1_2_CN_REPAIR::RunFast(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0042c1a2);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0042c1a2);
	}
}

void PVZ_1_2_CN_REPAIR::SetCardNum(int num)
{
	if (InGame())
	{
		WriteMemory(0xc3, 1, 0x00423530);
		clear();
		mov_exx(RegType::EAX, num);
		mov_exx_dword_ptr(RegType::ESI, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::ESI, OFFSET_1);
		mov_exx_dword_ptr_exx_add_dword(RegType::ESI, 0x15c);
		call(0x004a4140);
		RunAsm();
		WriteMemory(0x56, 1, 0x00423530);
	}
}

void PVZ_1_2_CN_REPAIR::SetCard(int cid, int type, bool copy)
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

void PVZ_1_2_CN_REPAIR::FSBoom(bool b)
{
	if (b)
	{
		WriteMemory(0x26eb, 2, 0x0042511e);
		WriteMemory(0x70, 1, 0x00479042);
	}
	else
	{
		WriteMemory(0x5f7f, 2, 0x0042511e);
		WriteMemory(0x75, 1, 0x00479042);
	}
}

void PVZ_1_2_CN_REPAIR::FSSquash(bool b)
{
	if (b)
	{
		WriteMemory(0x42eb, 2, 0x00473358);
		WriteMemory(0x80, 1, 0x00473263);
		WriteMemory(0x5beb, 2, 0x00473166);
	}
	else
	{
		WriteMemory(0x6f7f, 2, 0x00473358);
		WriteMemory(0x85, 1, 0x00473263);
		WriteMemory(0x6c75, 2, 0x00473166);
	}
}

void PVZ_1_2_CN_REPAIR::FSMagnet(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x00474a40);
		WriteMemory(0x70, 1, 0x00474ac1);
	}
	else
	{
		WriteMemory(0x87, 1, 0x00474a40);
		WriteMemory(0x74, 1, 0x00474ac1);
	}
}

void PVZ_1_2_CN_REPAIR::FSSputter(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00480e73);
	}
	else
	{
		WriteMemory(0x74, 1, 0x00480e73);
	}
}

void PVZ_1_2_CN_REPAIR::FSMouth(bool b)
{
	if (b)
	{
		WriteMemory(0x74, 1, eCode.Row + 0x18 - 0x0d);
		WriteMemory(0x74, 1, eCode.Col + 0x3d - 0x32);
	}
	else
	{
		WriteMemory(0x70, 1, eCode.Row + 0x18 - 0x0d);
		WriteMemory(0x70, 1, eCode.Col + 0x3d - 0x32);
	}
}

void PVZ_1_2_CN_REPAIR::FSPrick(bool b)
{
	if (b)
	{
		WriteMemory(0x74, 1, eCode.Row + 0x22 - 0x0d);
		WriteMemory(0x74, 1, eCode.Row + 0x27 - 0x0d);
		WriteMemory(0x74, 1, eCode.Col + 0x4c - 0x32);
		WriteMemory(0x74, 1, eCode.Col + 0x51 - 0x32);
		WriteMemory(0x74, 1, eCode.RHurt + 0x69 - 0x5c);
		WriteMemory(0x74, 1, eCode.RHurt + 0x6e - 0x5c);
		WriteMemory(0x74, 1, eCode.CHurt + 0x8b - 0x79);
		WriteMemory(0x74, 1, eCode.CHurt + 0x90 - 0x79);
	}
	else
	{
		WriteMemory(0x70, 1, eCode.Row + 0x22 - 0x0d);
		WriteMemory(0x70, 1, eCode.Row + 0x27 - 0x0d);
		WriteMemory(0x70, 1, eCode.Col + 0x4c - 0x32);
		WriteMemory(0x70, 1, eCode.Col + 0x51 - 0x32);
		WriteMemory(0x70, 1, eCode.RHurt + 0x69 - 0x5c);
		WriteMemory(0x70, 1, eCode.RHurt + 0x6e - 0x5c);
		WriteMemory(0x70, 1, eCode.CHurt + 0x8b - 0x79);
		WriteMemory(0x70, 1, eCode.CHurt + 0x90 - 0x79);
	}
}

void PVZ_1_2_CN_REPAIR::FSSpout(bool b)
{
	if (b)
	{
		WriteMemory(0x74, 1, eCode.Row + 0x1d - 0x0d);
		WriteMemory(0x74, 1, eCode.Col + 0x42 - 0x32);
		WriteMemory(0x74, 1, eCode.Col + 0x47 - 0x32);
		WriteMemory(0x74, 1, eCode.RHurt + 0x64 - 0x5c);
		WriteMemory(0x74, 1, eCode.CHurt + 0x81 - 0x79);
		WriteMemory(0x74, 1, eCode.CHurt + 0x86 - 0x79);
		WriteMemory(0x39eb, 2, 0x0047a258);
		WriteMemory(0x16eb, 2, 0x00471640);
	}
	else
	{
		WriteMemory(0x70, 1, eCode.Row + 0x1d - 0x0d);
		WriteMemory(0x70, 1, eCode.Col + 0x42 - 0x32);
		WriteMemory(0x70, 1, eCode.Col + 0x47 - 0x32);
		WriteMemory(0x70, 1, eCode.RHurt + 0x64 - 0x5c);
		WriteMemory(0x70, 1, eCode.CHurt + 0x81 - 0x79);
		WriteMemory(0x70, 1, eCode.CHurt + 0x86 - 0x79);
		WriteMemory(0x8c0f, 2, 0x0047a258);
		WriteMemory(0x8c0f, 2, 0x00471640);
	}
}

void PVZ_1_2_CN_REPAIR::FSCat(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0047a3bb);
		WriteMemory(0x75, 1, eCode.CatR + 0xc3 - 0x9b);
		WriteMemory(0x74, 1, eCode.CatC + 0xd1 - 0xc8);
		WriteMemory(0x1beb, 2, 0x00477545);
		static unsigned char code1[] = { 0x33, 0xc0, 0xb0, 0x03, 0xeb, 0x0d };
		WriteArrayMemory(code1, sizeof(code1), 0x00471460);
		WriteMemory(0x3f, 1, 0x00477554);
		static unsigned char _3_code[] = { 0xE9, 0x91, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x12, 0x75, 0xE9, 0x8B, 0x83, 0x90, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x09, 0x74, 0xBE, 0x83, 0xF8, 0x05, 0x74, 0xB9, 0x83, 0xF8, 0x01, 0x74, 0xB4, 0xE9, 0x62, 0xFE, 0xFF, 0xFF };
		WriteArrayMemory(_3_code, sizeof(_3_code), 0x0047758f);
		WriteMemory(0xeb, 1, 0x00479ea0);
		WriteMemory(0x70, 1, 0x00479f11);
		WriteMemory(0x70, 1, eCode.Split);
		WriteMemory(0xe9, 1, 0x004776b3);
		WriteMemory(0xfffffeaa, 4, 0x004776b4);
		WriteMemory(0x70, 1, eCode.Throw);
		WriteMemory(0xcb, 1, 0x00479f5f);
		WriteMemory(0x80, 1, 0x00479e7d);
		WriteMemory(0x80, 1, 0x00479e86);
		WriteMemory(0x80, 1, 0x00479e8f);
		WriteMemory(0x80, 1, 0x00479e98);
		WriteMemory(0x80, 1, 0x00479efb);
		WriteMemory(0x80, 1, 0x00479f04);
		WriteMemory(0xeb, 1, 0x00479f16);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0047a3bb);
		WriteMemory(0x70, 1, eCode.CatR + 0xc3 - 0x9b);
		WriteMemory(0xeb, 1, eCode.CatC + 0xd1 - 0xc8);
		WriteMemory(0x4b8b, 2, 0x00477545);
		static unsigned char code1[] = { 0x8B, 0x40, 0x24, 0x83, 0xF8, 0x1A };
		WriteArrayMemory(code1, sizeof(code1), 0x00471460);
		WriteMemory(0x2d, 1, 0x00477554);
		static unsigned char _3_code[] = { 0x83, 0xF8, 0x12, 0x0F, 0x85, 0x8B, 0x00, 0x00, 0x00, 0x8B, 0x5F, 0x1C, 0x8B, 0x37, 0x8D, 0x53, 0xFF, 0x89, 0x54, 0x24, 0x14, 0x8B, 0x97, 0x9C, 0x00, 0x00, 0x00, 0x8B, 0xC6, 0xE8, 0x54, 0xCF, 0xFE, 0xFF, 0x8B, 0x97 };
		WriteArrayMemory(_3_code, sizeof(_3_code), 0x0047758f);
		WriteMemory(0x75, 1, 0x00479ea0);
		WriteMemory(0x74, 1, 0x00479f11);
		WriteMemory(0xeb, 1, eCode.Split);
		WriteMemory(0x8b, 1, 0x004776b3);
		WriteMemory(0x006a1c4b, 4, 0x004776b4);
		WriteMemory(0xeb, 1, eCode.Throw);
		WriteMemory(0x62, 1, 0x00479f5f);
		WriteMemory(0x84, 1, 0x00479e7d);
		WriteMemory(0x84, 1, 0x00479e86);
		WriteMemory(0x84, 1, 0x00479e8f);
		WriteMemory(0x84, 1, 0x00479e98);
		WriteMemory(0x84, 1, 0x00479efb);
		WriteMemory(0x84, 1, 0x00479f04);
		WriteMemory(0x75, 1, 0x00479f16);
	}
}

void PVZ_1_2_CN_REPAIR::CatTouch(bool b)
{
	if (b)
	{
		WriteMemory(0x81, 1, 0x00480887);
	}
	else
	{
		WriteMemory(0x85, 1, 0x00480887);
	}
}

static void __declspec(naked) DoorCode()
{
	_asm {
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
		je short label;
		mov edx, [ebp + 8];
		mov edx, [edx + 0x24];
		cmp edx, 6;
		jo short label;
		cmp edx, 10;
		jo short label;
		cmp edx, 21;
		jo short label;
		cmp edx, 46;
		jo short label;
		pop edx;
		push 0x0047A450;
	label:
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
		jge short label;
		mov edi, [ebp + 8];
		mov edi, [edi + 0x24];
		cmp edi, 6;
		jo short label;
		cmp edi, 10;
		jo short label;
		cmp edi, 42;
		jo short label;
		cmp edi, 21;
		jo short label;
		cmp edi, 46;
		jo short label;
		pop edi;
		push 0x0047A44D;
	label:
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
		je short label;
		mov edx, [ebp + 0x24];
		cmp edx, 10;
		jo short label;
		cmp edx, 21;
		jo short label;
		cmp edx, 46;
		jo short label;
		pop edx;
		push 0x0047171A;
	label:
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
		jg short label;
		mov edx, [ebp + 0x24];
		cmp edx, 10;
		jo short label;
		cmp edx, 42;
		jo short label;
		cmp edx, 21;
		jo short label;
		cmp edx, 46;
		jo short label;
		pop edx;
		push 0x0047171A;
	label:
		mov eax, [edi + 0x24];
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
		_jmp(0x0047A293);
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
		push 0x0047A3AC;
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
		cmp dword ptr[ebx + 0x24], 47;
		je _2;
		test eax, eax;
		jne _2;
		_jmp(0x00477415);
	_2:
		mov ecx, [ebx + 0x1c];
		push esi;
		_jmp(0x0047757B);
	_1:
		mov ecx, [ebx + 0x1c];
		push esi;
		push ecx;
		_jmp(0x00477720);
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
		fld dword ptr ds : [0x0072A8C4];
		ret;
	_1:
		cmp eax, 28;
		jne short _2;
		cmp ecx, 1;
		je short _3;
	_2:
		fld dword ptr ds : [0x0072A4D4];
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
		mov ecx, [ebx + 0x1c];
		push ecx;
		push ebx;
		xor ecx, ecx;
		_call(0x0047A190);
		test eax, eax;
		jne _2;
		_jmp(0x00477415);
	_2:
		push eax;
		mov ecx, [ebx + 0x1c];
		push 0;
		push ecx;
		push eax;
		push ebx;
		_call(0x00479A50);
		pop eax;
		mov edx, [ebx + 0x1c];
		push 1;
		push edx;
		push eax;
		push ebx;
		_call(0x00479A50);
		_jmp(0x00477415);
	_1:
		mov esi, 3;
		_jmp(0x0047764D);
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
		cmp[ebp + 0x55b4], ecx;
		ja _1;
		dec dword ptr ds : [ebp + 0x55b4];
		ret;
	_1:
		mov[ebp + 0x55b4], ecx;
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
		mov dword ptr[ebp + 0x28], 0x59;
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
		_jmp(0x0042ed00);
	label2:
		ret;
		int 3;
		int 3;
	fend:
	}
}

void PVZ_1_2_CN_REPAIR::InitExtraCode()
{
#define WRITE_CODE(f) \
	WriteArrayMemory(GET_ADDR(f) + HEAD_OF(f), SIZE_OF(f), temp), \
	WriteArrayMemory(CallCode, sizeof(CallCode), Caller)

	DWORD temp = ReadMemory(4, 0x0042e6f7);
	bool b = (temp != 0xcccccccc);
	if (!b)
	{
		temp = (DWORD)VirtualAllocEx(GetHandle(), NULL, 4096 * 2, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (temp)
			WriteMemory(temp, 4, 0x0042e6f7);
	}
	DWORD Caller;
	/*传送门*/
	if (!b)
	{
		Caller = 0x0042e7b0;
		static unsigned char CallCode[] = { 0xb9, 4, 0, 0, 0, 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[6] = temp - Caller - 10;
		WRITE_CODE(DoorCode);
	}
	temp += SIZE_OF(DoorCode);
	/*行判定*/
	eCode.Row = temp;
	if (!b)
	{
		Caller = 0x0047a28d;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(RowCode);
	}
	temp += SIZE_OF(RowCode);
	/*列判定*/
	eCode.Col = temp;
	if (!b)
	{
		Caller = 0x0047a3a6;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ColCode);
	}
	temp += SIZE_OF(ColCode);
	/*行伤害判定*/
	eCode.RHurt = temp;
	if (!b)
	{
		Caller = 0x00471652;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(RHurtCode);
	}
	temp += SIZE_OF(RHurtCode);
	/*列伤害判定*/
	eCode.CHurt = temp;
	if (!b)
	{
		Caller = 0x004716be;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CHurtCode);
	}
	temp += SIZE_OF(CHurtCode);
	/*香蒲免行判定*/
	eCode.CatR = temp;
	if (!b)
	{
		Caller = 0x0047a24b;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatRCode);
	}
	temp += SIZE_OF(CatRCode);
	/*香蒲免列判定*/
	eCode.CatC = temp;
	if (!b)
	{
		Caller = 0x0047a38c;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatCCode);
	}
	temp += SIZE_OF(CatCCode);
	/*投手特殊处理*/
	eCode.Throw = temp;
	if (!b)
	{
		Caller = 0x0047771b;
		static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ThrowCode);
	}
	temp += SIZE_OF(ThrowCode);
	/*猫式全屏子弹方向处理*/
	eCode.CatDir = temp;
	if (!b)
	{
		Caller = 0x00479f2e;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatDirCode);
	}
	temp += SIZE_OF(CatDirCode);
	/*裂荚处理*/
	eCode.Split = temp;
	if (!b)
	{
		Caller = 0x00477648;
		static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(SplitCode);
	}
	temp += SIZE_OF(SplitCode);
	/*刷怪倒计时*/
	eCode.ZTimer = temp;
	if (!b)
	{
		Caller = 0x004198ca;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ZTimerCode);
	}
	temp += SIZE_OF(ZTimerCode);
	/*僵博吐球行*/
	if (!b)
	{
		Caller = 0x0055967a;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x50, 0x90, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(GetHeadRowsCode);
	}
	temp += SIZE_OF(GetHeadRowsCode);
	/*僵博摔车行*/
	if (!b)
	{
		Caller = 0x00558d3c;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x50, 0x90, 0x90, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(GetRVRowsCode);
	}
	temp += SIZE_OF(GetRVRowsCode);
	/*传送门行*/
	if (!b)
	{
		Caller = 0x0042ed39;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(DoorRowsCode);
	}
	temp += SIZE_OF(DoorRowsCode);


	eCode.Thread = temp;
	return;
}

void PVZ_1_2_CN_REPAIR::StartDoor(bool b)
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
		WriteMemory(0x70, 1, 0x0047a230);
		WriteMemory(0x70, 1, 0x0042c1c8);
		WriteMemory(0x70, 1, 0x00428062);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0047a230);
		WriteMemory(0x75, 1, 0x0042c1c8);
		WriteMemory(0x75, 1, 0x00428062);
	}
}

void PVZ_1_2_CN_REPAIR::StableDoor(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x004198ca);
	}
	else
	{
		WriteMemory(-1, 1, 0x004198ca);
	}
}

void PVZ_1_2_CN_REPAIR::SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c)
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
				call(0x0045b160);
			}
		}
		WriteMemory(b1c, 1, 0x0042e723);
		WriteMemory(b1r, 1, 0x0042e72a);
		WriteMemory(b2c, 1, 0x0042e74f);
		WriteMemory(b2r, 1, 0x0042e756);
		WriteMemory(w1c, 1, 0x0042e780);
		WriteMemory(w1r, 1, 0x0042e787);
		WriteMemory(w2c, 1, 0x0042e7ac);
		WriteMemory(w2r, 1, 0x0042e7b1);
		mov_exx_dword_ptr(RegType::EDI, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::EDI, OFFSET_1);
		mov_exx_dword_ptr_exx_add_dword(RegType::EDI, OFFSET_2);
		call(0x0042e700);
		RunAsm();
		WriteMemory(2, 1, 0x0042e723);
		WriteMemory(0, 1, 0x0042e72a);
		WriteMemory(9, 1, 0x0042e74f);
		WriteMemory(1, 1, 0x0042e756);
		WriteMemory(9, 1, 0x0042e780);
		WriteMemory(3, 1, 0x0042e787);
		WriteMemory(2, 1, 0x0042e7ac);
		WriteMemory(4, 1, 0x0042e7b1);
	}
}

void PVZ_1_2_CN_REPAIR::CornNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x00473a2c);
	}
	else
	{
		WriteMemory(0x85, 1, 0x00473a2c);
	}
}

void PVZ_1_2_CN_REPAIR::MagnetNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x00474777);
	}
	else
	{
		WriteMemory(0x85, 1, 0x00474777);
	}
}

void PVZ_1_2_CN_REPAIR::PotatoNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x00472783);
	}
	else
	{
		WriteMemory(0x85, 1, 0x00472783);
	}
}

void PVZ_1_2_CN_REPAIR::MouthNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00473ea5);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00473ea5);
	}
}

void PVZ_1_2_CN_REPAIR::PStrong()
{
	WriteMemory(0x004046, 3, 0x00553c3b);
	WriteMemory(0x909090, 3, 0x004809a5);
	WriteMemory(0x909090, 3, 0x00481200);
	WriteMemory(0xeb, 1, 0x005527cb);
	WriteMemory(0xeb, 1, 0x004243df);
	WriteMemory(0xeb, 1, 0x0054b7bd);
	WriteMemory(0x70, 1, 0x004716e3);
	WriteMemory(0, 1, 0x0047157b);
}

void PVZ_1_2_CN_REPAIR::PNormal()
{
	WriteMemory(0xfc4046, 3, 0x00553c3b);
	WriteMemory(0x404629, 3, 0x004809a5);
	WriteMemory(0x405629, 3, 0x00481200);
	WriteMemory(0x74, 1, 0x005527cb);
	WriteMemory(0x74, 1, 0x004243df);
	WriteMemory(0x75, 1, 0x0054b7bd);
	WriteMemory(0x75, 1, 0x004716e3);
	WriteMemory(-32, 1, 0x0047157b);
}

void PVZ_1_2_CN_REPAIR::PWeak()
{
	WriteMemory(0x004066, 3, 0x00553c3b);
	WriteMemory(0x407629, 3, 0x004809a5);
	WriteMemory(0x407629, 3, 0x00481200);
	WriteMemory(0x74, 1, 0x005527cb);
	WriteMemory(0x74, 1, 0x004243df);
	WriteMemory(0x75, 1, 0x0054b7bd);
	WriteMemory(0x70, 1, 0x004716e3);
	WriteMemory(-32, 1, 0x0047157b);
}

void PVZ_1_2_CN_REPAIR::NoMark(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00424fcd);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00424fcd);
	}
}

void PVZ_1_2_CN_REPAIR::NoSleep(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x004704f4);
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
					call(0x004711b0);
				}
			}
			RunAsm();
		}
	}
	else
	{
		WriteMemory(0x74, 1, 0x004704f4);
	}
}

void PVZ_1_2_CN_REPAIR::StrongWind(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x00479160);
	}
	else
	{
		WriteMemory(0x74, 1, 0x00479160);
	}
}

void PVZ_1_2_CN_REPAIR::StrongBean(bool b)
{
	if (b)
	{
		WriteMemory(0x3e, 1, 0x00481d52);
	}
	else
	{
		WriteMemory(0x25, 1, 0x00481d52);
	}
}

void PVZ_1_2_CN_REPAIR::Umbrella(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00424828);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00424828);
	}
}

void PVZ_1_2_CN_REPAIR::NoCD(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x004a1764);
		WriteMemory(0xeb, 1, 0x004a26f0);
	}
	else
	{
		WriteMemory(0x7e, 1, 0x004a1764);
		WriteMemory(0x75, 1, 0x004a26f0);
	}
}

void PVZ_1_2_CN_REPAIR::LockScoop(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x00417c9b);
		WriteMemory(6, 4, DATA_ADDR, 3, OFFSET_1, 0x150, 0x30);
	}
	else
	{
		WriteMemory(0x89, 1, 0x00417c9b);
	}
}

void PVZ_1_2_CN_REPAIR::AnyMove(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x00428985);
		WriteMemory(0x81, 1, 0x00428608);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00428985);
		WriteMemory(0x85, 1, 0x00428608);
	}
}

void PVZ_1_2_CN_REPAIR::NoPurple(bool b)
{
	if (b)
	{
		WriteMemory(0xc301b0, 3, 0x00424ff0);
		WriteMemory(0xeb, 1, 0x00413837);
	}
	else
	{
		WriteMemory(0x575351, 3, 0x00424ff0);
		WriteMemory(0x74, 1, 0x00413837);
	}
}

void PVZ_1_2_CN_REPAIR::LockButter(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x004776cf);
	}
	else
	{
		WriteMemory(0x75, 1, 0x004776cf);
	}
}

void PVZ_1_2_CN_REPAIR::LockCorn(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x004776cf);
	}
	else
	{
		WriteMemory(0x75, 1, 0x004776cf);
	}
}

void PVZ_1_2_CN_REPAIR::BoomIm()
{
	WriteMemory(0x80, 1, 0x0054ad4c);
	WriteMemory(0x80, 1, 0x0054b6fa);
}

void PVZ_1_2_CN_REPAIR::BoomNormal()
{
	WriteMemory(0x8f, 1, 0x0054ad4c);
	WriteMemory(0x85, 1, 0x0054b6fa);
}

void PVZ_1_2_CN_REPAIR::BoomNever()
{
	WriteMemory(0x81, 1, 0x0054ad4c);
	WriteMemory(0x81, 1, 0x0054b6fa);
}

void PVZ_1_2_CN_REPAIR::VisiableNo()
{
	WriteMemory(0x70, 1, 0x005521cd);
	WriteMemory(0x70, 1, 0x00558230);
}

void PVZ_1_2_CN_REPAIR::VisiableNormal()
{
	WriteMemory(0x75, 1, 0x005521cd);
	WriteMemory(0x75, 1, 0x00558230);
}

void PVZ_1_2_CN_REPAIR::VisiableYes()
{
	WriteMemory(0xeb, 1, 0x005521cd);
	WriteMemory(0xeb, 1, 0x00558230);
}

void PVZ_1_2_CN_REPAIR::ZStrong()
{
	WriteMemory(0xc0, 1, 0x00555001);
	WriteMemory(0x90909090, 4, 0x00555476);
	WriteMemory(0xf633c033, 4, 0x00554b68);
	WriteMemory(0xc3, 1, 0x00556d60);
	WriteMemory(0xeb, 1, 0x00473dba);
}

void PVZ_1_2_CN_REPAIR::ZNormal()
{
	WriteMemory(0xc8, 1, 0x00555001);
	WriteMemory(0x2024742b, 4, 0x00555476);
	WriteMemory(0xf38b027c, 4, 0x00554b68);
	WriteMemory(0x55, 1, 0x00556d60);
	WriteMemory(0x74, 1, 0x00473dba);
}

void PVZ_1_2_CN_REPAIR::ZWeak()
{
	WriteMemory(0xc9, 1, 0x00555001);
	WriteMemory(0x9090f631, 4, 0x00555476);
	WriteMemory(0xf38bde8b, 4, 0x00554b68);
	WriteMemory(0x55, 1, 0x00556d60);
	WriteMemory(0x74, 1, 0x00473dba);
}

void PVZ_1_2_CN_REPAIR::NoSlow(bool b)
{
	if (b)
	{
		WriteMemory(0x90c033, 3, 0x0054f5c0);
	}
	else
	{
		WriteMemory(0xffc083, 3, 0x0054f5c0);
	}
}

void PVZ_1_2_CN_REPAIR::NoButter(bool b)
{
	if (b)
	{
		WriteMemory(0x90c033, 3, 0x0054f5dc);
	}
	else
	{
		WriteMemory(0xffc083, 3, 0x0054f5dc);
	}
}

void PVZ_1_2_CN_REPAIR::No3zGroup(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x0042e08c);
		WriteMemory(0xeb, 1, 0x00418b8c);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0042e08c);
		WriteMemory(0x75, 1, 0x00418b8c);
	}
}

void PVZ_1_2_CN_REPAIR::NoIMP(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x0054b364);
	}
	else
	{
		WriteMemory(0x7d, 1, 0x0054b364);
	}
}

void PVZ_1_2_CN_REPAIR::NoICE(bool b)
{
	if (b)
	{
		WriteMemory(0xc3, 1, 0x0054e8d0);
		WriteMemory(0xeb, 1, 0x004278c6);
		for (int i = 0; i < 6; ++i)
		{
			WriteMemory(1, 4, DATA_ADDR, 2, OFFSET_1, 0x63c + i * 4);
		}
	}
	else
	{
		WriteMemory(0x51, 1, 0x0054e8d0);
		WriteMemory(0x75, 1, 0x004278c6);
	}
}

void PVZ_1_2_CN_REPAIR::ZEatable(bool b)
{
	if (b)
	{
		WriteMemory(0, 1, 0x00473dc7);
	}
	else
	{
		WriteMemory(1, 1, 0x00473dc7);
	}
}

void PVZ_1_2_CN_REPAIR::StopZTimer(bool b)
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

void PVZ_1_2_CN_REPAIR::StopZombies(bool b)
{
	if (b)
	{
		WriteMemory(0x54, 1, 0x0054ec57);
		WriteMemory(0x54, 1, 0x0054ec60);
	}
	else
	{
		WriteMemory(0x64, 1, 0x0054ec57);
		WriteMemory(0x44, 1, 0x0054ec60);
	}
}

void PVZ_1_2_CN_REPAIR::StopBoss(bool b)
{
	if (b)
	{
		WriteMemory(0xc3, 1, 0x0055a320);
	}
	else
	{
		WriteMemory(0x51, 1, 0x0055a320);
	}
}

void PVZ_1_2_CN_REPAIR::BalloonBoom(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00545f91);
		WriteMemory(0x70, 1, 0x00549f6c);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00545f91);
		WriteMemory(0x74, 1, 0x00549f6c);
	}
}

void PVZ_1_2_CN_REPAIR::ZTimerSpeed(int value)
{
	WriteMemory(value, 4, eCode.ZTimer + 1);
}

void PVZ_1_2_CN_REPAIR::GatherZombie(bool b, float pos)
{
	if (b)
	{
		static unsigned char code[] = { 0xc7, 0x46, 0x2c, 0, 0, 0, 0, 0xeb, 0x23 };
		(DWORD&)code[3] = (DWORD&)pos;
		WriteMemory(0x32, 1, 0x0054eb6b);
		WriteArrayMemory(code, 0x0054ec44);
	}
	else
	{
		static unsigned char code[] = { 0xe8, 0xb7, 0x13, 0, 0, 0x84, 0xc0, 0x75, 0x0f };
		WriteMemory(0x84, 1, 0x0054eb6b);
		WriteArrayMemory(code, 0x0054ec44);
	}
}

void PVZ_1_2_CN_REPAIR::KillAllZombies()
{
	GetZombies();
	for (auto& rz : vz)
	{
		WriteMemory(3, 4, rz.addr + 0x28);
	}
}

void PVZ_1_2_CN_REPAIR::ConfuseZombies()
{
	GetZombies();
	for (auto& rz : vz)
	{
		WriteMemory(1, 1, rz.addr + 0xb8);
	}
}

void PVZ_1_2_CN_REPAIR::NaturalSeed(int* zombies, int znum)
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
			WriteMemory(0x80, 1, 0x00445488);
			RunAsm();
			WriteMemory(0x85, 1, 0x00445488);
		}
	}
}

void PVZ_1_2_CN_REPAIR::asm_destroy_model()
{
	mov_exx_dword_ptr(RegType::EBX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EBX, OFFSET_1);
	call(0x00413630);
}
void PVZ_1_2_CN_REPAIR::asm_create_model()
{
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, 0x174);
	push_exx(RegType::EAX);
	call(0x00445470);
}
void PVZ_1_2_CN_REPAIR::LimitSeed(int* zombies, int znum, bool thief, bool red)
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
			WriteMemory(0x80, 1, 0x00445488);
			RunAsm();
			WriteMemory(0x85, 1, 0x00445488);
		}
	}
}
void PVZ_1_2_CN_REPAIR::asm_init_seed()
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
	call(0x0042cff0);
}

void PVZ_1_2_CN_REPAIR::asm_init_zlist()
{
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	call(0x0040e690);
}

void PVZ_1_2_CN_REPAIR::ResetSeed()
{
	if (InGame())
	{
		DWORD addr = ReadMemory(4, DATA_ADDR, 1, OFFSET_1) + 0x54ec;
		for (int i = 0; i < 34; ++i)
		{
			WriteMemory(0, 1, addr + i);
		}
		clear();
		asm_init_seed();
		asm_init_zlist();
		if (ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET+4) == 2)
		{
			asm_destroy_model();
			asm_create_model();
			WriteMemory(0x80, 1, 0x00445488);
		}
		RunAsm();
		WriteMemory(0x85, 1, 0x00445488);
	}
}

void PVZ_1_2_CN_REPAIR::SeeLeftZombies(bool(&zombies)[33])
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

void PVZ_1_2_CN_REPAIR::FlowerNPK(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x0053c864);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x220);
	}
	else
	{
		WriteMemory(-1, 1, 0x0053c864);
	}
}

void PVZ_1_2_CN_REPAIR::Medicine(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x0053c931);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x224);
	}
	else
	{
		WriteMemory(-1, 1, 0x0053c931);
	}
}

void PVZ_1_2_CN_REPAIR::Chocolate(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x0053c583);
		WriteMemory(0x39, 1, 0x0053c5ca);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x250);
	}
	else
	{
		WriteMemory(-1, 1, 0x0053c583);
		WriteMemory(-1, 1, 0x0053c5ca);
	}
}

void PVZ_1_2_CN_REPAIR::TreeNPK(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x004368c9);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x258);
	}
	else
	{
		WriteMemory(-1, 1, 0x004368c9);
	}
}

void PVZ_1_2_CN_REPAIR::TreeHeight(int h)
{
	if (GetMode() == 50)
	{
		WriteMemory(h - 1, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x11c);
		clear();
		mov_exx_dword_ptr(RegType::EBX, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::EBX, OFFSET_1);
		mov_exx_dword_ptr_exx_add_dword(RegType::EBX, OFFSET_2);
		push_exx(RegType::EBX);
		call(0x004366a0);
		RunAsm();
	}
	else
		WriteMemory(h, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x11c);
}

void PVZ_1_2_CN_REPAIR::FlowerNum(int n)
{
	WriteMemory(n, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x378);
}

void PVZ_1_2_CN_REPAIR::ChangeFlower(int fid, int type)
{
	WriteMemory(type, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x380 + fid * 0x58);
}

void PVZ_1_2_CN_REPAIR::ChangeFSize(int fid, int size)
{
	WriteMemory(size, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x3a4 + fid * 0x58);
}

void PVZ_1_2_CN_REPAIR::ChangeFDir(int fid, int dir)
{
	WriteMemory(dir, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x390 + fid * 0x58);
}

void PVZ_1_2_CN_REPAIR::ChangeFPlace(int fid, int place)
{
	WriteMemory(place, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x384 + fid * 0x58);
}

void PVZ_1_2_CN_REPAIR::Patch()
{
	WriteMemory(0x0ceb, 2, 0x00423cf1);
	WriteMemory(0x10eb, 2, 0x00423e31);
	WriteMemory(0x80, 1, 0x00412750);
	WriteMemory(0xeb, 1, 0x00425ec8);
	WriteMemory(0xeb, 1, 0x00425ef9);
	WriteMemory(0x70, 1, 0x006b6353);

	WriteMemory(0x23, 1, 0x00558f12);
	WriteMemory(0, 2, 0x005596a0);
}

std::string PVZ_1_2_CN_REPAIR::getUserdataDir(char systemDir)
{
	return PVZ::getUserdataDir(systemDir);
}
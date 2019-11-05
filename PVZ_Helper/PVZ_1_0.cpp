#include "stdafx.h"
#include "PVZ_1_0.h"

#define DATA_ADDR 0x006a9ec0
#define OFFSET_1 0x768
#define OFFSET_2 0x160
#define MODE_OFFSET 0x7f8
#define GARDON_OFFSET 0x82c
#define EXCODE_SAVE_ADDR 0x00426fb7

void PVZ_1_0::RunAsm()
{
	if (getLength() > 0)
	{
		WriteMemory(-2, 1, 0x00552014);
		Sleep(10);
		ret();
		runThread(GetHandle(), eCode.Thread);
		WriteMemory(0xdb, 1, 0x00552014);
	}
}
void PVZ_1_0::GetPlants()
{
	vp.clear();
	Plant one;
	one.addr = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0xac);
	int pnum = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0xbc);
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
void PVZ_1_0::GetItems()
{
	vi.clear();
	Item one;
	one.addr = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x11c);
	int inum = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x12c);
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
void PVZ_1_0::GetCars()
{
	vc.clear();
	Car one;
	one.addr = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x100);
	int cnum = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x110);
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
void PVZ_1_0::GetZombies()
{
	vz.clear();
	Zombie one;
	one.addr = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x90);
	int znum = ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0xa0);
	for (int i = 0; i < znum; one.addr += 0x15c)
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
int PVZ_1_0::GetMapType()
{
	return ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x554c); 
}
int PVZ_1_0::GetBlockType(int row, int col)/*1.草地2.裸地3.水池*/
{
	return ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x168 + row * 4 + col * 24);
}
void PVZ_1_0::SetBlockType(int row, int col, int type)/*1.草地2.裸地3.水池*/
{
	WriteMemory(type, 4, DATA_ADDR, 2, OFFSET_1, 0x168 + row * 4 + col * 24);
}
int PVZ_1_0::GetRowType(int row)/*0.不出怪1.草地2.水路*/
{
	return ReadMemory(4, DATA_ADDR, 2, OFFSET_1, 0x5d8 + row * 4);
}
void PVZ_1_0::SetRowType(int row, int type)/*0.不出怪1.草地2.水路*/
{
	WriteMemory(type, 4, DATA_ADDR, 2, OFFSET_1, 0x5d8 + row * 4);
}
bool PVZ_1_0::InGame()
{
	return ReadMemory(4, DATA_ADDR, 1, OFFSET_1) != 0;
}

int PVZ_1_0::GetMode()
{
	return ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET);
}

void PVZ_1_0::SetSun(int sun)
{
	WriteMemory(sun, 4, DATA_ADDR, 2, OFFSET_1, 0x5560);
}

void PVZ_1_0::SetMoney(int money)
{
	WriteMemory(money, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x28);
}

void PVZ_1_0::IgnoreSun(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0041ba72);
		WriteMemory(0x3b, 1, 0x0041ba74);
		WriteMemory(0x91, 1, 0x0041bac0);
		WriteMemory(0x80, 1, 0x00427a92);
		WriteMemory(0x80, 1, 0x00427dfd);
		WriteMemory(0xeb, 1, 0x0042487f);
	}
	else
	{
		WriteMemory(0x7f, 1, 0x0041ba72);
		WriteMemory(0x2b, 1, 0x0041ba74);
		WriteMemory(0x9e, 1, 0x0041bac0);
		WriteMemory(0x8f, 1, 0x00427a92);
		WriteMemory(0x8f, 1, 0x00427dfd);
		WriteMemory(0x74, 1, 0x0042487f);
	}
}

void PVZ_1_0::AutoPickup(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x0043158f);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0043158f);
	}
}

void PVZ_1_0::StartCar()
{
	if (InGame())
	{
		clear();
		GetCars();
		for (auto& rc:vc)
		{
			mov_exx(RegType::ESI, rc.addr);
			call(0x00458da0);
		}
		RunAsm();
	}
}

void PVZ_1_0::ResetCar()
{
	if (InGame())
	{
		clear();
		GetCars();
		for (auto& rc : vc)
		{
			mov_exx(RegType::EAX, rc.addr);
			call(0x00458d10);
		}
		WriteMemory(0x60eb, 2, 0x0040bc98);
		WriteMemory(1, 1, 0x0040bd17);
		WriteMemory(0x99fc, 2, 0x00458002);
		mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
		push_exx(RegType::EAX);
		call(0x0040bc70);
		RunAsm();
		WriteMemory(0x0975, 2, 0x0040bc98);
		WriteMemory(0, 1, 0x0040bd17);
		WriteMemory(0x9bf8, 2, 0x00458002);
	}
}

void PVZ_1_0::asm_plant(int row, int col, int type, bool copy)
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
		call(0x0040d120);
		mov_exx_eyy(RegType::ESI, RegType::EAX);
		push_exx(RegType::EAX);
		call(0x00466b80);
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
		call(0x0040d120);
	}
}

void PVZ_1_0::asm_plant_paper(int row, int col, int type)
{
	asm_plant(row, col, type, false);
	push_exx(RegType::EAX);
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_2);
	call(0x0042a530);
}

void PVZ_1_0::NoPause(bool b)
{
	if (b)
	{
		WriteMemory(0xc3, 1, 0x0054ebef);
	}
	else
	{
		WriteMemory(0x57, 1, 0x0054ebef);
	}
}

void PVZ_1_0::asm_put_zombie(int row, int col, int type)
{
	push_byte(col);
	push_byte(type);
	mov_exx(RegType::EAX, row);
	mov_exx_dword_ptr(RegType::ECX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_2);
	call(0x0042a0f0);
}

void PVZ_1_0::asm_put_boss()
{
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	push_byte(0);
	push_byte(25);
	call(0x0040ddc0);
}

void PVZ_1_0::asm_put_ladder(int row, int col)
{
	mov_exx(RegType::EDI, row);
	push_byte(col);
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	call(0x00408f40);
}

void PVZ_1_0::asm_put_grave(int row, int col)
{
	mov_exx_dword_ptr(RegType::EDX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EDX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::EDX, OFFSET_2);
	push_exx(RegType::EDX);
	mov_exx(RegType::EDI, row);
	mov_exx(RegType::EBX, col);
	call(0x00426620);
}

// static void asm_put_rake(int row, int col)
// {
// 	push_byte(row);
// 	push_byte(col);
// 	mov_exx_dword_ptr(ECX, DATA_ADDR);
// 	mov_exx_dword_ptr_exx_add_dword(ECX, OFFSET_1);
// 	push_exx(ECX);
// 	call(0x0040b9c0);
// 	add_esp(8);
// }
// 
// void PVZ_1_0::PutRake(int row, int col)
// {
// 	if (InGame())
// 	{
// 		Map_1_0 map(process);
// 		int Rows = map.GetRowCount();
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
// 		WriteMemory(0x0000a681, 4, 0x0040b9e3);
// 		WriteMemory(0, 1, 0x0040bb2b);
// 		WriteMemory(0x900c4d8b, 4, 0x0040bb3b);
// 		WriteMemory(0x9010458b, 4, 0x0040bb41);
// 		RunAsm();
// 		WriteMemory(0x00027984, 4, 0x0040b9e3);
// 		WriteMemory(-1, 1, 0x0040bb2b);
// 		WriteMemory(0x10244c8b, 4, 0x0040bb3b);
// 		WriteMemory(0x1424448b, 4, 0x0040bb41);
// 	}
// }

void PVZ_1_0::NoDelete(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0054b267);
	}
	else
	{
		WriteMemory(0x74, 1, 0x0054b267);
	}
}

void PVZ_1_0::NoSave(bool b)
{
	if (b)
	{
		WriteMemory(0x2e, 1, 0x00482149);
	}
	else
	{
		WriteMemory(0x13, 1, 0x00482149);
	}
}

void PVZ_1_0::Anywhere(bool b)
{
	if (b)
	{
		WriteMemory(0x81, 1, 0x0040fe30);
		WriteMemory(0xeb, 1, 0x00438e40);
		WriteMemory(0x8d, 1, 0x0042a2d9);
	}
	else
	{
		WriteMemory(0x84, 1, 0x0040fe30);
		WriteMemory(0x74, 1, 0x00438e40);
		WriteMemory(0x84, 1, 0x0042a2d9);
	}
}

void PVZ_1_0::FastBelt(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x00422d20);
		WriteMemory(0x33, 1, 0x00489ca1);
	}
	else
	{
		WriteMemory(0x8f, 1, 0x00422d20);
		WriteMemory(0x85, 1, 0x00489ca1);
	}
}

void PVZ_1_0::ShowHide(bool b)
{
	if (b)
	{
		WriteMemory(0x38, 1, 0x0042df5d);
	}
	else
	{
		WriteMemory(0x88, 1, 0x0042df5d);
	}
}

void PVZ_1_0::FullFog(bool b)
{
	if (b)
	{
		WriteMemory(0xc3c033, 3, 0x0041c1c0);
		WriteMemory(0x3feb, 2, 0x0041a476);
		WriteMemory(0, 1, 0x0041a4ba);
	}
	else
	{
		DWORD FogAddr = ReadMemory(4, DATA_ADDR, 1, OFFSET_1) + 0x4c8;
		for (int i = 0; i < 63; ++i)
		{
			WriteMemory(0, 4, FogAddr + i * 4);
		}
		WriteMemory(0x8c818b, 3, 0x0041c1c0);
		WriteMemory(0x0575, 2, 0x0041a476);
		WriteMemory(4, 1, 0x0041a4ba);
	}
}

void PVZ_1_0::SeeFog(bool b)
{
	if (b)
	{
		WriteMemory(0xd231, 2, 0x0041a68d);
	}
	else
	{
		WriteMemory(0xf23b, 2, 0x0041a68d);
	}
}

void PVZ_1_0::SeeVase(bool b)
{
	if (b)
	{
		WriteMemory(0x0033b866, 4, 0x0044e5cc);
	}
	else
	{
		WriteMemory(0x067ec085, 4, 0x0044e5cc);
	}
}

void PVZ_1_0::JmpLevel(int level)
{
	WriteMemory(level, 4, DATA_ADDR, 3, OFFSET_1, OFFSET_2, 0x6c);
}

void PVZ_1_0::MixMode(int mode, int level)
{
	if (mode == 0)
	{
		WriteMemory(level, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x24);
		WriteMemory(level, 4, DATA_ADDR, 2, OFFSET_1, 0x5550);
	}
	WriteMemory(mode, 4, DATA_ADDR, 1, MODE_OFFSET);
}

void PVZ_1_0::Unlock(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00454109);
		WriteMemory(0x70, 1, 0x0044a514);
		WriteMemory(0xc3c031, 3, 0x0042e440);
		WriteMemory(0xeb, 1, 0x00449e9d);
		WriteMemory(0xc3c030, 3, 0x0048aad0);
		WriteMemory(0xeb, 1, 0x0048a54c);
		WriteMemory(0xeb, 1, 0x0048d32b);
		WriteMemory(0xeb, 1, 0x0048c491);
		WriteMemory(0xeb, 1, 0x00449e7a);
		WriteMemory(0xeb, 1, 0x00453ad1);
		WriteMemory(0x5beb01b0, 4, 0x00403a10);
		WriteMemory(0, 4, 0x0069dca0);
		WriteMemory(0xc301b0, 3, 0x00403b30);
	}
	else
	{
		WriteMemory(0x7e, 1, 0x00454109);
		WriteMemory(0x7e, 1, 0x0044a514);
		WriteMemory(0x555351, 3, 0x0042e440);
		WriteMemory(0x7f, 1, 0x00449e9d);
		WriteMemory(0xd98b53, 3, 0x0048aad0);
		WriteMemory(0x7f, 1, 0x0048a54c);
		WriteMemory(0x7f, 1, 0x0048d32b);
		WriteMemory(0x7f, 1, 0x0048c491);
		WriteMemory(0x7f, 1, 0x00449e7a);
		WriteMemory(0x7f, 1, 0x00453ad1);
		WriteMemory(0x6c8b5551, 4, 0x00403a10);
		WriteMemory(40, 4, 0x0069dca0);
		WriteMemory(0x6c808b, 3, 0x00403b30);
	}
}

void PVZ_1_0::Win()
{
	int mode = GetMode();
	if (!InGame() ||
		mode >= 1 && mode <= 15 && ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET + 4) != 3)
	{
		return;
	}
	clear();
	mov_exx_dword_ptr(RegType::ECX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::ECX, OFFSET_1);
	call(0x0040c3e0);
	RunAsm();
}

void PVZ_1_0::Lock(bool b, int mode)
{
	if (b)
	{
		static const int val[] = { 24, -624, 73, 100, 122, 190, 249, 360 };
		WriteMemory(0x81, 1, 0x0042b047);
		WriteMemory(val[mode], 4, 0x0042b048);
	}
	else
	{
		WriteMemory(0x85, 1, 0x0042b047);
		WriteMemory(144, 4, 0x0042b048);
	}
}

void PVZ_1_0::Little(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x00523ed5);
	}
	else
	{
		WriteMemory(0x74, 1, 0x00523ed5);
	}

}

void PVZ_1_0::Rain(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00426b4e);
		WriteMemory(0x70, 1, 0x00416f07);
		WriteMemory(0xeb, 1, 0x00424715);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00426b4e);
		WriteMemory(0x75, 1, 0x00416f07);
		WriteMemory(0x74, 1, 0x00424715);
	}
}

void PVZ_1_0::Storm(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x00416eef);
		WriteMemory(0xeb, 1, 0x00426b6b);
		WriteMemory(0xeb, 1, 0x00426b35);
		WriteMemory(0xeb, 1, 0x004246c9);
		WriteMemory(0xeb, 1, 0x00424729);
	}
	else
	{
		WriteMemory(0x74, 1, 0x00416eef);
		WriteMemory(0x74, 1, 0x00426b6b);
		WriteMemory(0x74, 1, 0x00426b35);
		WriteMemory(0x74, 1, 0x004246c9);
		WriteMemory(0x74, 1, 0x00424729);
	}
}

void PVZ_1_0::Force(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0046d546);
		WriteMemory(0x70, 1, 0x0046dc28);
		WriteMemory(0x70, 1, 0x00525bed);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0046d546);
		WriteMemory(0x75, 1, 0x0046dc28);
		WriteMemory(0x75, 1, 0x00525bed);
	}
}

void PVZ_1_0::LikeCol(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x0043903d);
		WriteMemory(0x80, 1, 0x00410ae7);
	}
	else
	{
		WriteMemory(0x85, 1, 0x0043903d);
		WriteMemory(0x85, 1, 0x00410ae7);
	}
}

void PVZ_1_0::RunFast(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x004248aa);
	}
	else
	{
		WriteMemory(0x75, 1, 0x004248aa);
	}
}

void PVZ_1_0::SetCardNum(int num)
{
	if (InGame())
	{
		WriteMemory(0xc3, 1, 0x0041bee0);
		clear();
		mov_exx(RegType::EAX, num);
		mov_exx_dword_ptr(RegType::ESI, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::ESI, OFFSET_1);
		mov_exx_dword_ptr_exx_add_dword(RegType::ESI, 0x144);
		call(0x00489cd0);
		RunAsm();
		WriteMemory(0x56, 1, 0x0041bee0);
	}
}

void PVZ_1_0::SetCard(int cid, int type, bool copy)
{
	if (copy)
	{
		WriteMemory(48, 4, DATA_ADDR, 3, OFFSET_1, 0x144, 0x5c + cid * 0x50);
		WriteMemory(type, 4, DATA_ADDR, 3, OFFSET_1, 0x144, 0x60 + cid * 0x50);
	}
	else
	{
		WriteMemory(type, 4, DATA_ADDR, 3, OFFSET_1, 0x144, 0x5c + cid * 0x50);
	}
}

void PVZ_1_0::FSBoom(bool b)
{
	if (b)
	{
		WriteMemory(0x20eb, 2, 0x0041d8ff);
		WriteMemory(0x70, 1, 0x004664f2);
	}
	else
	{
		WriteMemory(0x3e7f, 2, 0x0041d8ff);
		WriteMemory(0x75, 1, 0x004664f2);
	}
}

void PVZ_1_0::FSSquash(bool b)
{
	if (b)
	{
		WriteMemory(0x45eb, 2, 0x00460929);
		WriteMemory(0x80, 1, 0x00460838);
		WriteMemory(0x5feb, 2, 0x00460747);
	}
	else
	{
		WriteMemory(0x707f, 2, 0x00460929);
		WriteMemory(0x85, 1, 0x00460838);
		WriteMemory(0x7275, 2, 0x00460747);
	}
}

void PVZ_1_0::FSMagnet(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x004620a3);
		WriteMemory(0x70, 1, 0x00462124);
	}
	else
	{
		WriteMemory(0x87, 1, 0x004620a3);
		WriteMemory(0x74, 1, 0x00462124);
	}
}

void PVZ_1_0::FSSputter(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0046d455);
	}
	else
	{
		WriteMemory(0x74, 1, 0x0046d455);
	}
}

void PVZ_1_0::FSMouth(bool b)
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

void PVZ_1_0::FSPrick(bool b)
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

void PVZ_1_0::FSSpout(bool b)
{
	if (b)
	{
		WriteMemory(1, 1, eCodeStart + EXCODE_VAR_FS_SPOUT);
		WriteMemory(0x39eb, 2, 0x0046768d);
		WriteMemory(0x16eb, 2, 0x0045ed5c);
	}
	else
	{
		WriteMemory(0, 1, eCodeStart + EXCODE_VAR_FS_SPOUT);
		WriteMemory(0x8c0f, 2, 0x0046768d);
		WriteMemory(0x8c0f, 2, 0x0045ed5c);
	}
}

void PVZ_1_0::FSCat(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x004677ef);
		WriteMemory(1, 1, eCodeStart + EXCODE_VAR_FS_CAT);
		WriteMemory(0x1beb, 2, 0x00464a52);
		static unsigned char code1[] = { 0x33, 0xc0, 0xb0, 0x03, 0xeb, 0x0f };
		WriteArrayMemory(code1, sizeof(code1), 0x0045eb10);
		WriteMemory(0x3f, 1, 0x00464a61);
		static unsigned char _3_code[] = { 0xE9, 0x91, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x12, 0x75, 0xE9, 0x8B, 0x87, 0x90, 0x00, 0x00, 0x00, 0x83, 0xF8, 0x09, 0x74, 0xBE, 0x83, 0xF8, 0x05, 0x74, 0xB9, 0x83, 0xF8, 0x01, 0x74, 0xB4, 0xE9, 0x74, 0xFE, 0xFF, 0xFF };
		WriteArrayMemory(_3_code, sizeof(_3_code), 0x00464a9c);
		WriteMemory(0xeb, 1, 0x004672f4);
		WriteMemory(0x70, 1, 0x00467369);
		WriteMemory(0xe9, 1, 0x00464bbc);
		WriteMemory(0xfffffeae, 4, 0x00464bbd);
		WriteMemory(0x02ebd175, 4, 0x004673b3);
		WriteMemory(0x80, 1, 0x004672d1);
		WriteMemory(0x80, 1, 0x004672da);
		WriteMemory(0x80, 1, 0x004672e3);
		WriteMemory(0x80, 1, 0x004672ec);
		WriteMemory(0x80, 1, 0x00467353);
		WriteMemory(0x80, 1, 0x0046735c);
		WriteMemory(0xeb, 1, 0x0046736e);
	}
	else
	{
		WriteMemory(0x75, 1, 0x004677ef);
		WriteMemory(0, 1, eCodeStart + EXCODE_VAR_FS_CAT);
		WriteMemory(0x4f8b, 2, 0x00464a52);
		static unsigned char code1[] = { 0x8B, 0x40, 0x24, 0x83, 0xF8, 0x1A };
		WriteArrayMemory(code1, sizeof(code1), 0x0045eb10);
		WriteMemory(0x2d, 1, 0x00464a61);
		static unsigned char _3_code[] = { 0x83, 0xF8, 0x12, 0x0F, 0x85, 0x8D, 0x00, 0x00, 0x00, 0x8B, 0x5F, 0x1C, 0x8B, 0x37, 0x8B, 0x8F, 0x9C, 0x00, 0x00, 0x00, 0x8D, 0x53, 0xFF, 0x8B, 0xC6, 0x89, 0x54, 0x24, 0x14, 0xE8, 0xF2, 0xF1, 0xFE, 0xFF, 0x8B, 0x8F };
		WriteArrayMemory(_3_code, sizeof(_3_code), 0x00464a9c);
		WriteMemory(0x75, 1, 0x004672f4);
		WriteMemory(0x74, 1, 0x00467369);
		WriteMemory(0x8b, 1, 0x00464bbc);
		WriteMemory(0x006a1c4f, 4, 0x00464bbd);
		WriteMemory(0x0193850f, 4, 0x004673b3);
		WriteMemory(0x84, 1, 0x004672d1);
		WriteMemory(0x84, 1, 0x004672da);
		WriteMemory(0x84, 1, 0x004672e3);
		WriteMemory(0x84, 1, 0x004672ec);
		WriteMemory(0x84, 1, 0x00467353);
		WriteMemory(0x84, 1, 0x0046735c);
		WriteMemory(0x75, 1, 0x0046736e);
	}
}

void PVZ_1_0::CatTouch(bool b)
{
	if (b)
	{
		WriteMemory(0x81, 1, 0x0046cecc);
	}
	else
	{
		WriteMemory(0x85, 1, 0x0046cecc);
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
		_jmp(0x00467884);
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
		_jmp(0x00467881);
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
		_jmp(0x0045ee76);
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
		_jmp(0x0045ee76);
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
		jne check;
	allow:
		pop eax;
		_jmp(0x004676c8);
	check:
		push eax;
		mov eax, ds : [EXCODE_SAVE_ADDR];
		cmp byte ptr[eax + EXCODE_VAR_FS_CAT], 1;
		pop eax;
		jne not_allow;
		cmp eax, 4;
		je not_allow;
		cmp eax, 6;
		je not_allow;
		cmp eax, 10;
		je not_allow;
		cmp eax, 42;
		je not_allow;
		cmp eax, 21;
		je not_allow;
		cmp eax, 46;
		jne allow;
	not_allow:
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
		mov ecx, ds:[EXCODE_SAVE_ADDR];
		cmp byte ptr[ecx + EXCODE_VAR_FS_CAT], 1;
		jne not_allow;
		mov ecx, [ebp + 8];
		mov ecx, [ecx + 0x24];
		cmp ecx, 4;
		je not_allow;
		cmp ecx, 6;
		je not_allow;
		cmp ecx, 10;
		je not_allow;
		cmp ecx, 42;
		je not_allow;
		cmp ecx, 21;
		je not_allow;
		cmp ecx, 46;
		je not_allow;
		pop ecx;
		_jmp(0x004677e0);
	not_allow:
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
		push eax;
		mov eax, ds:[EXCODE_SAVE_ADDR];
		cmp byte ptr[eax + EXCODE_VAR_FS_CAT], 1;
		pop eax;
		jne not_open;
		cmp dword ptr[edi + 0x24], 47;
		je _2;
		test eax, eax;
		jne _2;
		_jmp(0x00464934);
	_2:
		mov ecx, [edi + 0x1c];
		push esi;
		_jmp(0x00464a88);
	not_open:
		mov ecx, [edi + 0x1c];
		push esi;
		push ecx;
		_jmp(0x00464c2d);
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
		jne check_split;
	left:
		fld dword ptr ds : [0x006799e8];
		ret;
	check_split:
		cmp eax, 28;
		jne right;
		cmp edx, 1;
		je left;
	right:
		fld dword ptr ds : [0x006793e0];
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
		push eax;
		mov eax, ds:[EXCODE_SAVE_ADDR];
		cmp byte ptr[eax + EXCODE_VAR_FS_CAT], 1;
		pop eax;
		jne not_open;
		mov ecx, [edi + 0x1c];
		push ecx;
		push edi;
		xor ecx, ecx;
		_call(0x004675c0);
		test eax, eax;
		jne _2;
		_jmp(0x00464934);
	_2:
		push eax;
		mov ecx, [edi + 0x1c];
		push 0;
		push ecx;
		push eax;
		push edi;
		_call(0x00466e00);
		pop eax;
		mov edx, [edi + 0x1c];
		push 1;
		push edx;
		push eax;
		push edi;
		_call(0x00466e00);
		_jmp(0x00464934);
	not_open:
		mov esi, 3;
		_jmp(0x00464b5a);
		int 3;
		int 3;
	fend:
	}
}

static void __declspec(naked) ZTimerCode()
{
	_asm {
		jmp fend;
		mov ecx, 0xffffffff;
		cmp[edi + 0x559c], ecx;
		ja _1;
		dec dword ptr ds : [edi + 0x559c];
		ret;
	_1:
		mov[edi + 0x559c], ecx;
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
		mov eax, [eax + 0x554c];
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
		mov eax, [eax + 0x554c];
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
		mov eax, [eax + 0x554c];
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
		inc edi;
		cmp edi, eax;
		ret;
		int 3;
		int 3;
	fend:
	}
}

void PVZ_1_0::InitExtraCode()
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
		Caller = 0x0042706c;
		static unsigned char CallCode[] = { 0xb9, 4, 0, 0, 0, 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[6] = temp - Caller - 10;
		WRITE_CODE(DoorCode);
	}
	temp += SIZE_OF(DoorCode);
	/*行判定*/
	if (!b)
	{
		Caller = 0x004676c2;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(RowCode);
	}
	temp += SIZE_OF(RowCode);
	/*列判定*/
	if (!b)
	{
		Caller = 0x004677da;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ColCode);
	}
	temp += SIZE_OF(ColCode);
	/*行伤害判定*/
	if (!b)
	{
		Caller = 0x0045ed6e;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(RHurtCode);
	}
	temp += SIZE_OF(RHurtCode);
	/*列伤害判定*/
	if (!b)
	{
		Caller = 0x0045ede2;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CHurtCode);
	}
	temp += SIZE_OF(CHurtCode);
	/*香蒲免行判定*/
	if (!b)
	{
		Caller = 0x00467680;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatRCode);
	}
	temp += SIZE_OF(CatRCode);
	/*香蒲免列判定*/
	if (!b)
	{
		Caller = 0x004677c0;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatCCode);
	}
	temp += SIZE_OF(CatCCode);
	/*投手特殊处理*/
	if (!b)
	{
		Caller = 0x00464c28;
		static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ThrowCode);
	}
	temp += SIZE_OF(ThrowCode);
	/*猫式全屏子弹方向处理*/
	if (!b)
	{
		Caller = 0x00467386;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(CatDirCode);
	}
	temp += SIZE_OF(CatDirCode);
	/*裂荚处理*/
	if (!b)
	{
		Caller = 0x00464b55;
		static unsigned char CallCode[] = { 0xe9, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(SplitCode);
	}
	temp += SIZE_OF(SplitCode);
	/*刷怪倒计时*/
	eCode.ZTimer = temp;
	if (!b)
	{
		Caller = 0x00413e45;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(ZTimerCode);
	}
	temp += SIZE_OF(ZTimerCode);
	/*僵博吐球行*/
	if (!b)
	{
		Caller = 0x00535484;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(GetHeadRowsCode);
	}
	temp += SIZE_OF(GetHeadRowsCode);
	if (!b)
	{
		Caller = 0x00534aeb;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(GetRVRowsCode);
	}
	temp += SIZE_OF(GetRVRowsCode);
	/*传送门行*/
	if (!b)
	{
		Caller = 0x00427535;
		static unsigned char CallCode[] = { 0xe8, 0, 0, 0, 0, 0x90 };
		(DWORD&)CallCode[1] = temp - Caller - 5;
		WRITE_CODE(DoorRowsCode);
	}
	temp += SIZE_OF(DoorRowsCode);


	eCode.Thread = temp;
	return;
}

void PVZ_1_0::StartDoor(bool b)
{
	if (b)
	{
		if (InGame() && ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET + 4) > 2)
		{
			bool HasDoor = false;
			GetItems();
			for (auto& ri:vi)
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
		WriteMemory(0x70, 1, 0x00467665);
		WriteMemory(0x70, 1, 0x004248ce);
		WriteMemory(0x70, 1, 0x0041ffb4);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00467665);
		WriteMemory(0x75, 1, 0x004248ce);
		WriteMemory(0x75, 1, 0x0041ffb4);
	}
}

void PVZ_1_0::StableDoor(bool b)
{
	if (b)
	{
		WriteMemory(0, 1, 0x004276dd);
	}
	else
	{
		WriteMemory(-1, 1, 0x004276dd);
	}
}

void PVZ_1_0::SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c)
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
				call(0x0044d000);
			}
		}
		WriteMemory(b1c, 1, 0x00426fe2);
		WriteMemory(b1r, 1, 0x00426fe9);
		WriteMemory(b2c, 1, 0x0042700d);
		WriteMemory(b2r, 1, 0x00427014);
		WriteMemory(w1c, 1, 0x0042703d);
		WriteMemory(w1r, 1, 0x00427044);
		WriteMemory(w2c, 1, 0x00427068);
		WriteMemory(w2r, 1, 0x0042706d);
		mov_exx_dword_ptr(RegType::EDI, DATA_ADDR);
		mov_exx_dword_ptr_exx_add_dword(RegType::EDI, OFFSET_1);
		mov_exx_dword_ptr_exx_add_dword(RegType::EDI, OFFSET_2);
		call(0x00426fc0);
		RunAsm();
		WriteMemory(2, 1, 0x00426fe2);
		WriteMemory(0, 1, 0x00426fe9);
		WriteMemory(9, 1, 0x0042700d);
		WriteMemory(1, 1, 0x00427014);
		WriteMemory(9, 1, 0x0042703d);
		WriteMemory(3, 1, 0x00427044);
		WriteMemory(2, 1, 0x00427068);
		WriteMemory(4, 1, 0x0042706d);
	}
}

void PVZ_1_0::CornNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x0046103b);
	}
	else
	{
		WriteMemory(0x85, 1, 0x0046103b);
	}
}

void PVZ_1_0::MagnetNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x00461e37);
	}
	else
	{
		WriteMemory(0x85, 1, 0x00461e37);
	}
}

void PVZ_1_0::PotatoNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x80, 1, 0x0045fe54);
	}
	else
	{
		WriteMemory(0x85, 1, 0x0045fe54);
	}
}

void PVZ_1_0::MouthNoWait(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00461565);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00461565);
	}
}

void PVZ_1_0::PStrong()
{
	WriteMemory(0x004046, 3, 0x0052fcf1);
	WriteMemory(0x909090, 3, 0x0046cfeb);
	WriteMemory(0x909090, 3, 0x0046d7a6);
	WriteMemory(0xeb, 1, 0x0052e93b);
	WriteMemory(0xeb, 1, 0x0041cc2f);
	WriteMemory(0xeb, 1, 0x005276ea);
	WriteMemory(0x70, 1, 0x0045ee0a);
	WriteMemory(0, 1, 0x0045ec66);
}

void PVZ_1_0::PNormal()
{
	WriteMemory(0xfc4046, 3, 0x0052fcf1);
	WriteMemory(0x405029, 3, 0x0046cfeb);
	WriteMemory(0x404e29, 3, 0x0046d7a6);
	WriteMemory(0x74, 1, 0x0052e93b);
	WriteMemory(0x74, 1, 0x0041cc2f);
	WriteMemory(0x75, 1, 0x005276ea);
	WriteMemory(0x75, 1, 0x0045ee0a);
	WriteMemory(-32, 1, 0x0045ec66);
}

void PVZ_1_0::PWeak()
{
	WriteMemory(0x004066, 3, 0x0052fcf1);
	WriteMemory(0x404029, 3, 0x0046cfeb);
	WriteMemory(0x407629, 3, 0x0046d7a6);
	WriteMemory(0x74, 1, 0x0052e93b);
	WriteMemory(0x74, 1, 0x0041cc2f);
	WriteMemory(0x75, 1, 0x005276ea);
	WriteMemory(0x70, 1, 0x0045ee0a);
	WriteMemory(-32, 1, 0x0045ec66);
}

void PVZ_1_0::NoMark(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0041d79e);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0041d79e);
	}
}

void PVZ_1_0::NoSleep(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x0045de8e);
		if (InGame())
		{
			clear();
			GetPlants();
			for (auto& rp:vp)
			{
				if (rp.Sleeping)
				{
					mov_exx(RegType::EAX, rp.addr);
					push_byte(0);
					call(0x0045e860);
				}
			}
			RunAsm();
		}
	}
	else
	{
		WriteMemory(0x74, 1, 0x0045de8e);
	}
}

void PVZ_1_0::StrongWind(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x00466601);
	}
	else
	{
		WriteMemory(0x74, 1, 0x00466601);
	}
}

void PVZ_1_0::StrongBean(bool b)
{
	if (b)
	{
		WriteMemory(0xc3, 1, 0x0046e429);
	}
	else
	{
		WriteMemory(0xe9, 1, 0x0046e429);
	}
}

void PVZ_1_0::Umbrella(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x0041d3f8);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0041d3f8);
	}
}

void PVZ_1_0::NoCD(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00487296);
		WriteMemory(0xeb, 1, 0x00488250);
	}
	else
	{
		WriteMemory(0x7e, 1, 0x00487296);
		WriteMemory(0x75, 1, 0x00488250);
	}
}

void PVZ_1_0::LockScoop(bool b)
{
	if (b)
	{
		WriteMemory(0x39, 1, 0x004123a3);
		WriteMemory(6, 4, DATA_ADDR, 3, OFFSET_1, 0x138, 0x30);
	}
	else
	{
		WriteMemory(0x89, 1, 0x004123a3);
	}
}

void PVZ_1_0::AnyMove(bool b)
{
	if (b)
	{
		WriteMemory(0x81, 1, 0x004208fb);
		WriteMemory(0x81, 1, 0x00420533);
	}
	else
	{
		WriteMemory(0x85, 1, 0x004208fb);
		WriteMemory(0x85, 1, 0x00420533);
	}
}

void PVZ_1_0::NoPurple(bool b)
{
	if (b)
	{
		WriteMemory(0xc301b0, 3, 0x0041d7d0);
		WriteMemory(0xeb, 1, 0x0040e477);
	}
	else
	{
		WriteMemory(0xf88351, 3, 0x0041d7d0);
		WriteMemory(0x74, 1, 0x0040e477);
	}
}

void PVZ_1_0::LockButter(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00464bd6);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00464bd6);
	}
}

void PVZ_1_0::LockCorn(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x00464bd6);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00464bd6);
	}
}

void PVZ_1_0::BoomIm()
{
	WriteMemory(0x80, 1, 0x00526afc);
	WriteMemory(0x80, 1, 0x005275dd);
}

void PVZ_1_0::BoomNormal()
{
	WriteMemory(0x8f, 1, 0x00526afc);
	WriteMemory(0x85, 1, 0x005275dd);
}

void PVZ_1_0::BoomNever()
{
	WriteMemory(0x81, 1, 0x00526afc);
	WriteMemory(0x81, 1, 0x005275dd);
}

void PVZ_1_0::VisiableNo()
{
	WriteMemory(0x70, 1, 0x0052e357);
	WriteMemory(0x70, 1, 0x0053402b);
}

void PVZ_1_0::VisiableNormal()
{
	WriteMemory(0x75, 1, 0x0052e357);
	WriteMemory(0x75, 1, 0x0053402b);
}

void PVZ_1_0::VisiableYes()
{
	WriteMemory(0xeb, 1, 0x0052e357);
	WriteMemory(0xeb, 1, 0x0053402b);
}

void PVZ_1_0::ZStrong()
{
	WriteMemory(0xc0, 1, 0x00531045);
	WriteMemory(0x90909090, 4, 0x0053130f);
	WriteMemory(0xd233c033, 4, 0x00530c9b);
	WriteMemory(0x81, 1, 0x00532ba1);
	WriteMemory(0xeb, 1, 0x0046144a);
}

void PVZ_1_0::ZNormal()
{
	WriteMemory(0xc8, 1, 0x00531045);
	WriteMemory(0x20247c2b, 4, 0x0053130f);
	WriteMemory(0xc28b027c, 4, 0x00530c9b);
	WriteMemory(0x8d, 1, 0x00532ba1);
	WriteMemory(0x74, 1, 0x0046144a);
}

void PVZ_1_0::ZWeak()
{
	WriteMemory(0xc9, 1, 0x00531045);
	WriteMemory(0x9090ff2b, 4, 0x0053130f);
	WriteMemory(0xc28bd08b, 4, 0x00530c9b);
	WriteMemory(0x80, 1, 0x00532ba1);
	WriteMemory(0x74, 1, 0x0046144a);
}

void PVZ_1_0::NoSlow(bool b)
{
	if (b)
	{
		WriteMemory(0x90c033, 3, 0x0052b448);
	}
	else
	{
		WriteMemory(0xffc083, 3, 0x0052b448);
	}
}

void PVZ_1_0::NoButter(bool b)
{
	if (b)
	{
		WriteMemory(0x90c033, 3, 0x0052b464);
	}
	else
	{
		WriteMemory(0xffc083, 3, 0x0052b464);
	}
}

void PVZ_1_0::No3zGroup(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x0042694a);
		WriteMemory(0xeb, 1, 0x00413083);
	}
	else
	{
		WriteMemory(0x75, 1, 0x0042694a);
		WriteMemory(0x75, 1, 0x00413083);
	}
}

void PVZ_1_0::NoIMP(bool b)
{
	if (b)
	{
		WriteMemory(0xeb, 1, 0x00527205);
	}
	else
	{
		WriteMemory(0x7d, 1, 0x00527205);
	}
}

void PVZ_1_0::NoICE(bool b)
{
	if (b)
	{
		WriteMemory(0xc3, 1, 0x0052a7b0);
		WriteMemory(0xeb, 1, 0x0041f79a);
		for (int i = 0; i < 6; ++i)
		{
			WriteMemory(1, 4, DATA_ADDR, 2, OFFSET_1, 0x624 + i * 4);
		}
	}
	else
	{
		WriteMemory(0x51, 1, 0x0052a7b0);
		WriteMemory(0x75, 1, 0x0041f79a);
	}
}

void PVZ_1_0::ZEatable(bool b)
{
	if (b)
	{
		WriteMemory(0, 1, 0x00461457);
	}
	else
	{
		WriteMemory(1, 1, 0x00461457);
	}
}

void PVZ_1_0::StopZTimer(bool b)
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

void PVZ_1_0::StopZombies(bool b)
{
	if (b)
	{
		WriteMemory(0x54, 1, 0x0052ab2b);
		WriteMemory(0x54, 1, 0x0052ab34);
	}
	else
	{
		WriteMemory(0x64, 1, 0x0052ab2b);
		WriteMemory(0x44, 1, 0x0052ab34);
	}
}

void PVZ_1_0::StopBoss(bool b)
{
	if (b)
	{
		WriteMemory(0xc3, 1, 0x00536080);
	}
	else
	{
		WriteMemory(0x83, 1, 0x00536080);
	}
}

void PVZ_1_0::BalloonBoom(bool b)
{
	if (b)
	{
		WriteMemory(0x70, 1, 0x00525cab);
		WriteMemory(0x70, 1, 0x00525cc6);
	}
	else
	{
		WriteMemory(0x75, 1, 0x00525cab);
		WriteMemory(0x74, 1, 0x00525cc6);
	}
}

void PVZ_1_0::ZTimerSpeed(int value)
{
	WriteMemory(value, 4, eCode.ZTimer + 1);
}

void PVZ_1_0::GatherZombie(bool b, float pos)
{
	if (b)
	{
		static unsigned char code[] = { 0xc7, 0x46, 0x2c, 0, 0, 0, 0, 0xeb, 0x23 };
		(DWORD&)code[3] = (DWORD&)pos;
		WriteMemory(0x32, 1, 0x0052aa4a);
		WriteArrayMemory(code, 0x0052ab18);
	}
	else
	{
		static unsigned char code[] = { 0xe8, 0xc3, 0x13, 0, 0, 0x84, 0xc0, 0x75, 0x0f };
		WriteMemory(0x84, 1, 0x0052aa4a);
		WriteArrayMemory(code, 0x0052ab18);
	}
}

void PVZ_1_0::KillAllZombies()
{
	GetZombies();
	for (auto& rz : vz)
	{
		WriteMemory(3, 4, rz.addr + 0x28);
	}
}

void PVZ_1_0::ConfuseZombies()
{
	GetZombies();
	for (auto& rz : vz)
	{
		WriteMemory(1, 1, rz.addr + 0xb8);
	}
}

void PVZ_1_0::NaturalSeed(int* zombies, int znum)
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
		DWORD SeedAddr = ReadMemory(4, DATA_ADDR, 1, OFFSET_1) + 0x54d4;
		DWORD ListAddr = ReadMemory(4, DATA_ADDR, 1, OFFSET_1) + 0x6b4;
		for (int i = 0; i < 33; ++i)
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
		if (ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET + 4) == 2)
		{
			clear();
			asm_destroy_model();
			asm_create_model();
			WriteMemory(0x80, 1, 0x0043a153);
			RunAsm();
			WriteMemory(0x85, 1, 0x0043a153);
		}
	}
}
void PVZ_1_0::asm_destroy_model()
{
	mov_exx_dword_ptr(RegType::EBX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EBX, OFFSET_1);
	call(0x0040df70);
}

void PVZ_1_0::asm_create_model()
{
	mov_exx_dword_ptr(RegType::EAX, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::EAX, 0x15c);
	push_exx(RegType::EAX);
	call(0x0043a140);
}
void PVZ_1_0::LimitSeed(int* zombies, int znum, bool thief, bool red)
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
		DWORD ListAddr = ReadMemory(4, DATA_ADDR, 1, OFFSET_1) + 0x6b4;
		WriteArrayMemory(buffer, ListAddr);
		if (ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET + 4) == 2)
		{
			clear();
			asm_destroy_model();
			asm_create_model();
			WriteMemory(0x80, 1, 0x0043a153);
			RunAsm();
			WriteMemory(0x85, 1, 0x0043a153);
		}
	}
}

void PVZ_1_0::asm_init_seed()
{
	mov_exx_dword_ptr(RegType::EDI, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EDI, OFFSET_1);
	add_word(0xc781);
	add_dword(0x54d4);//add edi,0x54d4
	mov_exx(RegType::ECX, 33);
	add_word(0xc030);//xor al,al
	add_byte(0xfc);//cld
	add_word(0xaaf3);// rep stos byte ptr es : [edi]
	mov_exx_dword_ptr(RegType::ESI, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::ESI, OFFSET_1);
	mov_exx_dword_ptr_exx_add_dword(RegType::ESI, OFFSET_2);
	call(0x00425840);
}

void PVZ_1_0::asm_init_zlist()
{
	mov_exx_dword_ptr(RegType::EDI, DATA_ADDR);
	mov_exx_dword_ptr_exx_add_dword(RegType::EDI, OFFSET_1);
	call(0x004092e0);
}

void PVZ_1_0::ResetSeed()
{
	if (InGame())
	{
		clear();
		asm_init_seed();
		asm_init_zlist();
		if (ReadMemory(4, DATA_ADDR, 1, MODE_OFFSET + 4) == 2)
		{
			asm_destroy_model();
			asm_create_model();
			WriteMemory(0x80, 1, 0x0043a153);
		}
		RunAsm();
		WriteMemory(0x85, 1, 0x0043a153);
	}
}

void PVZ_1_0::SeeLeftZombies(bool(&zombies)[33])
{
	if (InGame())
	{
		DWORD Base = ReadMemory(4, DATA_ADDR, 1, OFFSET_1);
		DWORD WaveCount = ReadMemory(4, Base + 0x5564);
		DWORD CurWave = ReadMemory(4, Base + 0x557c);
		if (WaveCount == 0)
			return;
		DWORD LastAddr = Base + 0x6b4 + (WaveCount - 1) * 200;
		DWORD CurAddr = Base + 0x6b4 + CurWave * 200;
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

void PVZ_1_0::FlowerNPK(bool b)
{
	if (b)
	{
		WriteMemory(0, 1, 0x0051f017);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x1f8);
	}
	else
	{
		WriteMemory(-1, 1, 0x0051f017);
	}
}

void PVZ_1_0::Medicine(bool b)
{
	if (b)
	{
		WriteMemory(0, 1, 0x0051f110);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x1fc);
	}
	else
	{
		WriteMemory(-1, 1, 0x0051f110);
	}
}

void PVZ_1_0::Chocolate(bool b)
{
	if (b)
	{
		WriteMemory(0, 1, 0x0051ec38);
		WriteMemory(0, 1, 0x0051ecb8);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x228);
	}
	else
	{
		WriteMemory(-1, 1, 0x0051ec38);
		WriteMemory(-1, 1, 0x0051ecb8);
	}
}

void PVZ_1_0::TreeNPK(bool b)
{
	if (b)
	{
		WriteMemory(0, 1, 0x0042d463);
		WriteMemory(1020, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x230);
	}
	else
	{
		WriteMemory(-1, 1, 0x0042d463);
	}
}

void PVZ_1_0::TreeHeight(int h)
{
	if (GetMode() == 50)
	{
		WriteMemory(h - 1, 4, DATA_ADDR, 2, GARDON_OFFSET, 0xf4);
		clear();
		mov_exx(RegType::EDI, DATA_ADDR);
		call(0x0042d1f0);
		RunAsm();
	}
	else
		WriteMemory(h, 4, DATA_ADDR, 2, GARDON_OFFSET, 0xf4);
}

void PVZ_1_0::FlowerNum(int n)
{
	WriteMemory(n, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x350);
}

void PVZ_1_0::ChangeFlower(int fid, int type)
{
	WriteMemory(type, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x358 + fid * 0x58);
}

void PVZ_1_0::ChangeFSize(int fid, int size)
{
	WriteMemory(size, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x37c + fid * 0x58);
}

void PVZ_1_0::ChangeFDir(int fid, int dir)
{
	WriteMemory(dir, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x368 + fid * 0x58);
}

void PVZ_1_0::ChangeFPlace(int fid, int place)
{
	WriteMemory(place, 4, DATA_ADDR, 2, GARDON_OFFSET, 0x35c + fid * 0x58);
}

void PVZ_1_0::Patch()
{
	WriteMemory(0x23, 1, 0x00534d01);
	WriteMemory(0x23, 1, 0x005354ba);
}

std::string PVZ_1_0::getUserdataDir(char systemDir)
{
	return PVZ::getUserdataDir(systemDir);
}
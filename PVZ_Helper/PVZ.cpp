#include "stdafx.h"
#include "PVZ.h"
#include <Psapi.h>
#pragma comment (lib,"Psapi.lib")

void PVZ::SetBlocks(int row, int col, int type)
{
	int Rows = GetRowCount();
	int Cols = GetColCount();
	if (row == -1 && col == -1)
	{
		for (int i = 0; i < Rows; ++i)
		{
			for (int j = 0; j < Cols; ++j)
			{
				SetBlockType(i, j, type);
			}
		}
	}
	else if (row != -1 && col == -1)
	{
		for (int i = 0; i < Cols; ++i)
		{
			SetBlockType(row, i, type);
		}
	}
	else if (row == -1 && col != -1)
	{
		for (int i = 0; i < Rows; ++i)
		{
			SetBlockType(i, col, type);
		}
	}
	else
	{
		SetBlockType(row, col, type);
	}
}

int PVZ::GetRowCount()
{
	switch (GetMapType())
	{
	case 2:
	case 3:
		return 6;
	default:
		return 5;
	}
}

int PVZ::GetColCount()
{
	switch (GetMapType())
	{
	case 4:
	case 5:
		return 8;
	default:
		return 9;
	}
}

void PVZ::Planting(int row, int col, int type, bool copy)
{
	if (InGame())
	{
		int Rows = GetRowCount();
		int Cols = GetColCount();
		clear();
		int mode = GetMode();
		if (row == -1 && col == -1)
		{
			for (int i = 0; i < Rows; ++i)
			{
				for (int j = 0; j < Cols; ++j)
				{
					if (mode >= 61 && mode <= 70)
						asm_plant_paper(i, j, type);
					else
						asm_plant(i, j, type, copy);
					if (type == 47)
						++j;
				}
			}
		}
		else if (row != -1 && col == -1)
		{
			for (int i = 0; i < Cols; ++i)
			{
				if (mode >= 61 && mode <= 70)
					asm_plant_paper(row, i, type);
				else
					asm_plant(row, i, type, copy);
				if (type == 47)
					++i;
			}
		}
		else if (row == -1 && col != -1)
		{
			for (int i = 0; i < Rows; ++i)
			{
				if (mode >= 61 && mode <= 70)
					asm_plant_paper(i, col, type);
				else
					asm_plant(i, col, type, copy);
			}
		}
		else
		{
			if (mode >= 61 && mode <= 70)
				asm_plant_paper(row, col, type);
			else
				asm_plant(row, col, type, copy);
		}
		RunAsm();
	}
}

void PVZ::PutZombie(int row, int col, int type)
{
	if (InGame())
	{
		clear();
		if (type == 25)
		{
			asm_put_boss();
			RunAsm();
			return;
		}
		int Rows = GetRowCount();
		int Cols = GetColCount();
		if (row == -1 && col == -1)
		{
			for (int i = 0; i < Rows; ++i)
			{
				for (int j = 0; j < Cols; ++j)
				{
					asm_put_zombie(i, j, type);
				}
			}
		}
		else if (row != -1 && col == -1)
		{
			for (int i = 0; i < Cols; ++i)
			{
				asm_put_zombie(row, i, type);
			}
		}
		else if (row == -1 && col != -1)
		{
			for (int i = 0; i < Rows; ++i)
			{
				asm_put_zombie(i, col, type);
			}
		}
		else
		{
			asm_put_zombie(row, col, type);
		}
		RunAsm();
	}
}

void PVZ::PutLadder(int row, int col)
{
	if (InGame())
	{
		int Rows = GetRowCount();
		int Cols = GetColCount();
		clear();
		if (row == -1 && col == -1)
		{
			for (int i = 0; i < Rows; ++i)
			{
				for (int j = 0; j < Cols; ++j)
				{
					asm_put_ladder(i, j);
				}
			}
		}
		else if (row != -1 && col == -1)
		{
			for (int i = 0; i < Cols; ++i)
			{
				asm_put_ladder(row, i);
			}
		}
		else if (row == -1 && col != -1)
		{
			for (int i = 0; i < Rows; ++i)
			{
				asm_put_ladder(i, col);
			}
		}
		else
		{
			asm_put_ladder(row, col);
		}
		RunAsm();
	}
}

void PVZ::PutGrave(int row, int col)
{
	if (InGame())
	{
		int Rows = GetRowCount();
		int Cols = GetColCount();
		clear();
		if (row == -1 && col == -1)
		{
			for (int i = 0; i < Rows; ++i)
			{
				for (int j = 0; j < Cols; ++j)
				{
					asm_put_grave(i, j);
				}
			}
		}
		else if (row != -1 && col == -1)
		{
			for (int i = 0; i < Cols; ++i)
			{
				asm_put_grave(row, i);
			}
		}
		else if (row == -1 && col != -1)
		{
			for (int i = 0; i < Rows; ++i)
			{
				asm_put_grave(i, col);
			}
		}
		else
		{
			asm_put_grave(row, col);
		}
		RunAsm();
	}
}


void PVZ::AutoLadder()
{
	if (InGame())
	{
		bool HasLadder[6][9] = { 0 };
		clear();
		GetItems();
		for (auto& ri : vi)
		{
			if (ri.Type == 3)
			{
				HasLadder[ri.Row][ri.Col] = true;
			}
		}
		GetPlants();
		for (auto& rp : vp)
		{
			if (rp.Type == 30)
			{
				if (rp.Col != 0 && !HasLadder[rp.Row][rp.Col]
					&& GetBlockType(rp.Row, rp.Col) != 3)
				{
					asm_put_ladder(rp.Row, rp.Col);
					HasLadder[rp.Row][rp.Col] = true;
				}
			}
		}
		RunAsm();
	}
}

//以下功能继承实现
void PVZ::RunAsm()
{
}
void PVZ::GetPlants()
{
}
void PVZ::GetItems()
{
}
void PVZ::GetCars()
{
}
void PVZ::GetZombies()
{
}
int PVZ::GetMapType()
{
	return 0;
}
int PVZ::GetBlockType(int row, int col)/*1.草地2.裸地3.水池*/
{
	return 0;
}
void PVZ::SetBlockType(int row, int col, int type)/*1.草地2.裸地3.水池*/
{
}
int PVZ::GetRowType(int row)/*0.不出怪1.草地2.水路*/
{
	return 0;
}
void PVZ::SetRowType(int row, int type)/*0.不出怪1.草地2.水路*/
{
}
bool PVZ::InGame()
{
	return false;
}

int PVZ::GetMode()
{
	return 0;
}

void PVZ::SetSun(int sun)
{
}

void PVZ::SetMoney(int money)
{
}

void PVZ::IgnoreSun(bool b)
{
}

void PVZ::AutoPickup(bool b)
{
}

void PVZ::StartCar()
{
}

void PVZ::ResetCar()
{
}

void PVZ::asm_plant(int row, int col, int type, bool copy)
{
}

void PVZ::asm_plant_paper(int row, int col, int type)
{
}

void PVZ::NoPause(bool b)
{
}

void PVZ::asm_put_zombie(int row, int col, int type)
{
}

void PVZ::asm_put_boss()
{
}

void PVZ::asm_put_ladder(int row, int col)
{
}

void PVZ::asm_put_grave(int row, int col)
{
}

void PVZ::NoDelete(bool b)
{
}

void PVZ::NoSave(bool b)
{
}

void PVZ::Anywhere(bool b)
{
}

void PVZ::FastBelt(bool b)
{
}

void PVZ::ShowHide(bool b)
{
}

void PVZ::FullFog(bool b)
{
}

void PVZ::SeeFog(bool b)
{
}

void PVZ::SeeVase(bool b)
{
}

void PVZ::JmpLevel(int level)
{
}

void PVZ::MixMode(int mode, int level)
{
}

void PVZ::Unlock(bool b)
{
}

void PVZ::Win()
{
}

void PVZ::Lock(bool b, int mode)
{
}

void PVZ::Little(bool b)
{
}

void PVZ::Rain(bool b)
{
}

void PVZ::Storm(bool b)
{
}

void PVZ::Force(bool b)
{
}

void PVZ::LikeCol(bool b)
{
}

void PVZ::RunFast(bool b)
{
}

void PVZ::SetCardNum(int num)
{
}

void PVZ::SetCard(int cid, int type, bool copy)
{
}

void PVZ::FSBoom(bool)
{
}

void PVZ::FSSquash(bool)
{
}

void PVZ::FSMagnet(bool)
{
}

void PVZ::FSSputter(bool)
{
}

void PVZ::FSMouth(bool)
{
}

void PVZ::FSPrick(bool)
{
}

void PVZ::FSSpout(bool)
{
}

void PVZ::FSCat(bool)
{
}

void PVZ::CatTouch(bool)
{
}

void PVZ::InitExtraCode()
{
}

void PVZ::StartDoor(bool b)
{
}

void PVZ::StableDoor(bool b)
{
}

void PVZ::SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c)
{
}

void PVZ::CornNoWait(bool b)
{
}

void PVZ::MagnetNoWait(bool b)
{
}

void PVZ::PotatoNoWait(bool b)
{
}

void PVZ::MouthNoWait(bool b)
{
}

void PVZ::PStrong()
{
}

void PVZ::PNormal()
{
}

void PVZ::PWeak()
{
}

void PVZ::NoMark(bool b)
{
}

void PVZ::NoSleep(bool b)
{
}

void PVZ::StrongWind(bool b)
{
}

void PVZ::StrongBean(bool b)
{
}

void PVZ::Umbrella(bool b)
{
}

void PVZ::NoCD(bool b)
{
}

void PVZ::LockScoop(bool b)
{
}

void PVZ::AnyMove(bool b)
{
}

void PVZ::NoPurple(bool b)
{
}

void PVZ::LockButter(bool b)
{
}

void PVZ::LockCorn(bool b)
{
}

void PVZ::BoomIm()
{
}

void PVZ::BoomNormal()
{
}

void PVZ::BoomNever()
{
}

void PVZ::VisiableNo()
{
}

void PVZ::VisiableNormal()
{
}

void PVZ::VisiableYes()
{
}

void PVZ::ZStrong()
{
}

void PVZ::ZNormal()
{
}

void PVZ::ZWeak()
{
}

void PVZ::NoSlow(bool b)
{
}

void PVZ::NoButter(bool b)
{
}

void PVZ::No3zGroup(bool b)
{
}

void PVZ::NoIMP(bool b)
{
}

void PVZ::NoICE(bool b)
{
}

void PVZ::ZEatable(bool b)
{
}

void PVZ::StopZTimer(bool b)
{
}

void PVZ::StopZombies(bool b)
{
}

void PVZ::StopBoss(bool b)
{
}

void PVZ::BalloonBoom(bool b)
{
}

void PVZ::ZTimerSpeed(int value)
{
}

void PVZ::GatherZombie(bool b, float pos)
{
}

void PVZ::KillAllZombies()
{
}

void PVZ::ConfuseZombies()
{
}

void PVZ::NaturalSeed(int* zombies, int znum)
{
}
void PVZ::asm_destroy_model()
{
}

void PVZ::asm_create_model()
{
}
void PVZ::LimitSeed(int* zombies, int znum, bool thief, bool red)
{
}

void PVZ::asm_init_seed()
{
}

void PVZ::asm_init_zlist()
{
}

void PVZ::ResetSeed()
{
}

void PVZ::SeeLeftZombies(bool(&zombies)[33])
{
}

void PVZ::FlowerNPK(bool b)
{
}

void PVZ::Medicine(bool b)
{
}

void PVZ::Chocolate(bool b)
{
}

void PVZ::TreeNPK(bool b)
{
}

void PVZ::TreeHeight(int h)
{
}

void PVZ::FlowerNum(int n)
{
}

void PVZ::ChangeFlower(int fid, int type)
{
}

void PVZ::ChangeFSize(int fid, int size)
{
}

void PVZ::ChangeFDir(int fid, int dir)
{
}

void PVZ::ChangeFPlace(int fid, int place)
{
}

void PVZ::Patch()
{
}

std::string PVZ::getUserdataDir(char systemDir)
{
	std::string dir;
	dir += systemDir;
	dir += ":\\ProgramData\\PopCap Games\\PlantsVsZombies\\userdata";
	return dir;
}

std::string PVZ::getUserdataDirXP()
{
	char dir[256] = {0};
	GetModuleFileNameEx(GetHandle(), NULL, dir, 256);
	char* p = dir + strlen(dir);
	while (*--p != '\\' && p >= dir) {}
	if (p <= dir)
	{
		return "";
	}
	else
	{
		strcpy(p + 1, "userdata");
	}
	return dir;
}
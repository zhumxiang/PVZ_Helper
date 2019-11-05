#pragma once

#include "Process.h"
#include "Asm.h"
#include <vector>
#include <string>

#define _call(addr)	\
	call $ + 7}\
	_asm{_emit 0xeb}\
	_asm{_emit 6}\
	_asm{push addr}\
	_asm{ret

#define _jmp(addr)	\
	push addr}\
	_asm{ret

#define _j(flag, addr) \
	_emit flag}\
	_asm{_emit 6}\
	_asm{_jmp(addr)

#define GET_ADDR(f)	(unsigned char*)f
#define HEAD_OF(f)	(*GET_ADDR(f) == 0xeb ? 2 : 5)
#define SIZE_OF(f)	(*GET_ADDR(f) == 0xeb ? *(GET_ADDR(f)+1) : *(unsigned int*)(GET_ADDR(f)+1))

#define DECLARE_PUBLIC	\
	virtual bool InGame();\
	virtual int	 GetMode();\
	virtual void SetSun(int);\
	virtual void SetMoney(int);\
	virtual void IgnoreSun(bool);\
	virtual void AutoPickup(bool);\
	virtual void StartCar();\
	virtual void ResetCar();\
	virtual void NoPause(bool);\
	/*virtual void PutRake(int row, int col);*/\
	virtual void NoDelete(bool);\
	virtual void NoSave(bool);\
	virtual void Anywhere(bool);\
	virtual void FastBelt(bool);\
	virtual void ShowHide(bool);\
	virtual void FullFog(bool);\
	virtual void SeeFog(bool);\
	virtual void SeeVase(bool);\
	virtual void JmpLevel(int);\
	virtual void MixMode(int mode, int level = 0);\
	virtual void Unlock(bool);\
	virtual void Win();\
	virtual void Lock(bool,int = 0);\
	virtual void Little(bool);\
	virtual void Rain(bool);\
	virtual void Storm(bool);\
	virtual void Force(bool);\
	virtual void LikeCol(bool);\
	virtual void RunFast(bool);\
	virtual void SetCardNum(int);\
	virtual void SetCard(int cid, int type, bool copy = false);\
	virtual void FSBoom(bool);\
	virtual void FSSquash(bool);\
	virtual void FSMagnet(bool);\
	virtual void FSSputter(bool);\
	virtual void FSMouth(bool);\
	virtual void FSPrick(bool);\
	virtual void FSSpout(bool);\
	virtual void FSCat(bool);\
	virtual void CatTouch(bool);\
	virtual void InitExtraCode();\
	virtual void StartDoor(bool);\
	virtual void StableDoor(bool);\
	virtual void SetDoor(int b1r, int b1c, int b2r, int b2c, int w1r, int w1c, int w2r, int w2c);\
	virtual void CornNoWait(bool);\
	virtual void MagnetNoWait(bool);\
	virtual void PotatoNoWait(bool);\
	virtual void MouthNoWait(bool);\
	virtual void PStrong();\
	virtual void PNormal();\
	virtual void PWeak();\
	virtual void NoMark(bool);\
	virtual void NoSleep(bool);\
	virtual void StrongWind(bool);\
	virtual void StrongBean(bool);\
	virtual void Umbrella(bool);\
	virtual void NoCD(bool);\
	virtual void LockScoop(bool);\
	virtual void AnyMove(bool);\
	virtual void NoPurple(bool);\
	virtual void LockButter(bool);\
	virtual void LockCorn(bool);\
	virtual void BoomIm();\
	virtual void BoomNormal();\
	virtual void BoomNever();\
	virtual void VisiableNo();\
	virtual void VisiableNormal();\
	virtual void VisiableYes();\
	virtual void ZStrong();\
	virtual void ZNormal();\
	virtual void ZWeak();\
	virtual void NoSlow(bool);\
	virtual void NoButter(bool);\
	virtual void No3zGroup(bool);\
	virtual void NoIMP(bool);\
	virtual void NoICE(bool);\
	virtual void ZEatable(bool);\
	virtual void StopZTimer(bool);\
	virtual void StopZombies(bool);\
	virtual void StopBoss(bool);\
	virtual void BalloonBoom(bool);\
	virtual void ZTimerSpeed(int);\
	virtual void GatherZombie(bool, float = 0.0f);\
	virtual void KillAllZombies();\
	virtual void ConfuseZombies();\
	virtual void NaturalSeed(int* zombies, int znum);\
	virtual void LimitSeed(int* zombies, int znum, bool thief, bool red);\
	virtual void ResetSeed();\
	virtual void SeeLeftZombies(bool(&zombies)[33]);\
	virtual void FlowerNPK(bool);\
	virtual void Medicine(bool);\
	virtual void Chocolate(bool);\
	virtual void TreeNPK(bool);\
	virtual void TreeHeight(int);\
	virtual void FlowerNum(int);\
	virtual void ChangeFlower(int fid, int type);\
	virtual void ChangeFSize(int fid, int size);\
	virtual void ChangeFDir(int fid, int dir);\
	virtual void ChangeFPlace(int fid, int place);\
	virtual void Patch();\
	virtual void RunAsm();\
	virtual void GetPlants();\
	virtual void GetCars();\
	virtual void GetItems();\
	virtual void GetZombies();\
	virtual int GetMapType();\
	virtual int GetBlockType(int row, int col);/*1.草地2.裸地3.水池*/\
	virtual void SetBlockType(int row, int col, int type);/*1.草地2.裸地3.水池*/\
	virtual int GetRowType(int row);/*0.不出怪1.草地2.水路*/\
	virtual void SetRowType(int row, int type);/*0.不出怪1.草地2.水路*/\
	virtual std::string getUserdataDir(char systemDir);\


#define DECLARE_PRIVATE	\
	virtual void asm_plant(int row, int col, int type, bool copy);\
	virtual void asm_plant_paper(int row, int col, int type);\
	virtual void asm_put_zombie(int row, int col, int type);\
	virtual void asm_put_boss();\
	virtual void asm_put_ladder(int row, int col);\
	virtual void asm_put_grave(int row, int col);\
	virtual void asm_create_model();\
	virtual void asm_destroy_model();\
	virtual void asm_init_seed();\
	virtual void asm_init_zlist();\

enum
{
	EXCODE_VAR_FS_MOUTH,
	EXCODE_VAR_FS_SPOUT,
	EXCODE_VAR_FS_PRICK,
	EXCODE_VAR_FS_CAT,

	EXCODE_VAR_COUNT
};

struct Plant
{
	DWORD addr;
	int Row;
	int Col;
	int Type;
	bool Sleeping;
};

struct Item
{
	DWORD addr;
	int Row;
	int Col;
	int Type;
};

struct Car
{
	DWORD addr;
	int Row;
};

struct Zombie
{
	DWORD addr;
	int Row;
	int Type;
};


class PVZ : protected Process, protected Asm
{
public:
	PVZ(const Process& p) : Process(p), Asm(4){}
	void SetBlocks(int row, int col, int type);
	int GetRowCount();
	int GetColCount();
	void Planting(int row, int col, int type, bool copy);
	void PutZombie(int row, int col, int type);
	void PutLadder(int row, int col);
	void PutGrave(int row, int col);
	void AutoLadder();
	std::string getUserdataDirXP();
	DECLARE_PUBLIC
protected:
	std::vector<Plant> vp;
	std::vector<Item> vi;
	std::vector<Car> vc;
	std::vector<Zombie> vz;

	struct ExtraCodeAddr
	{
		DWORD ZTimer;
		DWORD Thread;
	}eCode;
	DWORD eCodeStart;
private:
	DECLARE_PRIVATE
};


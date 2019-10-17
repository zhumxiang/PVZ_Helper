// PVZ_Helper.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "PVZ_Helper.h"
#include "PVZ_1_0.h"
#include "PVZ_1_2_EN_1073.h"
#include "PVZ_1_2_CN_REPAIR.h"
#include "PVZ_1_2_CN_ENDLESS.h"
#include "stdio.h"
#include "commctrl.h"
#include "DllFromMem.h"
#include <memory>


#define WM_INIT		WM_USER + 1

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

std::shared_ptr<PVZ> pvz;

#define INIT_LIST(hwnd,IDC,_List) \
for (int i = 0; i < sizeof(_List) / sizeof(*_List); ++i)\
{\
	SendDlgItemMessage(hwnd, IDC, CB_ADDSTRING, 0, LPARAM(_List[i])); \
}\
	SendDlgItemMessage(hwnd, IDC, CB_SETCURSEL, 0, 0)


const char* pList[] = { "豌豆射手", "向日葵", "樱桃炸弹", "坚果墙", "土豆雷",
"寒冰射手", "大嘴花", "双发射手", "小喷菇", "阳光菇", "大喷菇", "墓碑吞噬者",
"魅惑菇", "胆小菇", "寒冰菇", "毁灭菇", "睡莲", "窝瓜", "三线射手", "缠绕海藻",
"火爆辣椒", "地刺", "火炬树桩", "高坚果", "海蘑菇", "路灯花", "仙人掌",
"三叶草", "裂荚射手", "杨桃", "南瓜头", "磁力菇", "卷心菜投手", "花盆",
"玉米投手", "咖啡豆", "大蒜", "叶子保护伞", "金盏花", "西瓜投手", "机枪射手",
"双子向日葵", "忧郁菇", "香蒲", "冰瓜", "吸金磁", "地刺王", "玉米加农炮",
"左向双发射手", "爆炸坚果", "巨大坚果" };

const char* zList[] = { "普通", "旗帜", "路障", "撑杆", "铁桶", "读报",
"铁栅门", "橄榄球", "舞王", "伴舞", "鸭子", "潜水", "冰车", "雪橇", "海豚",
"小丑", "气球", "矿工", "跳跳", "雪人", "蹦极", "扶梯", "篮球", "白眼", "小鬼",
"僵博", "豌豆", "坚果", "辣椒", "机枪", "窝瓜", "高坚果", "红眼" };

const char* bList[] = { "草地", "裸地", "水池" };

const char* nList[] = { "1", "2", "3", "4", "5", "6" };

const char* rList[] = { "不出怪", "陆路", "水路" };

const char* mList[] = { "冒险模式", "白天简单", "夜晚简单", "泳池简单",
"浓雾简单", "屋顶简单", "白天困难", "夜晚困难", "泳池困难", "浓雾困难",
"屋顶困难", "白天无尽", "夜晚无尽", "泳池无尽", "浓雾无尽", "屋顶无尽",
"植物僵尸", "坚果保龄球", "老虎机", "雨中种植物", "宝石迷阵", "隐形食脑者",
"看星星", "僵尸水族馆", "宝石转转看", "小僵尸大麻烦", "保护传送门", "柱子一样",
"雪橇区", "僵尸快跑", "锤僵尸", "谁笑到最后", "植物僵尸2", "坚果保龄球2",
"跳跳舞会", "僵王复仇", "坚果艺术", "晴天", "无草皮之地", "重要时间",
"种太阳花", "空袭", "冰冻关卡", "超乎寻常压力", "坟墓模式", "把它挖出来",
"暴风雨之夜", "蹦极闪电战", "松鼠", "花瓶无尽", "我是僵尸无尽" };

const char* izeList[] = { "豌豆阵", "大嘴地雷阵", "地刺杨桃阵", "大喷磁铁阵", "胆小阵",
"大杂烩阵1", "大杂烩阵2", "大杂烩阵3" };

const char* cnList[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

const char* czList[] = { "普通", "路障", "撑杆", "铁桶", "扶梯", "矿工", "蹦极", "橄榄球", "气球", "铁栅门", "冰车", "跳跳", "舞王", "白眼", "小鬼" };

const char* fList[] = { "豌豆射手", "向日葵", "樱桃炸弹", "坚果墙", "土豆雷",
"寒冰射手", "大嘴花", "双发射手", "小喷菇", "阳光菇", "大喷菇", "墓碑吞噬者",
"魅惑菇", "胆小菇", "寒冰菇", "毁灭菇", "睡莲", "窝瓜", "三线射手", "缠绕海藻",
"火爆辣椒", "地刺", "火炬树桩", "高坚果", "海蘑菇", "路灯花", "仙人掌",
"三叶草", "裂荚射手", "杨桃", "南瓜头", "磁力菇", "卷心菜投手", "花盆",
"玉米投手", "咖啡豆", "大蒜", "叶子保护伞", "金盏花", "西瓜投手", "机枪射手",
"双子向日葵", "忧郁菇", "香蒲", "冰瓜", "吸金磁", "地刺王", "玉米加农炮", "模仿者",
"爆炸坚果", "巨大坚果" };

const char* fsList[] = { "幼苗", "小花", "中花", "大花" };

const char* fpList[] = { "花园", "蘑菇园", "小车", "水族馆" };

enum{
	Tab_General,
	Tab_Pass,
	Tab_Plants,
	Tab_Zombies,
	Tab_Card,
	Tab_Seed,
	Tab_FullScreen,
	Tab_Garden,

	Tab_Num
};

class Tabs
{
public:
	void init(HWND hWnd
		, const int ID[]
		, const DLGPROC Procs[]
		, const char* Name[]
		, const int count)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		TCITEM tie;
		tie.mask = TCIF_TEXT | TCIF_IMAGE;
		tie.iImage = -1;
		for (int i = 0; i < count; ++i)
		{
			HWND hc = CreateDialog(hInst, MAKEINTRESOURCE(ID[i]), hWnd, Procs[i]);
			m_child.push_back(hc);
			MoveWindow(m_child[i], 1, 39, rect.right - rect.left - 2, rect.bottom - rect.top - 40, FALSE);
			tie.pszText = const_cast<char*>(Name[i]);
			SendMessage(hWnd, TCM_INSERTITEM, i, LPARAM(&tie));
			ShowWindow(hc, SW_HIDE);
		}
		m_curWnd = 0;
	}
	void Show(const int n)
	{
		ShowWindow(m_child[m_curWnd], SW_HIDE);
		ShowWindow(m_child[m_curWnd = n], SW_SHOW);
	}
	HWND getChild(const int n){ return m_child[n]; }
private:
	std::vector<HWND> m_child;
	int m_curWnd;
};

INT_PTR CALLBACK GeneralProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK PassProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK PlantsProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ZombiesProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK CardProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK SeedProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK FullScreenProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK GardenProc(HWND, UINT, WPARAM, LPARAM);

const int Tab_ID[Tab_Num] = { IDD_GENERAL, IDD_PASS, IDD_PLANTS, IDD_ZOMBIES, IDD_CARD, IDD_SEED, IDD_FULLSCREEN, IDD_GARDEN };
const DLGPROC Tab_Procs[Tab_Num] = { GeneralProc, PassProc, PlantsProc, ZombiesProc, CardProc, SeedProc, FullScreenProc, GardenProc };
const char* Tab_Name[Tab_Num] = { "常规", "穿越", "植物", "僵尸", "卡槽", "出怪", "全屏", "花园" };
// 此代码模块中包含的函数的前向声明: 
BOOL				InitInstance(HINSTANCE, int);
INT_PTR CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				LoadSkin();
std::shared_ptr<PVZ> judgeProcess(Process& p, std::string& err);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PVZ_HELPER, szWindowClass, MAX_LOADSTRING);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PVZ_HELPER));
	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return (int)msg.wParam;
}

Tabs htabs;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中
	LoadSkin();

	hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, WndProc);
	SetWindowPos(hWnd, NULL, 500, 100, 345, 475, SWP_NOREPOSITION);
	if (!hWnd)
	{
		return FALSE;
	}

	char winTitle[256];
	SendMessage(hWnd, WM_GETTEXT, sizeof(winTitle), LPARAM(winTitle));
	strcat(winTitle, "(" VERSION_NUM ")");
	SendMessage(hWnd, WM_SETTEXT, 0, LPARAM(winTitle));

	htabs.init(GetDlgItem(hWnd, IDC_TAB1)
		,Tab_ID
		,Tab_Procs
		,Tab_Name
		,Tab_Num);
	SetDlgItemText(htabs.getChild(Tab_General), IDC_SUNNUM, "9990");
	SetDlgItemText(htabs.getChild(Tab_General), IDC_MONEYNUM, "99999");
	SetDlgItemText(htabs.getChild(Tab_Pass), IDC_B1R, "1");
	SetDlgItemText(htabs.getChild(Tab_Pass), IDC_B1C, "3");
	SetDlgItemText(htabs.getChild(Tab_Pass), IDC_B2R, "2");
	SetDlgItemText(htabs.getChild(Tab_Pass), IDC_B2C, "10");
	SetDlgItemText(htabs.getChild(Tab_Pass), IDC_W1R, "4");
	SetDlgItemText(htabs.getChild(Tab_Pass), IDC_W1C, "10");
	SetDlgItemText(htabs.getChild(Tab_Pass), IDC_W2R, "5");
	SetDlgItemText(htabs.getChild(Tab_Pass), IDC_W2C, "3");

	INIT_LIST(htabs.getChild(Tab_General), IDC_PLIST, pList);
	INIT_LIST(htabs.getChild(Tab_General), IDC_ZLIST, zList);
	SendDlgItemMessage(hWnd, IDC_ZLIST, CB_ADDSTRING, 0, LPARAM("无梯僵尸"));
	INIT_LIST(htabs.getChild(Tab_General), IDC_BLIST, bList);
	INIT_LIST(htabs.getChild(Tab_General), IDC_NLIST, nList);
	INIT_LIST(htabs.getChild(Tab_General), IDC_RLIST, rList);
	INIT_LIST(htabs.getChild(Tab_Pass), IDC_MLIST, mList);
	INIT_LIST(htabs.getChild(Tab_Pass), IDC_IZELIST, izeList);
	INIT_LIST(htabs.getChild(Tab_Card), IDC_CNLIST, cnList);
	INIT_LIST(htabs.getChild(Tab_Card), IDC_CIDLIST, cnList);
	INIT_LIST(htabs.getChild(Tab_Card), IDC_CPLIST, pList);
	INIT_LIST(htabs.getChild(Tab_Card), IDC_CZLIST, czList);
	INIT_LIST(htabs.getChild(Tab_Seed), IDC_SLIST, zList);
	INIT_LIST(htabs.getChild(Tab_Garden), IDC_FLOWER_LIST, fList);
	INIT_LIST(htabs.getChild(Tab_Garden), IDC_FLOWER_SIZE, fsList);
	INIT_LIST(htabs.getChild(Tab_Garden), IDC_PLACE_LIST, fpList);

	CheckDlgButton(htabs.getChild(Tab_Plants), IDC_PNORMAL, BST_CHECKED);
	CheckDlgButton(htabs.getChild(Tab_Zombies), IDC_BOOM_NORMAL, BST_CHECKED);
	CheckDlgButton(htabs.getChild(Tab_Zombies), IDC_VISIABLE_NORMAL, BST_CHECKED);
	CheckDlgButton(htabs.getChild(Tab_Zombies), IDC_ZNORMAL, BST_CHECKED);
	CheckDlgButton(htabs.getChild(Tab_Seed), IDC_NATURAL, BST_CHECKED);

	SendDlgItemMessage(htabs.getChild(Tab_Zombies), IDC_SLIDER_TIMER, TBM_SETRANGE, TRUE, MAKELONG(0, 350));
	SendDlgItemMessage(htabs.getChild(Tab_Zombies), IDC_SLIDER_GATHER, TBM_SETRANGE, TRUE, MAKELONG(0, 800));
	SendDlgItemMessage(htabs.getChild(Tab_Zombies), IDC_SLIDER_GATHER, TBM_SETPOS, TRUE, 800);

	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)));
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	htabs.Show(Tab_General);
	SendMessage(hWnd, WM_INIT, 0, 0);
	return TRUE;
}

void LoadSkin()
{
	static CDllFromMem dll;
	dll.LoadLibraryFromRs(hInst, "File", MAKEINTRESOURCE(IDR_SKINDLL));
	auto pLoadSkin = (void(__stdcall*)(void*, int, char*, int, int, int))dll.GetProcAddressFromRs("SkinH_AttachRes");
	HRSRC hres = FindResource(NULL, MAKEINTRESOURCE(IDR_SKIN), "File");
	size_t skinsize = SizeofResource(NULL, hres);
	LPVOID pSkinBuffer = LockResource(LoadResource(NULL, hres));
	pLoadSkin(pSkinBuffer, skinsize, "", 0, 0, 0);
}

static void InitGame(HWND hWnd)
{
	std::string err;
	Process::LookupProcess([&](const char* name, DWORD pid)
	{
		std::string strName = name;
		if (strName != "popcapgame1.exe"
			&& strName.find("PlantsVsZombies") == std::string::npos
			&& strName.find("植物大战僵尸") == std::string::npos)
		{
			return false;
		}
		Process tmp;
		tmp.OpenByPid(pid);
		pvz = judgeProcess(tmp, err);
		if (pvz != nullptr)
		{
			return true;
		}
		return false;
	});
	if (pvz == nullptr)
	{
		pvz = std::make_shared<PVZ>(Process());
		if (err != "")
		{
			MessageBox(hWnd, err.c_str(), "错误", MB_OK | MB_ICONERROR);
		}
		else
		{
			MessageBox(hWnd, "找不到游戏", "错误", MB_OK);
		}
	}
	else
	{
		pvz->Patch();
		pvz->InitExtraCode();
	}
}

std::shared_ptr<PVZ> judgeProcess(Process& process, std::string& err)
{
	if (!process.IsValid())
	{
		return nullptr;
	}
	if (process.ReadMemory(4, 0x0042748e) == 0xff563de8)
	{
		return std::make_shared<PVZ_1_0>(process);
	}
	else if (process.ReadMemory(4, 0x00429f3e) == 0xff594de8)
	{
		return std::make_shared<PVZ_1_2_EN_1073>(process);
	}
	else if (process.ReadMemory(4, 0x0042ec0e) == 0xff566de8)
	{
		return std::make_shared<PVZ_1_2_CN_REPAIR>(process);
	}
	else if (process.ReadMemory(4, 0x0043271e) == 0xff5fbde8)
	{
		return std::make_shared<PVZ_1_2_CN_ENDLESS>(process);
	}
	else
	{
		if (process.ReadMemory(4, 0x0044002e) == 0xff531de8)
		{
			err = "这个版本的游戏会崩溃，去植物大战僵尸贴吧下个正常版本的游戏吧";
		}
		return nullptr;
	}
}

INT_PTR CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDCANCEL:
			DestroyWindow(hWnd);
			break;
		case IDC_RESTART:
			SendMessage(hWnd, WM_INIT, 0, 0);
			break;
		default:
			return FALSE;
		}
		break;
	case WM_NOTIFY:
		if (wParam == IDC_TAB1)
		{
			if (LPNMHDR(lParam)->code == TCN_SELCHANGE)
			{
				htabs.Show(SendDlgItemMessage(hWnd, IDC_TAB1, TCM_GETCURSEL, 0, 0));
			}
		}
		break;
	case WM_INIT:
		InitGame(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK GeneralProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// 分析菜单选择: 
		switch (wmId)
		{
		case IDC_SUN:/*修改阳光*/
			pvz->SetSun(GetDlgItemInt(hWnd, IDC_SUNNUM, NULL, TRUE));
			break;
		case IDC_MONEY:/*修改金钱*/
			pvz->SetMoney(GetDlgItemInt(hWnd, IDC_MONEYNUM, NULL, TRUE));
			break;
		case IDC_NOSUN:/*无视阳光*/
			pvz->IgnoreSun(IsDlgButtonChecked(hWnd, IDC_NOSUN));
			break;
		case IDC_COLLECT:/*自动收集*/
			pvz->AutoPickup(IsDlgButtonChecked(hWnd, IDC_COLLECT));
			break;
		case IDC_START_CAR:/*启动除草机*/
			pvz->StartCar();
			break;
		case IDC_RESET_CAR:/*恢复除草机*/
			pvz->ResetCar();
			break;
		case IDC_PLANTING:/*种植物*/
		{
			int row = GetDlgItemInt(hWnd, IDC_ROW, NULL, TRUE) - 1;
			int col = GetDlgItemInt(hWnd, IDC_COL, NULL, TRUE) - 1;
			int type = SendDlgItemMessage(hWnd, IDC_PLIST, CB_GETCURSEL, 0, 0);
			if (type == 48)
				type = 52;
			pvz->Planting(row, col, type, IsDlgButtonChecked(hWnd, IDC_COPY));
			break;
		}
		case IDC_NOPAUSE:/*取消暂停*/
			pvz->NoPause(IsDlgButtonChecked(hWnd, IDC_NOPAUSE));
			break;
		case IDC_PUT_ZOMBIE:/*放僵尸*/
		{
			int row = GetDlgItemInt(hWnd, IDC_ROW, NULL, TRUE) - 1;
			int col = GetDlgItemInt(hWnd, IDC_COL, NULL, TRUE) - 1;
			int type = SendDlgItemMessage(hWnd, IDC_ZLIST, CB_GETCURSEL, 0, 0);
			if (type == 33)
				type = 36;
			pvz->PutZombie(row, col, type);
			break;
		}
		case IDC_PUT_LADDER:/*放梯子*/
		{
			int row = GetDlgItemInt(hWnd, IDC_ROW, NULL, TRUE) - 1;
			int col = GetDlgItemInt(hWnd, IDC_COL, NULL, TRUE) - 1;
			pvz->PutLadder(row, col);
			break;
		}
		case IDC_PUT_GRAVE:/*放墓碑*/
		{
			int row = GetDlgItemInt(hWnd, IDC_ROW, NULL, TRUE) - 1;
			int col = GetDlgItemInt(hWnd, IDC_COL, NULL, TRUE) - 1;
			pvz->PutGrave(row, col);
			break;
		}
		case IDC_AUTO_LADDER:/*智能搭梯*/
			pvz->AutoLadder();
			break;
// 		case IDC_PUT_RAKE:/*放钉耙*/
// 		{
// 			int row = GetDlgItemInt(hWnd, IDC_ROW, NULL, TRUE) - 1;
// 			int col = GetDlgItemInt(hWnd, IDC_COL, NULL, TRUE) - 1;
// 			pvz->PutRake(row, col);
// 			break;
// 		}
		case IDC_BLOCK:/*改土地块*/
			{
				int row = GetDlgItemInt(hWnd, IDC_ROW, NULL, TRUE) - 1;
				int col = GetDlgItemInt(hWnd, IDC_COL, NULL, TRUE) - 1;
				int type = SendDlgItemMessage(hWnd, IDC_BLIST, CB_GETCURSEL, 0, 0) + 1;
				pvz->SetBlocks(row, col, type);
			}
			break;
		case IDC_ROWTYPE:/*改行判定*/
			{
				int row = SendDlgItemMessage(hWnd, IDC_NLIST, CB_GETCURSEL, 0, 0);
				int type = SendDlgItemMessage(hWnd, IDC_RLIST, CB_GETCURSEL, 0, 0);
				pvz->SetRowType(row, type);
			}
			break;
		case IDC_NO_DELETE:/*禁止删档*/
			pvz->NoDelete(IsDlgButtonChecked(hWnd, IDC_NO_DELETE));
			break;
		case IDC_NO_SAVE:/*禁止存档*/
			pvz->NoSave(IsDlgButtonChecked(hWnd, IDC_NO_SAVE));
			break;
		case IDC_SAVE_DIR:/*存档目录*/
		{
			OSVERSIONINFO osinfo;
			osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx(&osinfo);
			std::string DocDir;
			if (osinfo.dwMajorVersion >= 6)
			{
				char RootDir[20];
				GetEnvironmentVariable("windir", RootDir, 20);
				DocDir = pvz->getUserdataDir(RootDir[0]);
			}
			else
			{
				DocDir = pvz->getUserdataDirXP();
			}
			static char cmd[80];
			sprintf(cmd, "explorer \"%s\"", DocDir.c_str());
			WinExec(cmd, SW_SHOW);
			break;
		}
		case IDC_ANYWHERE:/*自由放置*/
			pvz->Anywhere(IsDlgButtonChecked(hWnd, IDC_ANYWHERE));
			break;
		case IDC_FAST_BELT:/*传送带无延迟*/
			pvz->FastBelt(IsDlgButtonChecked(hWnd, IDC_FAST_BELT));
			break;
		case IDC_SHOW_HIDE:/*显隐藏小游戏*/
			pvz->ShowHide(IsDlgButtonChecked(hWnd, IDC_SHOW_HIDE));
			break;
		case IDC_FULL_FOG:/*全屏浓雾*/
			pvz->FullFog(IsDlgButtonChecked(hWnd, IDC_FULL_FOG));
			break;
		case IDC_SEE_FOG:/*浓雾透视*/
			pvz->SeeFog(IsDlgButtonChecked(hWnd, IDC_SEE_FOG));
			break;
		case IDC_SEE_VASE:/*花瓶透视*/
			pvz->SeeVase(IsDlgButtonChecked(hWnd, IDC_SEE_VASE));
			break;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK PassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDC_JMP_LV:/*生存解谜跳关*/
			pvz->JmpLevel(GetDlgItemInt(hWnd, IDC_LV_NUM, NULL, TRUE));
			break;
		case IDC_MIX:/*混乱*/
		{
			int lv = GetDlgItemInt(hWnd, IDC_LV2, NULL, TRUE);
			if (lv == 0)
				lv = 1;
			int mode = SendDlgItemMessage(hWnd, IDC_MLIST, CB_GETCURSEL, 0, 0);
			if (mode > 42)
				++mode;
			if (mode == 50)
				mode = 60;
			if (mode == 51)
				mode = 70;
			pvz->MixMode(mode, lv);
			break;
		}
		case IDC_UNLOCK:/*全模式解锁*/
			pvz->Unlock(IsDlgButtonChecked(hWnd, IDC_UNLOCK));
			break;
		case IDC_WIN:/*直接过关*/
			pvz->Win();
			break;
		case IDC_LOCK:/*锁定阵型*/
			if (IsDlgButtonChecked(hWnd, IDC_LOCK))
			{
				EnableWindow(GetDlgItem(hWnd, IDC_IZELIST), FALSE);
				pvz->Lock(true, SendDlgItemMessage(hWnd, IDC_IZELIST, CB_GETCURSEL, 0, 0));
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_IZELIST), TRUE);
				pvz->Lock(false);
			}
			break;
		case IDC_LITTLE:/*小僵尸*/
			pvz->Little(IsDlgButtonChecked(hWnd, IDC_LITTLE));
			break;
		case IDC_RAIN:/*下雨*/
			pvz->Rain(IsDlgButtonChecked(hWnd, IDC_RAIN));
			break;
		case IDC_STORM:/*暴风雨*/
			pvz->Storm(IsDlgButtonChecked(hWnd, IDC_STORM));
			break;
		case IDC_FORCE:/*引力*/
			pvz->Force(IsDlgButtonChecked(hWnd, IDC_FORCE));
			break;
		case IDC_LIKE_COL:/*像柱子一样*/
			pvz->LikeCol(IsDlgButtonChecked(hWnd, IDC_LIKE_COL));
			break;
		case IDC_RUN_FAST:/*僵尸快跑*/
			pvz->RunFast(IsDlgButtonChecked(hWnd, IDC_RUN_FAST));
			break;
		case IDC_START_DOOR:/*启动传送门*/
			pvz->StartDoor(IsDlgButtonChecked(hWnd, IDC_START_DOOR));
			break;
		case IDC_STABLE_DOOR:/*固定传送门*/
			pvz->StableDoor(IsDlgButtonChecked(hWnd, IDC_STABLE_DOOR));
			break;
		case IDC_SET_DOOR:/*生成传送门*/
			pvz->SetDoor(
				GetDlgItemInt(hWnd, IDC_B1R, NULL, TRUE) - 1,
				GetDlgItemInt(hWnd, IDC_B1C, NULL, TRUE) - 1,
				GetDlgItemInt(hWnd, IDC_B2R, NULL, TRUE) - 1,
				GetDlgItemInt(hWnd, IDC_B2C, NULL, TRUE) - 1,
				GetDlgItemInt(hWnd, IDC_W1R, NULL, TRUE) - 1,
				GetDlgItemInt(hWnd, IDC_W1C, NULL, TRUE) - 1,
				GetDlgItemInt(hWnd, IDC_W2R, NULL, TRUE) - 1,
				GetDlgItemInt(hWnd, IDC_W2C, NULL, TRUE) - 1
				);
			break;
		default:
			return FALSE;
		}
		break;
	case WM_NOTIFY:
		if (wParam == IDC_MLIST)
		{
			if (LPNMHDR(lParam)->code == TCN_SELCHANGE)
			{
				if (SendDlgItemMessage(hWnd, IDC_MLIST, CB_GETCURSEL, 0, 0) == 0)
				{
					EnableWindow(GetDlgItem(hWnd, IDC_LV2), TRUE);
				}
				else
				{
					EnableWindow(GetDlgItem(hWnd, IDC_LV2), FALSE);
				}
			}
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK PlantsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDC_CORN_NO_WAIT:/*玉米炮免等待*/
			pvz->CornNoWait(IsDlgButtonChecked(hWnd, IDC_CORN_NO_WAIT));
			break;
		case IDC_MAGNET_NO_WAIT:/*磁力菇免等待*/
			pvz->MagnetNoWait(IsDlgButtonChecked(hWnd, IDC_MAGNET_NO_WAIT));
			break;
		case IDC_POTATO_NO_WAIT:/*土豆雷免等待*/
			pvz->PotatoNoWait(IsDlgButtonChecked(hWnd, IDC_POTATO_NO_WAIT));
			break;
		case IDC_MOUTH_NO_WAIT:/*大嘴花免等待*/
			pvz->MouthNoWait(IsDlgButtonChecked(hWnd, IDC_MOUTH_NO_WAIT));
			break;
		case IDC_PSTRONG:/*无敌*/
			pvz->PStrong();
			break;
		case IDC_PNORMAL:/*正常*/
			pvz->PNormal();
			break;
		case IDC_PWEAK:/*虚弱*/
			pvz->PWeak();
			break;
		case IDC_NO_MARK:/*毁灭菇无痕*/
			pvz->NoMark(IsDlgButtonChecked(hWnd, IDC_NO_MARK));
			break;
		case IDC_NO_SLEEP:/*蘑菇免唤醒*/
			pvz->NoSleep(IsDlgButtonChecked(hWnd, IDC_NO_SLEEP));
			break;
		case IDC_STRONG_WIND:/*强力三叶草*/
			pvz->StrongWind(IsDlgButtonChecked(hWnd, IDC_STRONG_WIND));
			break;
		case IDC_STRONG_BEAN:/*穿甲豌豆*/
			pvz->StrongBean(IsDlgButtonChecked(hWnd, IDC_STRONG_BEAN));
			break;
		case IDC_UMBRELLA:/*保护伞*/
			pvz->Umbrella(IsDlgButtonChecked(hWnd, IDC_UMBRELLA));
			break;
		case IDC_NO_CD:/*无冷却*/
			pvz->NoCD(IsDlgButtonChecked(hWnd, IDC_NO_CD));
			break;
		case IDC_LOCK_SCOOP:/*铲子锁定*/
			pvz->LockScoop(IsDlgButtonChecked(hWnd, IDC_LOCK_SCOOP));
			break;
		case IDC_ANY_MOVE:/*宝石随心移*/
			pvz->AnyMove(IsDlgButtonChecked(hWnd, IDC_ANY_MOVE));
			break;
		case IDC_NO_PURPLE:/*紫卡无限制*/
			pvz->NoPurple(IsDlgButtonChecked(hWnd, IDC_NO_PURPLE));
			break;
		case IDC_LOCK_BUTTER:/*锁定黄油*/
			CheckDlgButton(hWnd, IDC_LOCK_CORN, BST_UNCHECKED);
			pvz->LockButter(IsDlgButtonChecked(hWnd, IDC_LOCK_BUTTER));
			break;
		case IDC_LOCK_CORN:/*锁定玉米*/
			CheckDlgButton(hWnd, IDC_LOCK_BUTTER, BST_UNCHECKED);
			pvz->LockCorn(IsDlgButtonChecked(hWnd, IDC_LOCK_CORN));
			break;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK ZombiesProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDC_BOOM_IM:/*立刻爆炸*/
			pvz->BoomIm();
			break;
		case IDC_BOOM_NORMAL:
			pvz->BoomNormal();
			break;
		case IDC_BOOM_NEVER:
			pvz->BoomNever();
			break;
		case IDC_VISIABLE_NO:/*隐形*/
			pvz->VisiableNo();
			break;
		case IDC_VISIABLE_NORMAL:
			pvz->VisiableNormal();
			break;
		case IDC_VISIABLE_YES:
			pvz->VisiableYes();
			break;
		case IDC_ZSTRONG:/*无敌*/
			pvz->ZStrong();
			break;
		case IDC_ZNORMAL:
			pvz->ZNormal();
			break;
		case IDC_ZWEAK:
			pvz->ZWeak();
			break;
		case IDC_NO_SLOW:/*减速抗性*/
			pvz->NoSlow(IsDlgButtonChecked(hWnd, IDC_NO_SLOW));
			break;
		case IDC_NO_BUTTER:/*黄油*/
			pvz->NoButter(IsDlgButtonChecked(hWnd, IDC_NO_BUTTER));
			break;
		case IDC_NO3Z_GROUP:/*屏蔽3人组*/
			pvz->No3zGroup(IsDlgButtonChecked(hWnd, IDC_NO3Z_GROUP));
			break;
		case IDC_NO_IMP:/*不扔小鬼*/
			pvz->NoIMP(IsDlgButtonChecked(hWnd, IDC_NO_IMP));
			break;
		case IDC_NO_ICE:/*冰车无痕*/
			pvz->NoICE(IsDlgButtonChecked(hWnd, IDC_NO_ICE));
			break;
		case IDC_EATABLE:/*巨人可被吃*/
			pvz->ZEatable(IsDlgButtonChecked(hWnd, IDC_EATABLE));
			break;
		case IDC_STOP_ZTIMER:/*暂停刷僵尸*/
			pvz->StopZTimer(IsDlgButtonChecked(hWnd, IDC_STOP_ZTIMER));
			break;
		case IDC_STOP_ZOMBIES:/*僵尸不前进*/
			pvz->StopZombies(IsDlgButtonChecked(hWnd, IDC_STOP_ZOMBIES));
			break;
		case IDC_STOP_BOSS:/*定BOSS*/
			pvz->StopBoss(IsDlgButtonChecked(hWnd, IDC_STOP_BOSS));
			break;
		case IDC_BALLOON_BOOM:/*气球自爆*/
			pvz->BalloonBoom(IsDlgButtonChecked(hWnd, IDC_BALLOON_BOOM));
			break;
		case IDC_ZTIMER_SPEEDUP:/*加速刷僵尸*/
			if (IsDlgButtonChecked(hWnd, IDC_ZTIMER_SPEEDUP))
				pvz->ZTimerSpeed(400 - SendMessage(GetDlgItem(hWnd, IDC_SLIDER_TIMER), TBM_GETPOS, 0, 0));
			else
				pvz->ZTimerSpeed(-1);
			break;
		case IDC_GATHER_ZOMBIE:/*吸怪*/
			pvz->GatherZombie(IsDlgButtonChecked(hWnd, IDC_GATHER_ZOMBIE),
				(float)SendMessage(GetDlgItem(hWnd, IDC_SLIDER_GATHER), TBM_GETPOS, 0, 0));
			break;
		case IDC_KILL_ALLZ:/*秒杀僵尸*/
			pvz->KillAllZombies();
			break;
		case IDC_CONFUSE_ALL:/*魅惑僵尸*/
			pvz->ConfuseZombies();
			break;
		default:
			return FALSE;
		}
		break;
	case WM_HSCROLL:
		if (lParam == (LPARAM)GetDlgItem(hWnd, IDC_SLIDER_TIMER)
			&& IsDlgButtonChecked(hWnd, IDC_ZTIMER_SPEEDUP))
		{
			SendMessage(hWnd, WM_COMMAND, IDC_ZTIMER_SPEEDUP, 0);
		}
		else if (lParam == (LPARAM)GetDlgItem(hWnd, IDC_SLIDER_GATHER)
			&& IsDlgButtonChecked(hWnd, IDC_GATHER_ZOMBIE))
		{
			SendMessage(hWnd, WM_COMMAND, IDC_GATHER_ZOMBIE, 0);
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK CardProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDC_CARD_NUM:/*卡槽数量*/
			pvz->SetCardNum(GetDlgItemInt(hWnd, IDC_CNLIST, NULL, TRUE));
			break;
		case IDC_CPBTN:/*修改为植物*/
		{
			int type = SendDlgItemMessage(hWnd, IDC_CPLIST, CB_GETCURSEL, 0, 0);
			if (type == 48)
				type = 52;
			pvz->SetCard(GetDlgItemInt(hWnd, IDC_CIDLIST, NULL, TRUE) - 1, type, IsDlgButtonChecked(hWnd, IDC_CPCOPY));
			break;
		}
		case IDC_CZBTN:/*修改为僵尸*/
			pvz->SetCard(GetDlgItemInt(hWnd, IDC_CIDLIST, NULL, TRUE) - 1, SendDlgItemMessage(hWnd, IDC_CZLIST, CB_GETCURSEL, 0, 0) + 60);
			break;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK SeedProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// 分析菜单选择: 
		switch (wmId)
		{
		case IDC_ADD_LIST:
		{
			char zname[10];
			GetDlgItemText(hWnd, IDC_SLIST, zname, 10);
			if (SendDlgItemMessage(hWnd, IDC_SLIST_BOX, LB_FINDSTRING, -1, LPARAM(zname)) == LB_ERR)
			{
				SendDlgItemMessage(hWnd, IDC_SLIST_BOX, LB_ADDSTRING, 0, LPARAM(zname));
			}
			break;
		}
		case IDC_DEL_SLIST:
			SendDlgItemMessage(hWnd, IDC_SLIST_BOX, LB_DELETESTRING,
				SendDlgItemMessage(hWnd, IDC_SLIST_BOX, LB_GETCURSEL, 0, 0), 0);
			break;
		case IDC_CLEAR_SLIST:
			SendDlgItemMessage(hWnd, IDC_SLIST_BOX, LB_RESETCONTENT, 0, 0);
			break;
		case IDC_HELP_SEED:
			MessageBox(hWnd,
				"1.自然刷怪只修改了出怪种子，出怪列表则是调用了游戏自身的函数来决定\n"
				"2.僵尸并非<完全>由种子控制，也受自身属性影响\n"
				"3.在某些情况下自然刷怪，修改器会进行某种干预以保证游戏不崩溃\n"
				"4.极限刷怪平均填充出怪列表，不修改出怪种子，默认不对任何僵尸做特殊处理",
				"刷怪说明", MB_OK);
			break;
		case IDC_CHG_SEED:
		{
			char zname[10];
			int zombies[sizeof(zList) / sizeof(*zList)] = { 0 };
			int znum = SendDlgItemMessage(hWnd, IDC_SLIST_BOX, LB_GETCOUNT, 0, 0);
			if (znum == 0)
				break;
			for (int i = 0; i < znum; ++i)
			{
				SendDlgItemMessage(hWnd, IDC_SLIST_BOX, LB_GETTEXT, i, LPARAM(zname));
				zombies[i] = SendDlgItemMessage(hWnd, IDC_SLIST, CB_FINDSTRING, -1, LPARAM(zname));
			}
			if (IsDlgButtonChecked(hWnd, IDC_NATURAL))
			{
				pvz->NaturalSeed(zombies, znum);
			}
			else
			{
				if (znum == 2 &&
					(zombies[0] == 20 && zombies[1] == 32
						|| zombies[0] == 32 && zombies[1] == 20))
					CheckDlgButton(hWnd, IDC_RED_LIMIT, BST_UNCHECKED);
				pvz->LimitSeed(zombies, znum, IsDlgButtonChecked(hWnd, IDC_THIEF_LIMIT), IsDlgButtonChecked(hWnd, IDC_RED_LIMIT));
			}
			break;
		}
		case IDC_RESET_SEED:/*撤销修改出怪*/
			pvz->ResetSeed();
			break;
		case IDC_ZLEFT:
		{
			bool zombies[33] = { 0 };
			pvz->SeeLeftZombies(zombies);
			char str[300];
			char temp[10];
			str[0] = '\0';
			int j = 0;
			for (int i = 0; i < 33; ++i)
			{
				if (zombies[i])
				{
					sprintf(temp, "%d.%s\n", ++j, zList[i]);
					strcat(str, temp);
				}
			}
			if (str[0] != '\0')
				MessageBox(hWnd, str, "剩余出怪种类", MB_OK);
			break;
		}
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK FullScreenProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		if (pvz == NULL)
			return 0;
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDC_FSBOOM:/*灰烬全屏*/
			pvz->FSBoom(IsDlgButtonChecked(hWnd, IDC_FSBOOM));
			break;
		case IDC_FSSQUASH:/*窝瓜全屏*/
			pvz->FSSquash(IsDlgButtonChecked(hWnd, IDC_FSSQUASH));
			break;
		case IDC_FSMAGNET:/*磁力菇全屏*/
			pvz->FSMagnet(IsDlgButtonChecked(hWnd, IDC_FSMAGNET));
			break;
		case IDC_FSSPUTTER:/*溅射全屏*/
			pvz->FSSputter(IsDlgButtonChecked(hWnd, IDC_FSSPUTTER));
			break;
		case IDC_FSMOUSE:/*大嘴花全屏*/
			pvz->FSMouth(IsDlgButtonChecked(hWnd, IDC_FSMOUSE));
			break;
		case IDC_FSPRICK:/*地刺全屏*/
			pvz->FSPrick(IsDlgButtonChecked(hWnd, IDC_FSPRICK));
			break;
		case IDC_FSSPOUT:/*喷射全屏*/
			pvz->FSSpout(IsDlgButtonChecked(hWnd, IDC_FSSPOUT));
			break;
		case IDC_FSCAT:/*猫式全屏*/
			pvz->FSCat(IsDlgButtonChecked(hWnd, IDC_FSCAT));
			break;
		case IDC_CATTOUCH:/*触碰伤害*/
			pvz->CatTouch(IsDlgButtonChecked(hWnd, IDC_CATTOUCH));
			break;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK GardenProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDC_FLOWERNPK:
			pvz->FlowerNPK(IsDlgButtonChecked(hWnd, IDC_FLOWERNPK));
			break;
		case IDC_MEDICINE:
			pvz->Medicine(IsDlgButtonChecked(hWnd, IDC_MEDICINE));
			break;
		case IDC_CHOCOLATE:
			pvz->Chocolate(IsDlgButtonChecked(hWnd, IDC_CHOCOLATE));
			break;
		case IDC_TREENPK:
			pvz->TreeNPK(IsDlgButtonChecked(hWnd, IDC_TREENPK));
			break;
		case IDC_TREE_HIGH:
			pvz->TreeHeight(GetDlgItemInt(hWnd, IDC_TREE_HEIGHT, NULL, TRUE));
			break;
		case IDC_FLOWER_NUMBER:
			pvz->FlowerNum(GetDlgItemInt(hWnd, IDC_FLOWER_NUM, NULL, TRUE));
			break;
		case IDC_CHG_FLOWER:
			pvz->ChangeFlower(GetDlgItemInt(hWnd, IDC_FLOWER_ID, NULL, TRUE),
				SendDlgItemMessage(hWnd, IDC_FLOWER_LIST, CB_GETCURSEL, 0, 0));
			break;
		case IDC_CHG_FSIZE:
			pvz->ChangeFSize(GetDlgItemInt(hWnd, IDC_FLOWER_ID, NULL, TRUE), 
				SendDlgItemMessage(hWnd, IDC_FLOWER_SIZE, CB_GETCURSEL, 0, 0));
			break;
		case IDC_F_LEFT:
			pvz->ChangeFDir(GetDlgItemInt(hWnd, IDC_FLOWER_ID, NULL, TRUE), 1);
			break;
		case IDC_F_RIGHT:
			pvz->ChangeFDir(GetDlgItemInt(hWnd, IDC_FLOWER_ID, NULL, TRUE), 0);
			break;
		case IDC_CHG_FPLACE:
			pvz->ChangeFPlace(GetDlgItemInt(hWnd, IDC_FLOWER_ID, NULL, TRUE),
				SendDlgItemMessage(hWnd, IDC_PLACE_LIST, CB_GETCURSEL, 0, 0));
			break;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

#pragma once
#include"service.h"
//键盘符号：
#define ENTER 13
#define ESC 27
#define CTRL_C 3
#define CTRL_V 22
//与自然序列一致的宏：
#define leftup 0
#define up 1
#define rightup 2
#define left 3
#define center 4
#define right 5
#define leftdown 6
#define down 7
#define rightdown 8
//图像与grid的参数：
#define scale 32
#define PIC 768
#define INFO 256
#define pwidth (PIC+INFO)
#define pheight PIC

#define picbyte (pwidth*pheight*sizeof(DWORD))
extern DWORD(*p)[pwidth];

#if (scale%8==0)
//macmove采用的是图像坐标系。
//统一起见，全部采用图像坐标系。
inline int MacRev(int dir)
{
	switch (dir)
	{
	case up:return down;
	case down:return up;
	case left:return right;
	case right:return left;
	case leftup:return rightdown;
	case rightup:return leftdown;
	case leftdown:return rightup;
	case rightdown:return leftup;
	default:throw(-1);//包括center。
	}
}
inline void MacMove(int& x, int& y, int direction)
{
	switch (direction)
	{
	case up:y--; break;
	case down:y++; break;
	case left:x--; break;
	case right:x++; break;
	case leftup:x--; y--; break;
	case rightup:x++; y--; break;
	case leftdown:x--; y++; break;
	case rightdown:x++; y++; break;
	default:throw(-1);//包括center。
	}
}
inline int WASDtoDir(int wasdx)
{
	switch (wasdx)
	{
	case 'q':return 0;
	case 'w':return 1;
	case 'e':return 2;
	case 'a':return 3;
	case 's':return 4;
	case 'd':return 5;
	case 'z':return 6;
	case 'x':return 7;
	case 'c':return 8;
	default:throw(-1);
	}
}

typedef char _bs_[10];//bs规则集，与掩码一并发挥作用。
typedef char _count_[9];//掩码。
typedef char _genrule_[512]; //完全规则集。bs规则集是先翻译到这里才发挥作用的。
typedef char _core_[9];//一个元胞邻域状态，采用自然编码012\n345\n678，是闭包思想的体现。

#define NO 0
#define S 2
#define B 1
#define BS 3
#define SB 3
//B:0->1,1->0
//S:0->0,1_>1
//NO:0->0,1->0
//BS:0->1,1->1

extern _genrule_ nowrule;
extern _count_ nowcount;
extern _bs_ nowbs;

//使用的rulemode。
#define SUMMODE 0
#define GENERALMODE 1
extern int rulemode;

//两个方法都会用到。
//<<并不会截断。
inline char _ruleuse_(const _core_ cr)
{
	int state = (cr[leftup] << 0) | (cr[up] << 1) | (cr[rightup] << 2) | (cr[left] << 3) | (cr[center] << 4) | (cr[right] << 5) | (cr[leftdown] << 6) | (cr[down] << 7) | (cr[rightdown] << 8);
	return nowrule[state];
}
inline char _ruleuse_(char _leftup, char _up, char _rightup, char _left, char _center, char _right, char _leftdown, char _down, char _rightdown)
{
	int state = (_leftup << 0) | (_up << 1) | (_rightup << 2) | (_left << 3) | (_center << 4) | (_right << 5) | (_leftdown << 6) | (_down << 7) | (_rightdown << 8);
	return nowrule[state];
}


//元胞网格：
typedef
struct gridunit
{
	int lives;//活细胞的数量。
	int x,y;
	char cell[scale][scale];//采用这个模式，可以节省很多内存移动的计算。
	char vice[scale][scale];
	gridunit* neighbors[9];//为了与自然序兼容，包括了自己。
}*grid;
extern int at;//指的是at的为操作胞。!at为副区。有了它，可以轻易实现单步回溯。





struct RL_structure
{
	int savekind;//summode，generalmode。
	union
	{
		struct {
			_count_ count;
			_bs_ bs;
		}sumrl;
		unsigned totrl[512 / (8*sizeof(unsigned))];
	}rl;
	RL_structure(void);
	bool release(void);//成功返回true。
	//在rule.cpp中实现。
};

/*****************
*DX,DY:在解析时，每个网格自带的坐标加上这一个偏移量。
******************/
#define TO ('T'+('O'<<8))
#define FL ('F'+('L'<<8))
#define AR ('A'+('R'<<8))

#define NODERTA 0
#define DERTA 1
#define idcode 743027529//nul正好用于存储idcode，作为头识别码。
#pragma warning(disable:26495)
struct FH_structure//头结构体。
{
	int type;
	RL_structure RL;
	int tag, DX, DY;
	int nul;//空闲变量。
};
struct UN_structure//保存网格的结构体。
{
	int x;
	int y;
	unsigned int data[scale*scale/(8*sizeof(unsigned))];
};


//在area.cpp中实现。
//true表示非空。
//第一个：整体保存。
//flood：以目标网格开始，利用洪水填充法来一片连通的包。
//【这并不意味着图样的完整性。】
//range：选择空间进行保存。弥补了flood的缺点。
//为了方便实现，saf不带tag，flood带偏移，以x，y为原点。ranged带偏移，以调整后的xf，yf为原点。

bool save_as_file(void);
bool save_as_file_flood(int x, int y);
bool save_as_file_ranged(int xf, int yf, int xto, int yto);

//读取文件。
bool read_from_file(const char* fname);
#endif

////////应该添加头参数!!!!!。
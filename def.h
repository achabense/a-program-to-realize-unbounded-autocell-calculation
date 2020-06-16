#pragma once
#include"service.h"
//���̷��ţ�
#define ENTER 13
#define ESC 27
#define CTRL_C 3
#define CTRL_V 22
//����Ȼ����һ�µĺ꣺
#define leftup 0
#define up 1
#define rightup 2
#define left 3
#define center 4
#define right 5
#define leftdown 6
#define down 7
#define rightdown 8
//ͼ����grid�Ĳ�����
#define scale 32
#define PIC 768
#define INFO 256
#define pwidth (PIC+INFO)
#define pheight PIC

#define picbyte (pwidth*pheight*sizeof(DWORD))
extern DWORD(*p)[pwidth];

#if (scale%8==0)
//macmove���õ���ͼ������ϵ��
//ͳһ�����ȫ������ͼ������ϵ��
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
	default:throw(-1);//����center��
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
	default:throw(-1);//����center��
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

typedef char _bs_[10];//bs���򼯣�������һ���������á�
typedef char _count_[9];//���롣
typedef char _genrule_[512]; //��ȫ���򼯡�bs�������ȷ��뵽����ŷ������õġ�
typedef char _core_[9];//һ��Ԫ������״̬��������Ȼ����012\n345\n678���Ǳհ�˼������֡�

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

//ʹ�õ�rulemode��
#define SUMMODE 0
#define GENERALMODE 1
extern int rulemode;

//�������������õ���
//<<������ضϡ�
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


//Ԫ������
typedef
struct gridunit
{
	int lives;//��ϸ����������
	int x,y;
	char cell[scale][scale];//�������ģʽ�����Խ�ʡ�ܶ��ڴ��ƶ��ļ��㡣
	char vice[scale][scale];
	gridunit* neighbors[9];//Ϊ������Ȼ����ݣ��������Լ���
}*grid;
extern int at;//ָ����at��Ϊ��������!atΪ����������������������ʵ�ֵ������ݡ�





struct RL_structure
{
	int savekind;//summode��generalmode��
	union
	{
		struct {
			_count_ count;
			_bs_ bs;
		}sumrl;
		unsigned totrl[512 / (8*sizeof(unsigned))];
	}rl;
	RL_structure(void);
	bool release(void);//�ɹ�����true��
	//��rule.cpp��ʵ�֡�
};

/*****************
*DX,DY:�ڽ���ʱ��ÿ�������Դ������������һ��ƫ������
******************/
#define TO ('T'+('O'<<8))
#define FL ('F'+('L'<<8))
#define AR ('A'+('R'<<8))

#define NODERTA 0
#define DERTA 1
#define idcode 743027529//nul�������ڴ洢idcode����Ϊͷʶ���롣
#pragma warning(disable:26495)
struct FH_structure//ͷ�ṹ�塣
{
	int type;
	RL_structure RL;
	int tag, DX, DY;
	int nul;//���б�����
};
struct UN_structure//��������Ľṹ�塣
{
	int x;
	int y;
	unsigned int data[scale*scale/(8*sizeof(unsigned))];
};


//��area.cpp��ʵ�֡�
//true��ʾ�ǿա�
//��һ�������屣�档
//flood����Ŀ������ʼ�����ú�ˮ��䷨��һƬ��ͨ�İ���
//���Ⲣ����ζ��ͼ���������ԡ���
//range��ѡ��ռ���б��档�ֲ���flood��ȱ�㡣
//Ϊ�˷���ʵ�֣�saf����tag��flood��ƫ�ƣ���x��yΪԭ�㡣ranged��ƫ�ƣ��Ե������xf��yfΪԭ�㡣

bool save_as_file(void);
bool save_as_file_flood(int x, int y);
bool save_as_file_ranged(int xf, int yf, int xto, int yto);

//��ȡ�ļ���
bool read_from_file(const char* fname);
#endif

////////Ӧ�����ͷ����!!!!!��
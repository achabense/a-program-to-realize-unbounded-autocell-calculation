#include<stdlib.h>
#include"area.h"

/*****************************************
*2020/6/15�ع���
*hash���floodfill�ⵥ���г��Ե���ɢ��Ƕ�����
*ɾ���˶����ʵ�֡�
******************************************/
#if !defined hashph

/**************************
*hash.h
*2020/6/15�ع���
*ԭ�ȵ�ά��grid�ռ�������
*���±ߵ�floodfill����ȣ���Ҫ�����Ƕ���putout���ܡ�
***************************/

#define hashmod 2048
inline unsigned hashfunc(int x, int y)
{
	return (unsigned)(1414 * x + 1561 * y) % hashmod;
}

struct xygnode
{
	int x, y;
	grid gd;
	xygnode* next;
	xygnode(grid pgd, xygnode* pnext) :x(pgd->x), y(pgd->y), gd(pgd), next(pnext) {}
	xygnode(void) :x(0), y(0), gd(nullptr), next(nullptr) {}
}hashtable[hashmod]{};
void hashwashup(void)
{
	for (int i = 0; i < hashmod; i++)
	{
		xygnode* cur = hashtable[i].next;
		xygnode* med;
		while (cur != nullptr) {
			med = cur;
			cur = cur->next;
			delete med;
		}
		hashtable[i].next = nullptr;
	}
}
grid search(int x, int y)
{
	xygnode* sch = hashtable[hashfunc(x, y)].next;
	while (sch != nullptr)
	{
		if (sch->x == x && sch->y == y) {
			assert(sch->gd != nullptr);
			return sch->gd;
		}
		else { sch = sch->next; }
	}
	return nullptr;
}
//�ϰ溯����putin���м�Ⲣ����boolֵ��
//putin�ķ��ظ����Ǳ�Ҫ�ģ�����һ���ڳ�����������search������֤�ġ�
void putin(grid gd)
{
	assert(gd != nullptr);
	unsigned hash = hashfunc(gd->x, gd->y);
	hashtable[hash].next = new xygnode(gd, hashtable[hash].next);
}

bool putout(grid gd)
{
	assert(gd != nullptr);
	int x = gd->x;
	int y = gd->y;
	unsigned hash = hashfunc(x, y);
	struct xygnode* sch = hashtable[hash].next;
	struct xygnode* presch = &hashtable[hash];
	while (sch != nullptr)
	{
		if (sch->x == x && sch->y == y)
		{
			presch->next = sch->next;
			delete sch;
			return true;
		}
		else { presch = sch; sch = sch->next; }
	}
	return false;
}

#endif



#if !defined floodfillph
/**************************
*floodfill.h
*2020/6/15�ع���
*֧������ռ�floodfill�Ŀ⡣�ϲ���area�С�
*���˴��룬ֻʵ�ֱ�Ҫ�Ĺ��ܡ�
*ʹ���˸���cpp�ķ��
*ȫ��д��һ��ͷ�ļ����ǺϷ��ġ�
***************************/
//֮�����õ�cood������Ϊ��floodfillջ�ݹ�����Ҫ�Խṹ�����ʽ����ֵ��
struct cood
{
	int x, y;
	cood(int px, int py) :x(px), y(py) {}
	bool operator==(cood cmp)
	{
		return x == cmp.x && y == cmp.y;
	}
};
typedef
struct stackunit
{
	cood cood;
	stackunit* last;
	stackunit(struct cood pcood, stackunit* plast) :cood(pcood), last(plast) {}
}*&top;

inline bool isempty(top top) { return top == nullptr; }
inline void push(top top, cood cood) { top = new stackunit(cood, top); }
inline cood pop(top top)
{
	assert(top != nullptr);
	cood cd = top->cood;
	stackunit* unit = top;
	top = top->last;
	delete unit;
	return cd;
}

//�ɿ�����stackunitʵ����һ���ģ�����Ϊ�����ֺ��壬�ض���ṹ��
struct coodnode
{
	cood cood;
	coodnode* next;
	coodnode(struct cood pcood, coodnode* pnext) :cood(pcood), next(pnext) {}
	coodnode(void) :cood({ 0,0 }), next(nullptr) {}
};
//����Ĳ��ұ�Ҳ��һ����ϣ��
struct coodtable
{
	coodnode* hashtable;
	int hmod;
	coodtable(int phmod)
	{
		if (phmod <= 0 || phmod > 0xffffff)throw(-1);
		hmod = phmod;
		hashtable = new coodnode[hmod];
	}
	~coodtable(void)
	{
		for (int i = 0; i < hmod; i++) {
			coodnode* cur = hashtable[i].next;
			coodnode* med;
			while (cur != nullptr) {
				med = cur;
				cur = cur->next;
				delete med;
			}
		}
		delete[] hashtable;
	}
	unsigned hfunc(cood cood)
	{
		return (unsigned)(1414 * cood.x + 1561 * cood.y) % hmod;
	}
	bool isin(cood cood)
	{
		coodnode* sch = hashtable[hfunc(cood)].next;
		while (sch != nullptr)
		{
			if (sch->cood == cood) {
				return true;
			}
			else { sch = sch->next; }
		}
		return false;
	}
	void putin(cood cood)//������Ƿ��ظ���ֱ�ӷ��롣�����Ե�ά����isin�е���
	{
		unsigned hash = hfunc(cood);
		hashtable[hash].next = new coodnode(cood, hashtable[hash].next);
	}
};
#endif


/***********************************
*2020/6/15�ع���
*ʵ�ֻ��Ƶ�ȱ�㣺������ʵ�����ɾ��������ɾ������acirc��ʵ�ֵġ�
*Ϊ��ͳһ�߼������Ч�ʣ�������ͼ������ϵ��(x,y)�Ա�ʾ(�ᣬ��)����������������[y][x]����[x][y]��ʾ��
*���������򣬶Ե��Ĺ������ǿֲ���
*����ͼʹ��[at][][]�ܹ����Թ���ڴ渴�ơ������������ֲ���bug���ڡ�ĳЩ�����������ʧЧ������ȡ���ˡ�
*��Ĳ��붯�������������
*���ͼ����acorn��
***********************************/
void _calgrid_(grid gd)
{
	assert(gd != nullptr);
	int liveup = 0;
	char(*cl)[scale] = gd->cell;
	char(*vic)[scale] = gd->vice;
	grid* nbs = gd->neighbors;
	//����ȫ����
	if (gd->lives != 0) {
		for (int y = 1; y < scale - 1; y++)
			for (int x = 1; x < scale - 1; x++)
			{
				liveup += (vic[y][x] = _ruleuse_(cl[y - 1][x - 1], cl[y - 1][x], cl[y - 1][x + 1], cl[y][x - 1], cl[y][x], cl[y][x + 1], cl[y + 1][x - 1], cl[y + 1][x], cl[y + 1][x + 1]));
			}
	}
	if (nbs[up] != nullptr) {//��
		char(*ncl)[scale] = nbs[up]->cell;
		for (int x = 1; x < scale - 1; x++) {
			liveup += (vic[0][x] = _ruleuse_(ncl[scale - 1][x - 1], ncl[scale - 1][x], ncl[scale - 1][x + 1], cl[0][x - 1], cl[0][x], cl[0][x + 1], cl[1][x - 1], cl[1][x], cl[1][x + 1]));
		}
	}
	else {
		for (int x = 1; x < scale - 1; x++) {
			liveup += (vic[0][x] = _ruleuse_(0, 0, 0, cl[0][x - 1], cl[0][x], cl[0][x + 1], cl[1][x - 1], cl[1][x], cl[1][x + 1]));
		}
	}
	if (nbs[down] != nullptr) {//��
		char(*ncl)[scale] = nbs[down]->cell;
		for (int x = 1; x < scale - 1; x++) {
			liveup += (vic[scale - 1][x] = _ruleuse_(cl[scale - 2][x - 1], cl[scale - 2][x], cl[scale - 2][x + 1], cl[scale - 1][x - 1], cl[scale - 1][x], cl[scale - 1][x + 1], ncl[0][x - 1], ncl[0][x], ncl[0][x + 1]));
		}
	}
	else {
		for (int x = 1; x < scale - 1; x++) {
			liveup += (vic[scale - 1][x] = _ruleuse_(cl[scale - 2][x - 1], cl[scale - 2][x], cl[scale - 2][x + 1], cl[scale - 1][x - 1], cl[scale - 1][x], cl[scale - 1][x + 1], 0, 0, 0));
		}
	}
	if (nbs[left] != nullptr) {//��
		char(*ncl)[scale] = nbs[left]->cell;
		for (int y = 1; y < scale - 1; y++) {
			liveup += (vic[y][0] = _ruleuse_(ncl[y - 1][scale - 1], cl[y - 1][0], cl[y - 1][1], ncl[y][scale - 1], cl[y][0], cl[y][1], ncl[y + 1][scale - 1], cl[y + 1][0], cl[y + 1][1]));
		}
	}
	else {
		for (int y = 1; y < scale - 1; y++) {
			liveup += (vic[y][0] = _ruleuse_(0, cl[y - 1][0], cl[y - 1][1], 0, cl[y][0], cl[y][1], 0, cl[y + 1][0], cl[y + 1][1]));
		}
	}
	if (nbs[right] != nullptr) {//��
		char(*ncl)[scale] = nbs[right]->cell;
		for (int y = 1; y < scale - 1; y++) {
			liveup += (vic[y][scale - 1] = _ruleuse_(cl[y - 1][scale - 2], cl[y - 1][scale - 1], ncl[y - 1][0], cl[y][scale - 2], cl[y][scale - 1], ncl[y][0], cl[y + 1][scale - 2], cl[y + 1][scale - 1], ncl[y + 1][0]));
		}
	}
	else {
		for (int y = 1; y < scale - 1; y++) {
			liveup += (vic[y][scale - 1] = _ruleuse_(cl[y - 1][scale - 2], cl[y - 1][scale - 1], 0, cl[y][scale - 2], cl[y][scale - 1], 0, cl[y + 1][scale - 2], cl[y + 1][scale - 1], 0));
		}
	}
	//���ĸ��߽ǣ����ɲ���֮ǰ�ķ��
	_core_ core = { 0 };
	{//����(0,0)��
		core[center] = cl[0][0];
		core[right] = cl[0][1];
		core[down] = cl[1][0];
		core[rightdown] = cl[1][1];
		if (nbs[up] != nullptr) {
			core[up] = nbs[up]->cell[scale - 1][0]; core[rightup] = nbs[up]->cell[scale - 1][1];
		}
		else {
			core[up] = 0; core[rightup] = 0;
		}
		if (nbs[left] != nullptr) {
			core[left] = nbs[left]->cell[0][scale - 1]; core[leftdown] = nbs[left]->cell[1][scale - 1];
		}
		else {
			core[left] = 0; core[leftdown] = 0;
		}
		if (nbs[leftup] != nullptr) {
			core[leftup] = nbs[leftup]->cell[scale - 1][scale - 1];
		}
		else {
			core[leftup] = 0;
		}
		liveup += (vic[0][0] = _ruleuse_(core));
	}
	{//���ϣ�
		core[center] = cl[0][scale - 1];
		core[left] = cl[0][scale - 2];
		core[down] = cl[1][scale - 1];
		core[leftdown] = cl[1][scale - 2];
		if (nbs[up] != nullptr) {
			core[up] = nbs[up]->cell[scale - 1][scale - 1]; core[leftup] = nbs[up]->cell[scale - 1][scale - 2];
		}
		else {
			core[up] = 0; core[leftup] = 0;
		}
		if (nbs[right] != nullptr) {
			core[right] = nbs[right]->cell[0][0]; core[rightdown] = nbs[right]->cell[1][0];
		}
		else {
			core[right] = 0; core[rightdown] = 0;
		}
		if (nbs[rightup] != nullptr) {
			core[rightup] = nbs[rightup]->cell[scale - 1][0];
		}
		else {
			core[rightup] = 0;
		}
		liveup += (vic[0][scale - 1] = _ruleuse_(core));
	}
	{//���£�
		core[center] = cl[scale - 1][0];
		core[up] = cl[scale - 2][0];
		core[right] = cl[scale - 1][1];
		core[rightup] = cl[scale - 2][1];
		if (nbs[down] != nullptr) {
			core[down] = nbs[down]->cell[0][0]; core[rightdown] = nbs[down]->cell[0][1];
		}
		else {
			core[down] = 0; core[rightdown] = 0;
		}
		if (nbs[left] != nullptr) {
			core[left] = nbs[left]->cell[scale - 1][scale - 1]; core[leftup] = nbs[left]->cell[scale - 2][scale - 1];
		}
		else {
			core[left] = 0; core[leftup] = 0;
		}
		if (nbs[leftdown] != nullptr) {
			core[leftdown] = nbs[leftdown]->cell[0][scale - 1];
		}
		else {
			core[leftdown] = 0;
		}
		liveup += (vic[scale - 1][0] = _ruleuse_(core));
	}
	{//���£�
		core[center] = cl[scale - 1][scale - 1];
		core[left] = cl[scale - 1][scale - 2];
		core[up] = cl[scale - 2][scale - 1];
		core[leftup] = cl[scale - 2][scale - 2];
		if (nbs[down] != nullptr) {
			core[down] = nbs[down]->cell[0][scale - 1]; core[leftdown] = nbs[down]->cell[0][scale - 2];
		}
		else {
			core[down] = 0; core[leftdown] = 0;
		}
		if (nbs[right] != nullptr) {
			core[right] = nbs[right]->cell[scale - 1][0]; core[rightup] = nbs[right]->cell[scale - 2][0];
		}
		else {
			core[right] = 0; core[rightup] = 0;
		}
		if (nbs[rightdown] != nullptr) {
			core[rightdown] = nbs[rightdown]->cell[0][0];
		}
		else {
			core[rightdown] = 0;
		}
		liveup += (vic[scale - 1][scale - 1] = _ruleuse_(core));
	}
	gd->lives = liveup;
}

inline void _dellink_(grid gd)
{
	assert(gd != nullptr);
	grid* nbs = gd->neighbors;
	for(int i=0;i<9;i++)
		if (nbs[i] != nullptr)//��ʵ�֣�center��Ϊnullptr��
			nbs[i]->neighbors[MacRev(i)] = nullptr;
}

grid gridliveup(int x, int y)
{
	grid newone = new struct gridunit;
	newone->x = x, newone->y = y, newone->lives = 0;
	putin(newone);

	memset(newone->cell, 0, scale * scale * sizeof(char));
	memset(newone->vice, 0, scale * scale * sizeof(char));
	
	for (int i = 0; i < 9; i++)
		newone->neighbors[i] = nullptr;
	for (int i = 0; i < 9; i++)
	{
		if (i != center) {
			int seekx = x;
			int seeky = y;
			MacMove(seekx, seeky, i);
			grid seek = search(seekx, seeky);
			if (seek != nullptr) {
				newone->neighbors[i] = seek;
				seek->neighbors[MacRev(i)] = newone;
			}
		}
	}
	return newone;
}
inline void griddie(grid gd)
{
	assert(gd != nullptr);
	putout(gd);
	_dellink_(gd);
	delete gd;
}
/*********************
*��������
*********************/


struct gridnode {
	grid gd;
	gridnode* next;
};
/*********************
*�ڽڵ�β������½ڵ㡣
*********************/
inline void nodeappend(gridnode*& end, grid gd)
{
	assert(end->next == nullptr && gd != nullptr);
	end->next = new gridnode;
	end = end->next;
	end->next = nullptr;
	end->gd = gd;
}
/******************
*ɾ��at���Ľڵ㡣
******************/
inline void nodedel(gridnode*& before, gridnode*& at)
{
	assert(before != nullptr && at != nullptr && before->next == at);
	before->next = at->next;
	delete at;
	at = before->next;
}

struct gridnode gridhead = { nullptr,nullptr };
//���壺�䡾next��ָ��nullptr��
struct gridnode* gridend = &gridhead;

void acirc(long long* Blocks, long long* Cells)
{
	int blocks = 0;
	int cells = 0;
	gridnode* cur;
	gridnode* lastcur;

	gridnode tempnode = { nullptr,nullptr };
	gridnode* tempcur = &tempnode;

	cur = gridhead.next;
	lastcur = &gridhead;
	while (cur != nullptr)//��顣
	{
		grid gd = cur->gd;
		if (gd->lives != 0) {
			grid* neighbors = gd->neighbors;
			for (int i = 0; i < 9; i++)
			{
				if (i != center&& neighbors[i] == nullptr)
				{
					int x = gd->x;
					int y = gd->y;
					MacMove(x, y, i);
					nodeappend(tempcur, gridliveup(x, y));//liveup����ȷ������
					assert(neighbors[i] != nullptr);
				}
			}
		}
		lastcur = cur;
		cur = cur->next;
	}

	if (lastcur == &gridhead) {
		if (Blocks != nullptr)(*Blocks) = 0;
		if (Cells != nullptr)(*Cells) = 0;
		return;
	}//�ǿա�

	lastcur->next = tempnode.next;//���ϡ�

	//rewind:
	cur = gridhead.next;
	while (cur != nullptr)
	{
		_calgrid_(cur->gd);
		cur = cur->next;
	}

	//rewind:
	cur = gridhead.next;
	lastcur = &gridhead;
	while (cur != nullptr)
	{
		grid gd = cur->gd;
		if (gd->lives == 0) {
			int islive = 0;
			for (int i = 0; i < 9; i++)
			{
				if (gd->neighbors[i] != nullptr && gd->neighbors[i]->lives != 0)
				{
					islive = 1; break;
				}
			}//�������������ƣ�center��Ϊnullptr��
			if (islive == 0) {
				griddie(gd);
				nodedel(lastcur, cur);;
			}
			else {
				memset(gd->cell, 0, scale * scale * sizeof(char));
				lastcur = cur;
				cur = cur->next;
				blocks++;
			}
		}
		else {
			memcpy(gd->cell, gd->vice, scale * scale * sizeof(char));
			lastcur = cur;
			cur = cur->next;
			cells += gd->lives;
			blocks++;
		}
	}

	gridend = lastcur;

	if (Blocks != nullptr)(*Blocks) = blocks;
	if (Cells != nullptr)(*Cells) = cells;
}

void clearall(void)
{
	hashwashup();
	gridnode* del = gridhead.next;
	while (del != nullptr) {
		gridnode* med = del;
		del = del->next;
		delete med->gd;
		delete med;
	}
	gridhead.next = nullptr;
	gridend = &gridhead;
}
/***************************************************
*��񣺺����������жϡ�
*����������У�ֻҪ�����߼���ȷ���ǲ�Ӧ�ó��ִ���ġ�
*���Ҳʹ��assert��
***************************************************/
void outerset(int x, int y, int dx, int dy, int as)
{
	assert(dx >= 0 && dy >= 0 && dx < scale&& dy < scale && (as == 0 || as == 1));
	grid seek = search(x, y);
	if (seek != nullptr) {
		int cellval = seek->cell[dy][dx];
		if (cellval == 0 && as == 1)seek->cell[dy][dx] = 1, seek->lives++;
		else if (cellval == 1 && as == 0)seek->cell[dy][dx] = 0, seek->lives--;
	}
	else if (as == 1) {
		grid gd = gridliveup(x, y);
		nodeappend(gridend, gd);
		gd->cell[dy][dx] = 1;
		gd->lives = 1;
	}
}

void outerblockset(int x, int y, char(*cl)[scale])
{
	assert(cl != nullptr);
	grid seek = search(x, y);
	if (seek != nullptr) {
		int lives = 0;
		for (int y = 0; y < scale; y++)
			for (int x = 0; x < scale; x++)
				lives += (seek->cell[y][x] = cl[y][x]);
		seek->lives = lives;
	}
	else {
		grid gd = gridliveup(x, y);
		nodeappend(gridend, gd);
		int lives = 0;
		for (int y = 0; y < scale; y++)
			for (int x = 0; x < scale; x++)
				lives += (gd->cell[y][x] = cl[y][x]);
		gd->lives = lives;
	}
}

/********************
*�����ǳ���ĺ��Ĳ��֣��ṩԪ���ռ�ļ��㹦�ܡ�
********************/
//��������ĺ�����
//��ĳЩ���򼯲����á�
char cell[scale][scale] = { 0 };
char cell_vice[scale][scale] = { 0 };
char cell_stil[scale][scale] = { 0 };
int curx = scale / 2, cury = scale / 2;
bool isstill(void)
{
	for (int x = 1; x < scale - 1; x++)
		for (int y = 1; y < scale - 1; y++)
		{
			int out = _ruleuse_(cell[x - 1][y - 1], cell[x][y - 1], cell[x + 1][y - 1], cell[x - 1][y], cell[x][y], cell[x + 1][y], cell[x - 1][y + 1], cell[x][y + 1], cell[x + 1][y + 1]);
			if (out != cell[x][y])return false;
		}
	return true;
}
/*********************************
�ı�һλ��������ǰ������̬��Ĳ
**********************************/
int change(int x, int y)
{
	int prenots = 0;
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++)
		{
			int tx = x + dx;
			int ty = y + dy;
			int out = _ruleuse_(cell[tx - 1][ty - 1], cell[tx][ty - 1], cell[tx + 1][ty - 1], cell[tx - 1][ty], cell[tx][ty], cell[tx + 1][ty], cell[tx - 1][ty + 1], cell[tx][ty + 1], cell[tx + 1][ty + 1]);
			if (out != cell[tx][ty])prenots++;
		}
	int afternots = 0;
	cell[x][y] = !cell[x][y];
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++)
		{
			int tx = x + dx;
			int ty = y + dy;
			int out = _ruleuse_(cell[tx - 1][ty - 1], cell[tx][ty - 1], cell[tx + 1][ty - 1], cell[tx - 1][ty], cell[tx][ty], cell[tx + 1][ty], cell[tx - 1][ty + 1], cell[tx][ty + 1], cell[tx + 1][ty + 1]);
			if (out != cell[tx][ty])afternots++;
		}
	return afternots - prenots;//���衣
}
/*************************
*�Ľ���4�������Ϊ8������
**************************/
bool search(unsigned seeds, int dpth, int qt)
{
	if (isstill() == false) {//�������л��˹����µġ�
		memset(cell, 0, scale * scale * sizeof(char));
	}
	int quit = 0;
	srand(seeds);
	//���ﵽ��������������ʧ��qt��ʱ�˳�:
	for (int depth = 0; depth < dpth && quit < qt;) {

		int nots = 0;
		memcpy(cell_vice, cell, scale * scale * sizeof(char));
		bool ok = false;
		for (int i = 0; i < 64; i++)//��̽������Ϊ64�Ρ�
		{
			int r = rand() % 8;
			switch (r)
			{
			case 0:curx = curx == 2 ? scale - 3 : curx - 1; break;
			case 1:cury = cury == 2 ? scale - 3 : cury - 1; break;
			case 2:curx = curx == scale - 3 ? 2 : curx + 1; break;
			case 3:cury = cury == scale - 3 ? 2 : cury + 1; break;
			case 4:curx = curx == 2 ? scale - 3 : curx - 1; cury = cury == 2 ? scale - 3 : cury - 1; break;
			case 5:curx = curx == 2 ? scale - 3 : curx - 1; cury = cury == scale - 3 ? 2 : cury + 1; break;
			case 6:curx = curx == scale - 3 ? 2 : curx + 1; cury = cury == 2 ? scale - 3 : cury - 1; break;
			case 7:curx = curx == scale - 3 ? 2 : curx + 1; cury = cury == scale - 3 ? 2 : cury + 1; break;
			default:throw(-1);
			}//��΢�ţ���֤�仯�ľֲ��ԡ�
			nots += change(curx, cury);
			if (nots == 0) {
				ok = true;
				break;
			}
		}//����̽����Ϊ64����˵��û�гɹ������¿�ʼ��
		if (ok == true) {
			quit = 0;//�ɹ���quitֵ��0��
			depth++;
		}
		else {
			quit++;
			memcpy(cell, cell_vice, scale * scale * sizeof(char));
		}
	}
	if (quit == qt)return false;
	else return true;
}

void stillgrid()
{
	static char buffer[(scale + 4) * (scale + 8) + 16] = { 0 };
	bool s = search(seed, 256, 8192);
	if (s == true)
	{
		int sum = 0;
		for (int x = 0; x < scale; x++)
			for (int y = 0; y < scale; y++)
				sum += cell[x][y];
		if (sum == 0)goto fls;
		memcpy(cell_stil, cell, scale * scale * sizeof(char));
		int i = 0;
		for (int x = 0; x < scale; x++, buffer[i++] = '\n')
			for (int y = 0; y < scale; y++)
				buffer[i++] = cell_stil[y][x] == 0 ? '.' : 'o';//����һ��ʼ�������⡣�����ڶԳƹ��򼯣�����ν���򡣵��Ƕ��ڷǶԳƣ��򡾷�����Ե���Ҫ�ˡ���
		buffer[i++] = '\n', buffer[i++] = '\0';
		ClipBoardAcTextWrite(buffer, i);
	}
	else {
	fls:
		memset(cell, 0, scale * scale * sizeof(char));
		memset(cell_stil, 0, scale * scale * sizeof(char));
		curx = cury = scale / 2;//ʧ�ܣ�һ������Ϊ�㷨���ʺ϶�Ӧ�Ĺ��򼯡�����ֱ����ա�
		ClipBoardAcTextWrite("(fail)", 7);
	}
	BEEP();
}




void gridtoUN(grid gd, UN_structure* UN)
{
	assert(gd != nullptr && UN != nullptr);
	memset(UN, 0, sizeof(UN_structure));
	UN->x = gd->x, UN->y = gd->y;
	char* cl = (char*)gd->cell;
	unsigned int* data = UN->data;
	for (int i = 0; i < scale * scale; i++)
	{
		data[i / 32] |= ((unsigned int)(cl[i] != 0) << (i % 32));
	}
}
bool save_as_file(void)
{
	FILE* fp = fopen(getname("total", ".sve"), "wb");
	if (fp == nullptr)return false;

	int blocks = 0;
	FH_structure FH{};
	UN_structure UN{};
	FH.type = TO;

	FH.tag = NODERTA;
	FH.DX = FH.DY = 0;
	FH.nul = idcode;
	fwrite(&FH, 1, sizeof(FH_structure), fp);

	gridnode* cur = gridhead.next;
	while (cur != nullptr) {
		grid gd = cur->gd;
		assert(gd != nullptr);
		if (gd->lives != 0) {
			gridtoUN(gd, &UN);
			fwrite(&UN, 1, sizeof(UN_structure), fp);
			blocks++;
		}
		cur = cur->next;
	}
	fclose(fp);
	if (blocks == 0) { remove(getname(nullptr, nullptr)); }
	return blocks != 0 ? true : false;
}

/**********************
*�������������еڶ��ѵĺ����������ѵĻ���acirc������
*Ϊ�˱��ֿɿ��ԣ�ʹ������ջ��
*��ͬ��ȫ�ָ��ƣ����������ù�ϣ����еĸ��ơ�
*���ڴ�й©��
*���ظ�����(һ��ʼ���������bug��)��ⷽ�����Ƚ�flood��total��ͬһͼ��Ľ�ȡ��
**************************/
bool save_as_file_flood(int x, int y)
{
	grid gd = search(x, y);
	if (gd == nullptr)return false;

	FILE* fp = fopen(getname("flood", ".sve"), "wb");
	if (fp == nullptr)return false;

	int blocks = 0;
	FH_structure FH{};
	UN_structure UN{};
	FH.type = FL;

	FH.tag = DERTA;
	FH.DX = x, FH.DY = y;
	FH.nul = idcode;
	fwrite(&FH, 1, sizeof(FH_structure), fp);

	coodtable table(512);
	stackunit* stack = nullptr;
	//�����㷨��֤stack����ȡ�����������µ���Ч�ڵ㡣
	push(stack, { x,y });
	table.putin({x,y});
	while (isempty(stack) == false)
	{
		cood cood = pop(stack);
		grid gd = search(cood.x, cood.y);//s������������ġ�
		assert(gd != nullptr);
		if (gd->lives != 0) {
			gridtoUN(gd, &UN);
			UN.x -= x;
			UN.y -= y;//��Ϊ��ƫ�ơ�
			fwrite(&UN, 1, sizeof(UN_structure), fp);
			blocks++;
		}
		for (int i = 0; i < 9; i++)
		{
			if (i != center) {//���ﲻ���÷�Ƭ����(ʵ��������������ȡ���˷�Ƭ����)��
				int scx = cood.x;
				int scy = cood.y;
				MacMove(scx, scy, i);
				if (search(scx, scy) != nullptr && table.isin({scx,scy}) == false)//��ʼ��Ȼд��cood�ˡ�����
				{
					push(stack, { scx,scy });
					table.putin({ scx,scy });
				}
			}
		}
	}

	fclose(fp);
	if (blocks == 0) { remove(getname(nullptr, nullptr)); }
	return blocks != 0 ? true : false;
}


bool save_as_file_ranged(int xf, int yf, int xto, int yto)
{
	FILE* fp = fopen(getname("ranged", ".sve"), "wb");
	if (fp == nullptr)return false;

	if (xf > xto) {
		int med = xf; xf = xto; xto = med;
	}
	if (yf > yto) {
		int med = yf; yf = yto; yto = med;
	}

	int blocks = 0;
	FH_structure FH{};
	UN_structure UN{};
	FH.type = AR;

	FH.tag = DERTA;
	FH.DX = xf, FH.DY = yf;
	FH.nul = idcode;
	fwrite(&FH, 1, sizeof(FH_structure), fp);

	for (int x = xf; x <= xto; x++)
		for (int y = yf; y < yto; y++)
		{
			grid tar = search(x, y);
			if (tar != nullptr && tar->lives != 0)
			{
				gridtoUN(tar, &UN);
				UN.x -= xf;
				UN.y -= yf;
				fwrite(&UN, 1, sizeof(UN_structure), fp);
				blocks++;
			}
		}

	fclose(fp);
	if (blocks == 0) { remove(getname(nullptr, nullptr)); }
	return blocks != 0 ? true : false;
}

/***************
*ֻ�ṩһ�����ļ�������
*������tag�ĺ�����������Ҫ����tag��⡣
*savekindͬ��
*��������ǷǷ�ֵ�����ֻ�Ƿ���false��
****************/
bool read_from_file(const char* name)
{
	assert(name != nullptr);
	if (gridhead.next != nullptr)return false;
	FILE* fp = fopen(name, "rb");
	if (fp == nullptr)return false;

	FH_structure FH;
	UN_structure UN;
	char cl[scale][scale];
	char* pcl = (char*)cl;
	if (fread(&FH, 1, sizeof(FH_structure), fp) != sizeof(FH_structure)||FH.nul!=idcode) { fclose(fp); return false; }
	int tag = FH.tag;
	if (tag != DERTA && tag != NODERTA) { fclose(fp); return false; }
	int type = FH.type;
	if (type != FL && type != AR && type != TO) { fclose(fp); return false; }
	if (FH.RL.release() == false) { fclose(fp); return false; }
	while (fread(&UN, 1, sizeof(UN_structure), fp) == sizeof(UN_structure))
	{
		for (int i = 0; i < scale * scale; i++)
			pcl[i] = (UN.data[i / scale] & (1 << i % scale)) != 0;
		outerblockset(UN.x, UN.y, cl);
	}
	fclose(fp);
	return true;
}


/********************
*�����Ķ�ȡ������
*����move������
*���������򷵻�true�����򷵻�false��
*���Խ�˼���һ���������ѯ�����������ԡ�
*********************/
inline char _sig_(char s)
{
	switch (s)
	{
	case ' ':case '\t':case '.':case '\r':return 0;
	case '\n':return '\n';
	default:return 1;
	}
}
struct celllocation
{
	int x, y;
	int dx, dy;
};
bool rightmove(celllocation* place)
{
	if (place->dx == scale - 1) { place->x += 1; place->dx = 0; return true; }
	else { place->dx += 1; return false; }
}
bool downmove(celllocation* place)
{
	if (place->dy == scale - 1) { place->y += 1; place->dy = 0; return true; }
	else { place->dy += 1; return false; }
}






#define DEFAULT 1
#define maxnum 2048

//noneed.
//#define NOP 16
#define END 17
#define NUMERR 18
#define SBLERR 19
struct signal
{
	int signal;//0 1 \n stop aboves
	int number;//number of 0/1 bit.
};
//b==0
//o==1
//$=='\n'
//number
//\n==ignore
struct signal _decode_(char*& buffer)
{
	int num = -1;
	for (;;)
	{
		int s = *buffer;
		buffer++;
		switch (s)
		{
		case 'x':case 'X':case '#':
		{
			for (;;)
			{
				int s = *buffer;
				buffer++;
				if (s == '\0')return { END,-1 };
				else if (s == '\n')
				{
					break;
				}
			}
			break;
		}
		case 'b':case 'B':return { 0,num == -1 ? DEFAULT : num };
		case 'o':case 'O':return { 1,num == -1 ? DEFAULT : num };
		case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
		{
			if (num == -1) { num = s - '0'; }
			else {
				num = num * 10 + s - '0';
			}
			if (num <= maxnum)break;
			else return{ NUMERR,-1 };
		}
		case '$':return { '\n',num == -1 ? DEFAULT : num };//����Ҳ����������
		case '!':case '\0':return { END,-1 };
		case '\n':case '\r':break;
		default:return { SBLERR,-1 };
		}
	}
}
bool iscompact(char* buffer)
{
	assert(buffer != nullptr);
	char* test = buffer;
	struct signal sig;
	for (;;)
	{
		sig = _decode_(test);
		if (sig.signal == END)return true;//white list:
		else if (sig.signal != 0 && sig.signal != 1 && sig.signal != '\n')return false;
	}
}




#define maxpaste (1024*256)
#define buffersize (maxpaste+1)
char buffer[buffersize];
/**************************
*TODO:ͳһ���
*������������
*���������������һ����Ч�ˡ�
*�Ż�Ч�ʣ�����move���������ԡ�
**************************/
bool patternpaste(int x, int y, int dx, int dy)
{
	if (dx < 0 || dy < 0 || dx >= scale || dy >= scale) { return false; }
	size_t size = getclipsize();//size������β��'\0',������buffersize�ȣ�
	if (size > buffersize) { return false; }
	if (ClipBoardAcTextRead(buffer, size) != true) { return false; }

	buffer[maxpaste] = '\0';

	struct celllocation place;
	place.x = x;
	place.y = y;
	place.dx = dx;
	place.dy = dy;

	if (iscompact(buffer) == false)
	{
		int i = 0;
		struct celllocation current = place;
		for (;;)
		{
			if (buffer[i] == '\0')break;
			else {
				char as = _sig_(buffer[i]);
				if (as == 0 || as == 1)
				{//û�г������������������ԡ�
					outerset(current.x, current.y, current.dx, current.dy, as);
					rightmove(&current);
				}
				else {
					downmove(&place);
					current = place;
				}
			}
			i++;
		}
	}
	else {
		struct signal sig;
		char* read = buffer;
		struct celllocation current = place;
		for (;;)
		{
			sig = _decode_(read);
			switch (sig.signal)
			{
			case 0:
				assert(sig.number >= 0 && sig.number <= maxnum);
				for (int i = 0; i < sig.number; i++) {
					outerset(current.x, current.y, current.dx, current.dy, 0);
					rightmove(&current);
				}
				break;
			case 1:
				assert(sig.number >= 0 && sig.number <= maxnum);
				for (int i = 0; i < sig.number; i++) {
					outerset(current.x, current.y, current.dx, current.dy, 1);
					rightmove(&current);
				}
				break;
			case '\n':
				for (int i = 0; i < sig.number; i++) {
					downmove(&place);
				}
				current = place;
				break;
			case END:
				return true;
			default: { throw(-1); }
			}
		}
	}
	return true;
}



void blockandcell(long long* blocks, long long* cells)
{
	assert(blocks != nullptr && cells != nullptr);
	struct gridnode* cur = gridhead.next;
	while (cur != nullptr) {
		(*blocks)++;
		(*cells) += cur->gd->lives;
		cur = cur->next;
	}
}
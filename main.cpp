#if 1
/*****************************
*2020/6/16重构。
*改善了操作，优化了某些代码的风格。
*其实，用一个方法可以大大改进绘图效率。
*即指针板。这会导致大规模的代码调整，所以略。
*作为area的辅助功能，提供无限空间的绘图的功能。
******************************/
#include<time.h>//为了实现流畅的鼠标操作。
#include"rule.h"
#include"area.h"
#pragma warning(disable:4244)
DWORD(*p)[pwidth];

/* ps:其实运行大概500万个元胞时，就比较慢的了。*/
const char* proginfo = 
"这是一个实现无限空间(1)元胞运算(2)的程序(3)。\n"
"程序由三个界面组成，元胞计算界面(参考2)，BS规则编辑界面(参考2)，以及512规则编辑界面(参考2)。\n"
"其中元胞计算界面中有两种执行模式，自动模式和控制模式，分别支持不同的操作。打开程序的方法包括\n"
"两种(5)，但都会导致进入计算界面。\n"
"下面将直接介绍计算界面中的操作方法，在此之前请参考界面及其参数的介绍(4)。/n"
"\n"
"在两种模式中，以下操作都是相同的：\n"
">>>窗口操作：\n"
"wasd键调节窗口中心位置，qe键调节窗口大小，h键使窗口的尺寸和中心位置恢复为初始值。\n"
"窗口中心默认绘制一个小十字，0键可以开启和关闭这个功能。\n"
">>>运算节奏操作：\n"
"123键调节延时，其中3键使延时数归为1。\n"
"zxc键调节绘图频率，其中c键使绘图频率归为1。\n"
">>>清理操作：Q键，清空空间中的元胞。\n"
"保存操作：OKLP键(5)。\n"
">>>粘贴板操作：\n"
"I键，向粘贴板发送这份信息。(会响铃(11)。)\n"
"G键，向粘贴板发送一块现有规则下的静体(6)(会响铃)，可能会搜索失败(7)。\n"
"Z键：从粘贴板中粘贴元胞(6)。(失败时会响铃。)\n"
">>>其他操作：\n"
"这些操作均是在中规则编辑面板正式实现的，大多数会导致响铃(11)。\n"
"+键，使规则集调整为白板规则集(8)(令参考2)。\n"
"%键：复制规则到粘贴板上(9)。\n"
"^键：从粘贴板中读取规则(9)。\n"
"&键：载入生命游戏规则集(2)。\n"
"*键：清空规则集(10)。\n"
"\n"
"二者的区别在于：\n"
"鼠标机制不同。在自动模式下，鼠标左键可以实现窗口的拖拽，滚轮调节窗口大小。在控制模式下，鼠标\n"
"左右键实现绘图功能(要求窗口尺寸不大于768)，滚轮调节窗口大小。\n"
"自动模式按f键和空格键进入控制模式。控制模式下按f键返回自动模式，按空格键执行一步运算。两个模\n"
"式下均按enter键进入规则编辑界面。从规则界面返回，统一返回控制界面。\n"
"\n"
"关于规则界面，可以参考界面内部的提示进行操作。\n"
"\n"
"\t\t\t\t\t\t\t2020/6/16 作者：irmae\n"
"\n"
"<<<*1>>>\n"
"实际上是(rangeof int)*32范围的空间。\n"
"<<<*2>>>\n"
"关于元胞机的介绍，这里就不详细展开了，仅简要地说明一些程序中实现的元胞机的性质，但还是力求使\n"
"对元胞机不了解的读者理解其含义。\n"
"在这个程序中，每个元胞有两个可能的状态：0态和1态。在一轮计算中，程序获得元胞周围的8个元胞，即\n"
"XXX\n"
"XSX\n"
"XXX,包括中心元胞S在内，取这一组元胞的总的状态(后文称之为环境)，来决定中心元胞下一步的状态(0或1)。\n"
"可以以非常自然的方式对其进行编码，假设一个环境为\n"
"qwe\n"
"asd\n"
"zxc，(i∈{0,1})可以直接视为小端二进制数字qweasdzxc。例如\n"
"010\n"
"110\n"
"001可以编码为010110001(小端二进制)。在这个例子下，可以称之为状态282(大端十进制，或者说普通\n"
"的十进制数),即2^1+2^3+2^4+2^8。在这个状态下，假如中心元胞变为j(∈{0,1}),则记作state[282]->i。\n"
"称state[qweasdzxc]->j为一个规则单元。总共有2^9=512种环境，state[i]->j，i∈{0,1,...,511},\n"
"j∈{0,1}，就描述了这个(中心)元胞的所有可能的活动，称之为这个元胞的规则集(以下部分为了作区别，\n"
"称之为512规则集)。\n"
"在现实的实现中，规定空间中所有元胞共用同一个规则集。基于相同的方法，一个512规则集相当于一个\n"
"512位二进制数。可以很容易地用一个512字节的数组来存储它。在这种架构中，总共有2^512种512规则集，\n"
"而具体到实现上，这个程序仅支持了2^511种可能，其中state[0]->1是被禁止的。\n"
"以上就较为广义地介绍了元胞有两种可能状态，分布在二维方形网格空间，环境恒为相邻的8个单元，且\n"
"所有元胞共用规则集的情形。而这就是程序所实现的运算。在这个框架下，下边将介绍更为普及的BS规则\n"
"集，其中就包括著名的生命游戏(Game of Life)。\n"
"BS规则集要求计算不包括自身的8个相邻元胞中状态为1的元胞(称为活胞)的数量，以此来决定中心元胞转\n"
"化为什么状态。称0->1为“出生(B)”，1->0为死亡，1->1为“存活(S)”。将导致出生的总数和导致存活的\n"
"总数分别列出就构成了BS规则体系，在这样的规定下，生命游戏的规则集为B23/S3。用以下的表示方法，\n"
"或许能更清晰地说明情况：\n"
"B&&S:0->1,1->1。\n"
"B:0->1,1->0。\n"
"S:0->0,1->1。\n"
"无:0->0,1->0。\n"
"BS规则集是的512规则集的子集。之所以这么说，是因为BS规则集总可以翻译为512规则集。这里不列出具体\n"
"方法，仅仅在此指出，生命游戏可以翻译为：\n"
"00000001000101100001011101111110000101100110100001111110111010000001011001101000011111101\n"
"11010000110100010000000111010001000000000010110011010000111111011101000011010001000000011\n"
"10100010000000011010001000000011101000100000001000000000000000100000000000000000010110011\n"
"01000011111101110100001101000100000001110100010000000011010001000000011101000100000001000\n"
"00000000000010000000000000000110100010000000111010001000000010000000000000001000000000000\n"
"0001000000000000000100000000000000000000000000000000000000000000000\n"
"特别的，程序的默认初始规则就是生命游戏。\n"
"将BS规则集稍微广义化(当然还是512规则集的子集)，在计算相邻活胞数(包括自己)时统一定义一个掩码，\n"
"iopjklbnm,每位掩码为0或1。对于\n"
"qwe\n"
"asd\n"
"zxc的环境，活胞数计算为i*q+o*w+p*e+...，并在这之后采用BS规则。程序支持广义的编码以及这种带掩码的\n"
"(更广义的)BS规则集的编码。实际上，程序内部统一采用的是512规则集，在编辑BS规则的同时，程序会将之\n"
"翻译为512规则集。由于这个性质，在规则集页面切换时，BS界面切换到512界面不会造成损失，但是从512界面\n"
"返回会将规则切换到最近使用的BS规则上，从而可能会造成规则损失。\n"
"另外有一个有趣的规则集，它支持在元胞空间中直接进行绘图。那就是state[qweaXdzxc]->X。具体的调用方法\n"
"参见正文。\n"
"<<<*3>>>\n"
"正式的元胞机程序推荐Golly。\n"
"<<<*4>>>\n"
"称计算界面左边的部分为\"窗口\"，右边为信息栏。信息栏中的参数从上到下包括：\n"
"代数，即第几次计算。\n"
"区块数，每个区块容纳32*32的元胞，作为程序维护空间的单元。程序动态维护一个包含元胞的区块集合。在绘\n"
"图中，包含区块1值元胞的区块和不包含的区块的绘制方法不同。\n"
"元胞数，指的是值为1的元胞的数量总数。\n"
"窗口尺寸：窗口对应于几乘几个元胞的空间。当尺寸超过768时，含有元胞的区块用浅色块代替。窗口尺寸可能的\n"
"取值为96，192，384，768，1536，3072，6144，12288，24576。\n"
"中心位置：指的是窗口的中心指向的区块的坐标。\n"
"延时：在自动模式下，程序每连续计算i次后绘图，并sleep一段时间，以毫秒计。\n"
"绘图频率：i值。最大值为128。\n"
"<<<*5>>>\n"
"这个程序的保存机制,包括图像保存，程序自定义的文件格式，以及元胞片的粘贴。\n"
"在计算界面下，P键将窗口部分截图并存储为PNG文件并响铃。\n"
"程序自定义了sve文件格式，sve文件包含有元胞空间的规则集；并定义了三种保存模式，分别为全体保存(O键)，\n"
"floodfill保存(L键)，以及区块截取保存(K键)，分别适用于不同的保存目的。这都是在计算界面下实现的。\n"
"\n"
"O键将直接保存元胞空间为total-(...).sve。K/L键将使程序停止计算，并等待操作。floodfill保存机制保存\n"
"鼠标点击处的区块及其相邻(周围8个区块均算)的所有区块，将保存为flood-(...).sve。区块保存机制保存鼠\n"
"标划出的方块所覆盖到的区块，并保存为range-(...).sve。KL两种功能下，键盘的qweasd键依然生效，且均可以\n"
"按鼠标右键取消操作。以上三种功能不会保存没有元胞的文件，且无论成功与否，均会触发响铃。\n"
"\n"
"Z键实现粘贴机制。程序只接受文本粘贴，可以识别传统rle格式(要求rle数字不超过2048),对于不能识别的机制，\n"
"将统一按照默认模式识别。按过Z键之后，程序停止计算，使用者使用鼠标指定粘贴的左上角，要求窗口尺寸不大\n"
"于768，以精确到元胞的位置。可以按右键取消粘贴。\n"
"\n"
"rle格式的例子：(只支持bo版本)\n"
"# ark1 -- 16 cells, stabilizes at 736692 gens, found by Nick Gotts.\n"
"x = 32, y = 29, rule = B3/S23\n"
"27bo$28bo$29bo$28bo$27bo$29b3o20$oo$bbo$bbo$3b4o!\n"
"\n"
"默认格式下有效的例子：(daynight规则(B3678/S34678))\n"
" ........OO..................................\n"
" .........O..................................\n"
" .....O...O...OO.............................\n"
" ....OO.OOOOOOOO.........OO..................\n"
" ...OOOOOOOOOOO.O...OO.OOOOOO.............OO.\n"
" ..OOOOOOOOOOOOO..OOOOOOOOOOO.......O....OOO.\n"
" .OOOOOOOOOOOOOOOOOOOOOOOOOO..........OOOOO.O\n"
" ..OOOOOOOOOOOOO..OOOOOOOOOOO.......O....OOO.\n"
" ...OOOOOOOOOOO.O...OO.OOOOOO.............OO.\n"
" ....OO.OOOOOOOO.........OO..................\n"
" .....O...O...OO.............................\n"
" .........O..................................\n"
" ........OO..................................\n"
"程序将' '，'\\t'，'.'，'\\r'接收为0胞，将'\\n'接收为换行，其余处理为1胞。\n"
"若粘贴板为空，或粘贴板顶部格式不为文本格式，则会导致粘贴失败并响铃。\n"
"以上介绍的机制，除了元宝片粘贴是在粘贴失败时响铃外，其余部分均总是响铃。\n"
"\n"
"sve文件打开的方式为拖拽打开。这种打开方式使程序直接进入控制模式，直接打开程序，程序会进入自动模式，以\n"
"生命游戏为初始规则集，并分配两个blinker(生命游戏的术语)。\n"
"<<<*6>>>\n"
"静体：在特定规则下，经过一轮计算后保持原样。\n"
"<<<*7>>>\n"
"程序采取的搜索机制不一定生效，哪怕是对于存在静体的规则集。若失败，则向粘贴板发送一个(fail)。\n"
"<<<*8>>>\n"
"白板规则集下，任何分布均属于静体，这使得程序可以直接维护一份没有长宽的图片。\n"
"<<<*9>>>\n"
"是以512规则集的形式传递的。\n"
"<<<*10>>>\n"
"state[i]->0。\n"
"<<<*11>>>\n"
"响铃函数的分布十分混乱，这个文档可能会出现介绍错误。\n";

void sendproginfo()
{
	ClipBoardAcTextWrite(proginfo, strlen(proginfo) + 1);
	BEEP();
}
long long generations = 0;
long long Blocks = 0;
long long Cells = 0;
int startx = 0;
int starty = 0;//左上角的坐标。
#define maxsize (scale*8)
#define minsize (1)
int scalesize = scale * 4;
inline void sizebigger(void) { 
	if (scalesize != scale * 8) {
		startx += PIC / (4 * scalesize);
		starty += PIC / (4 * scalesize);
	}
	scalesize = scalesize == maxsize ? maxsize : scalesize * 2; 
}
inline void sizesmaller(void) { 
	if (scalesize != 1) {
		startx -= PIC / (2 * scalesize);
		starty -= PIC / (2 * scalesize);
	}
	scalesize = scalesize == minsize ? minsize : scalesize / 2;
}
#define maxdelay 1024
int delay = 1;
inline void delaysmaller() { delay = delay == 0 ? 0 : delay - 1; }
inline void delaybigger() { delay = delay == maxdelay ? maxdelay : delay + 1; }
#define maxfreq 128
int acircfrequency = 1;
inline void acfsmaller(void) { acircfrequency = acircfrequency == 1 ? 1 : acircfrequency - 1; }
inline void acfbigger(void) { acircfrequency = acircfrequency == maxfreq ? maxfreq : acircfrequency + 1; }


void pftextxy(int x, int y, const WCHAR* fmt, ...)
{
	static WCHAR buffer[512] = { 0 };
	va_list vp;
	va_start(vp, fmt);
	_vsnwprintf(buffer, 512, fmt, vp);//...//n为buffer大小。
	outtextxy(x, y, buffer);
	va_end(vp);
}
void info(void)
{
	int i = 0;
	int x = PIC;
	int y = 0;
	pftextxy(x, y + 16 * i++, L"generations:%-20lld", generations);
	pftextxy(x, y + 16 * i++, L"blocks:%-20lld", Blocks);
	pftextxy(x, y + 16 * i++, L"cells:%-20lld", Cells);
	pftextxy(x, y + 16 * i++, L"scopesize(qe):%-12d", scale * (PIC / scalesize));
	pftextxy(x, y + 16 * i++, L"center location(wasd):x:%d y:%d (/%d)               ", startx + PIC / (2 * scalesize), starty + PIC / (2 * scalesize), scale);
	pftextxy(x, y + 16 * i++, L"delay(ms)(zx):%-12d", delay);
	pftextxy(x, y + 16 * i++, L"portrait frequency(12):1/%-12d", acircfrequency);
	pftextxy(x, y + 16 * i++, L"For more pftextxy,type 'I(upper(i))'.");
}
inline void extrainfo(const WCHAR* matter=nullptr)
{
	const int x = PIC;
	const int y = 16 * 9;
	if (matter == nullptr)
		for (int dy = y; dy < y + 16; dy++)
			for (int dx = PIC; dx < pwidth; dx++)
				p[dy][dx] = BLACK;
	else {
		outtextxy(x, y, matter);
	}
}


//在哈希下，只要块数不是太高，效率并不低。
bool centercross = true;
void outportrait(void) {

	int bitsize = scalesize / scale;
	int gridper = PIC / scalesize;//每行的grid数。

	if (scalesize >= scale) {//两种读取方法。
		for (int y = 0; y < gridper; y++)
			for (int x = 0; x < gridper; x++)
			{
				int pix = x * scalesize;
				int piy = y * scalesize;//每个grid网格的起使坐标。
				grid seek = search(startx + x, starty + y);
				if (seek != NULL && seek->lives != 0) {
					char(*cl)[scale] = seek->cell;
					for (int cy = 0; cy < scale; cy++) 
						for (int cx = 0; cx < scale; cx++) {
							DWORD color = cl[cy][cx] == 1 ? WHITE : 0;

							int ppix = pix + cx * bitsize;
							int ppiy = piy + cy * bitsize;
							int ppixto = ppix + bitsize;
							int ppiyto = ppiy + bitsize;

							for (int py = ppiy; py < ppiyto; py++)
								for (int px = ppix; px < ppixto; px++) {
									p[py][px] = color;
								}
						}
				}
				else {
					DWORD color = seek != NULL ? 0x3f : 0x6f;
					int pxto = pix + scalesize;
					int pyto = piy + scalesize;
					for (int py = piy; py < pyto; py++)
						for (int px = pix; px < pxto; px++) {
							p[py][px] = color;
						}
				}
			}
	}
	else {//<scale。
		for (int y = 0; y < gridper; y++)//交换。
			for (int x = 0; x < gridper; x++) {
				int pix = x * scalesize;
				int piy = y * scalesize;//每个grid网格的起使坐标。
				grid seek = search(startx + x, starty + y);
				DWORD color;
				if (seek != NULL)color = seek->lives != 0 ? 0xff : 0x2f;
				else color = 0x4f;
				int pxto = pix + scalesize;
				int pyto = piy + scalesize;
				for (int py = piy; py < pyto; py++)
					for (int px = pix; px < pxto; px++) {
						p[py][px] = color;
					}
			}
	}
	if (centercross == true) {
		const DWORD crosscol = 0xff0000;
		p[PIC / 2 - 2][PIC / 2] = crosscol;
		p[PIC / 2 - 1][PIC / 2] = crosscol;
		p[PIC / 2 - 0][PIC / 2] = crosscol;
		p[PIC / 2 + 1][PIC / 2] = crosscol;
		p[PIC / 2 + 2][PIC / 2] = crosscol;

		p[PIC / 2][PIC / 2 - 2] = crosscol;
		p[PIC / 2][PIC / 2 - 1] = crosscol;
		p[PIC / 2][PIC / 2 + 1] = crosscol;
		p[PIC / 2][PIC / 2 + 2] = crosscol;
	}
}
/****************************
*重构：令函数自己负责值域的
*正确性。
****************************/
void setandcolor(int mx, int my, int setas)
{
	if (mx >= 0 && my >= 0 && mx < PIC && my < PIC && scalesize >= scale) {
		int bitsize = scalesize / scale;
		int per = bitsize * scale;
		int dx = mx % per;
		int dy = my % per;
		int x = mx / per;
		int y = my / per;
		outerset(startx + x, starty + y, dx / bitsize, dy / bitsize, setas);
		int fpx = bitsize * (mx / bitsize);
		int fpy = bitsize * (my / bitsize);
		DWORD color = setas == 0 ? 0 : WHITE;
		for (int pdy = 0; pdy < bitsize; pdy++)
			for (int pdx = 0; pdx < bitsize; pdx++)
				p[fpy + pdy][fpx + pdx] = color;
	}
}




void screenshot(void)
{
	IMAGE img(PIC, PIC);
	DWORD* imgp = GetImageBuffer(&img);
	for (int y = 0; y < PIC; y++)
		for (int x = 0; x < PIC; x++)
			imgp[y * PIC + x] = p[y][x];
	saveimage(getwname(L"screenshot", L".png"), &img);
	BEEP();
}
//以为参考系图像坐标系。
//我怀疑是搞反了。但是经过在调用时对调ft，直接正常工作。
int bicoodtodir(int xf, int yf, int xt, int yt)
{
	if (xf == xt && yf == yt)return center;
	else if (xf != xt)
	{
		double k = ((double)yt - yf) / ((double)xt - xf);
		if (-0.414 <= k || k <= 0.414)
		{//tan22.5
			if (xf < xt)return right;
			else return left;
		}
		else if (k <= -2.414 || k >= 2.414)
		{//tan67.5
			if (yf < yt)return down;
			else return up;
		}
		else{
			if (xf < xt)
			{
				if (yf < yt)return rightdown;
				else return rightup;
			}
			else {
				if (yf < yt)return leftdown;
				else return leftup;
			}
		}
	}
	else {
		if (yf < yt)return down;
		else return up;
	}
}
void scopemove(int pos)
{
	switch (pos)
	{
	case up:
		starty -= scalesize >= scale ? 1 : scale / scalesize;break;
	case down:
		starty += scalesize >= scale ? 1 : scale / scalesize;break;
	case left:
		startx -= scalesize >= scale ? 1 : scale / scalesize;break;
	case right:
		startx += scalesize >= scale ? 1 : scale / scalesize;break;
	case leftup:
		startx -= scalesize >= scale ? 1 : scale / scalesize;
		starty -= scalesize >= scale ? 1 : scale / scalesize;break;
	case leftdown:
		startx -= scalesize >= scale ? 1 : scale / scalesize;
		starty += scalesize >= scale ? 1 : scale / scalesize;break;
	case rightup:
		startx += scalesize >= scale ? 1 : scale / scalesize;
		starty -= scalesize >= scale ? 1 : scale / scalesize;break;
	case rightdown:
		startx += scalesize >= scale ? 1 : scale / scalesize;
		starty += scalesize >= scale ? 1 : scale / scalesize;break;
	case center:break;
	default:throw(-1);
	}
}


void paste_interface(void);
void floodsave_interface(void);
void rangesave_interface(void);

void commonop(int sig)
{
	bool isportrait = false;
	bool isinfo = false;
	switch (sig)
	{
	case 'h':startx = 0, starty = 0, scalesize = scale * 4;isportrait = isinfo = true;break;
	case 'w':scopemove(up); isportrait = isinfo = true; break;
	case 's':scopemove(down);isportrait = isinfo = true;break;
	case 'a':scopemove(left);isportrait = isinfo = true;break;
	case 'd':scopemove(right);isportrait = isinfo = true;break;
	case 'q':sizesmaller();isportrait = isinfo = true;break;
	case 'e':sizebigger();isportrait = isinfo = true;break;
	case '0':centercross = !centercross; break;

	case '1':delaysmaller();isinfo = true;break;
	case '2':delaybigger();isinfo = true;break;
	case '3':delay = 1; isinfo = true; break;
	case 'z':acfsmaller();isinfo = true;break;
	case 'x':acfbigger();isinfo = true;break;
	case 'c':acircfrequency = 1; isinfo = true; break;

	case 'O':save_as_file();BEEP(); break;
	case 'P':screenshot(); break;
	case 'Q':clearall(); Blocks = 0, Cells = 0, isportrait = isinfo = true; break;
	case 'G':stillgrid();break;
	case 'I':sendproginfo();break;
	case 'Z':paste_interface();break;
	case 'K':rangesave_interface();break;
	case 'L':floodsave_interface();break;

	case '%':copyrule(); break;
	case '^':pasterule(); break;
	case '&':GAMIZE(); BEEP(); break;
	case '*':CLIZE(); BEEP(); break;//照理是不该出现的。但是如果真的有需求的话，也可以实现。
	case '+':BOARDIZE(); BEEP(); break;

	default:break;
	}
	if (isportrait == true)outportrait();
	if (isinfo == true)info();
}



/***************
经常成对出现：
***************/
#define POR_IN() (outportrait(),info())
inline void flushcontrol(void)
{
	while (_kbhit())_getch();
	FlushMouseMsgBuffer();
}
int main(int argc, char* argv[])
{
	srand(time(nullptr));
	initgraph(pwidth, pheight);
	p = (DWORD(*)[pwidth])GetImageBuffer();

	//暂时的安排：
	clearall();
	bool loadfile = false;
	if (argc == 2 && read_from_file(argv[1]) == true) { loadfile = true; }
	else {
		GAMIZE();
		/*分配两个闪闪。*/
		outerset(startx + 2, starty + 2, scale / 2 - 1, scale / 2, 1);
		outerset(startx + 2, starty + 2, scale / 2, scale / 2, 1);
		outerset(startx + 2, starty + 2, scale / 2 + 1, scale / 2, 1);

		outerset(startx + 3, starty + 3, scale / 2 - 1, scale / 2, 1);
		outerset(startx + 3, starty + 3, scale / 2, scale / 2, 1);
		outerset(startx + 3, starty + 3, scale / 2 + 1, scale / 2, 1);
	}
	POR_IN();
	if (loadfile == true)goto stopping;

	clock_t lasttime;//辅助鼠标操作。
	int lastx, lasty;
	bool isactived;
nonstopping:
	//添加拖拽功能。
	lasttime = 0;
	lastx = 0, lasty = 0;
	isactived = false;
	flushcontrol();
	for (;;) {
		for (int i = 0; i < acircfrequency; i++) {
			acirc(&Blocks, &Cells);
			generations++;
		}
		POR_IN();
		while (_kbhit())
		{
			int sig = getch();
			commonop(sig);
			if (sig == ENTER ||sig=='f'|| sig == ' ')//双入单出模式。
			{
				if (sig == ENTER)
				{
					flushcontrol();
					ruleedit();
					POR_IN();
				}
				goto stopping;
			}
		}
		while (MouseHit())
		{
			MOUSEMSG m = GetMouseMsg();
			if (m.wheel < 0) {
				sizesmaller();
			}
			else if (m.wheel > 0)
			{
				sizebigger();
			}//拖拽移动镜头。还是不要支持左右键了。
			else if(m.mkRButton==true){
				if (isactived == false) {
					lastx = m.x, lasty = m.y;
					isactived = true;
					lasttime = clock();
				}
				else {//要大于8，是为了解决"过度灵活"的问题。
					if (clock() - lasttime < 512&&clock()-lasttime>8&&!(lastx==m.x&&lasty==m.y)) {
						scopemove(bicoodtodir(m.x, m.y,lastx, lasty));
					}
					lastx = m.x; lasty = m.y; lasttime = clock();
				}
			}
		}
		if (delay > 0)Sleep(delay);
	}
	/************************************************
	在这之前，实在有必要弄清楚要进行哪些操作。
	难点在于CV机制。
	C：空间的保存。保存至文件。支持三种模式。
	V：从粘贴板中获得元胞图样。
	对应的函数都实现了，而配套的鼠标操作，反而成了最大的困难。
	要实现较好的界面。在rangesave中要实现方框。而方框曾是mandelbrot集合实现中的一大难点。
	*************************************************/
stopping:
	lasttime = 0;//辅助鼠标操作。
	lastx = 0, lasty = 0;//【没有golly的流畅级别。】【在移动时，鼠标线被打断了。】
	isactived = false;
	int lastldown = false;

	int vcalled = false;


	flushcontrol();
	for (;;)
	{
		while (_kbhit())
		{
			int sig = getch();
			commonop(sig);
			switch (sig)
			{
			case 'f':goto nonstopping;//f与 ' '调换。
			case ENTER:
				flushcontrol(),ruleedit(),POR_IN(),flushcontrol();
				break;
			case ' ':
				acirc(&Blocks, &Cells),generations++,POR_IN();
				break;
			default:break;
			}
		}

		while(MouseHit()==true) {
			MOUSEMSG m = GetMouseMsg();
			if (m.wheel < 0) {
				sizesmaller();
				POR_IN();
			}
			else if (m.wheel > 0)
			{
				sizebigger();
				POR_IN();
			}
			else if (m.mkLButton == true || m.mkRButton == true) {
				if (isactived == false) {
					lastx = m.x, lasty = m.y;
					setandcolor(m.x, m.y, m.mkLButton == true ? 1 : 0);
					isactived = true;
					lasttime = clock();
					lastldown = m.mkLButton == true ? 1 : 0;
				}
				else {
					if ((clock() - lasttime) < 100 && lastldown == (m.mkLButton == true)) {
						linefunc(lastx, lasty, m.x, m.y, m.mkLButton == true ? 1 : 0, setandcolor);
						lastx = m.x;
						lasty = m.y;
						lasttime = clock();
					}
					else {
						lastx = m.x, lasty = m.y;
						lastldown = m.mkLButton == true ? 1 : 0;
						setandcolor(m.x, m.y, m.mkLButton == true ? 1 : 0);
						lasttime = clock();
					}
				}
			}
		}
		Sleep(2);
	}
}


#define col WHITE
void movingcross(int x, int y)
{
	p[y][x - 2] ^= col;
	p[y][x - 1] ^= col;
	p[y][x - 0] ^= col;
	p[y][x + 1] ^= col;
	p[y][x + 2] ^= col;

	p[y - 2][x] ^= col;
	p[y - 1][x] ^= col;
	p[y + 1][x] ^= col;
	p[y + 2][x] ^= col;
}
void movingcross(int x, int y, int xto, int yto)
{
	p[y][x - 2] ^= col;
	p[y][x - 1] ^= col;
	p[y][x - 0] ^= col;
	p[y][x + 1] ^= col;
	p[y][x + 2] ^= col;
	p[y - 2][x] ^= col;
	p[y - 1][x] ^= col;
	p[y + 1][x] ^= col;
	p[y + 2][x] ^= col;

	p[yto][xto - 2] ^= col;
	p[yto][xto - 1] ^= col;
	p[yto][xto - 0] ^= col;
	p[yto][xto + 1] ^= col;
	p[yto][xto + 2] ^= col;
	p[yto - 2][xto] ^= col;
	p[yto - 1][xto] ^= col;
	p[yto + 1][xto] ^= col;
	p[yto + 2][xto] ^= col;
}
//反正是最后几个函数了，就不管什么代码的紧凑性，复用性了。
void paste_interface(void)
{
	bool isacrossin = false;
	int lastcrossx = 0, lastcrossy = 0;
	extrainfo(L"paste from clipboard.right click to quit.");
	flushcontrol();
	while (true) {
		bool isportrait = false;
		bool isinfo = false;
		if (_kbhit())//提供一个非常有限的操作集合。
		{
			int sig = _getch();
			switch (sig)
			{
			case'w':scopemove(up); isportrait = isinfo = true; break;
			case 's':scopemove(down); isportrait = isinfo = true; break;
			case 'a':scopemove(left); isportrait = isinfo = true; break;
			case 'd':scopemove(right); isportrait = isinfo = true; break;
			case 'q':sizesmaller(); isportrait = isinfo = true; break;
			case 'e':sizebigger(); isportrait = isinfo = true; break;
			default:break;
			}
		}
		if (isportrait == true) { isacrossin = false; outportrait(); }
		if (isinfo == true)info();
		if (MouseHit()) {
			MOUSEMSG m = GetMouseMsg();
			if (m.mkRButton == true) {
				if (isacrossin == true)
				{
					movingcross(lastcrossx, lastcrossy);
					isacrossin = false;
				}
				break;
			}//右键取消。
			if (isacrossin == false && m.x >= 2 && m.y >= 2 && m.x < PIC - 2 && m.y < pheight - 2) {
				lastcrossx = m.x, lastcrossy = m.y;
				isacrossin = true;
				movingcross(lastcrossx, lastcrossy);
			}
			else if (m.x >= 2 && m.y >= 2 && m.x < PIC - 2 && m.y < pheight - 2) {
				movingcross(lastcrossx, lastcrossy, m.x, m.y);
				lastcrossx = m.x, lastcrossy = m.y;
			}
			if (m.mkLButton == true) {//debug:在这里和rawpaste处添加了安全检测。优化了流程。
				if (scalesize < scale || m.x < 0 || m.y < 0 || m.x >= PIC || m.y >= PIC) { BEEP(); }
				else {
					if (isacrossin == true)
					{
						movingcross(lastcrossx, lastcrossy);
						isacrossin = false;
					}
					int bitsize = scalesize / scale;

					if (patternpaste(startx + m.x / scalesize, starty + m.y / scalesize, (m.x % scalesize) / bitsize, (m.y % scalesize) / bitsize) == false)BEEP();
					blockandcell(&Blocks, &Cells);
					POR_IN();
					break;
				}
			}
		}
		Sleep(1);
	}
	extrainfo();
	flushcontrol();
}
//和上边的东西只有一丁点区别。
void floodsave_interface(void)
{
	bool isacrossin = false;
	int lastcrossx = 0, lastcrossy = 0;
	extrainfo(L"floodfill save.right click to quit.");
	flushcontrol();
	while (true) {
		bool isportrait = false;
		bool isinfo = false;
		if (_kbhit())//提供一个非常有限的操作集合。
		{
			int sig = _getch();
			switch (sig)
			{
			case'w':scopemove(up); isportrait = isinfo = true; break;
			case 's':scopemove(down); isportrait = isinfo = true; break;
			case 'a':scopemove(left); isportrait = isinfo = true; break;
			case 'd':scopemove(right); isportrait = isinfo = true; break;
			case 'q':sizesmaller(); isportrait = isinfo = true; break;
			case 'e':sizebigger(); isportrait = isinfo = true; break;
			default:break;
			}
		}
		if (isportrait == true) { isacrossin = false; outportrait(); }
		if (isinfo == true)info();
		if (MouseHit()) {
			MOUSEMSG m = GetMouseMsg();
			if (m.mkRButton == true) {
				if (isacrossin == true)
				{
					movingcross(lastcrossx, lastcrossy);
					isacrossin = false;
				}
				break;
			}//右键取消。
			if (isacrossin == false && m.x >= 2 && m.y >= 2 && m.x < PIC - 2 && m.y < pheight - 2) {
				lastcrossx = m.x, lastcrossy = m.y;
				isacrossin = true;
				movingcross(lastcrossx, lastcrossy);
			}
			else if (m.x >= 2 && m.y >= 2 && m.x < PIC - 2 && m.y < pheight - 2) {
				movingcross(lastcrossx, lastcrossy, m.x, m.y);
				lastcrossx = m.x, lastcrossy = m.y;
			}
			if (m.mkLButton == true) {//debug:在这里和rawpaste处添加了安全检测。优化了流程。
				if (m.x < 0 || m.y < 0 || m.x >= PIC || m.y >= PIC) { BEEP(); }//这个没有尺寸检测要求。
				else {
					if (isacrossin == true)
					{
						movingcross(lastcrossx, lastcrossy);
						isacrossin = false;
					}

					save_as_file_flood(startx + m.x / scalesize, starty + m.y / scalesize);
					BEEP();
					break;
				}
			}
		}
		Sleep(2);
	}
	extrainfo();
	flushcontrol();
}
//这个就比较的棘手了。
//幸好写过mand程序。
void rangesave_interface(void)
{
	setcolor(WHITE);
	setwritemode(R2_XORPEN);
	bool isrec = false;//有无长方框。
	int recfx = 0, recfy = 0, rectx = 0, recty = 0;	//选区。

	extrainfo(L"ranged save.right click to quit.");
	flushcontrol();	while (true)
	{
		if (MouseHit()) {
			MOUSEMSG m = GetMouseMsg();
			if (m.mkLButton == false && m.uMsg != WM_LBUTTONUP && isrec == true)
			{
				isrec = false;
				rectangle(recfx, recfy, rectx, recty);
			}
			if (m.x >= 0 && m.x < PIC && m.y >= 0 && m.y < PIC)
			{
				switch (m.uMsg)
				{
				case WM_RBUTTONDOWN:
					if (isrec == true) {
						isrec = false;
						rectangle(recfx, recfy, rectx, recty);
					}
					goto out;
				case WM_LBUTTONDOWN:
					if (isrec == false) {
						isrec = true;
						recfx = rectx = m.x;
						recfy = recty = m.y;
						rectangle(recfx, recfy, rectx, recty);
					}
					break;
				case WM_MOUSEMOVE:
					if (isrec == true)
					{
						rectangle(recfx, recfy, rectx, recty);
						rectx = m.x;
						recty = m.y;
						rectangle(recfx, recfy, rectx, recty);
					}
					break;
				case WM_LBUTTONUP:
					if (isrec == true) {
						isrec = false;
						rectangle(recfx, recfy, rectx, recty);
						rectx = m.x;
						recty = m.y;
						//下边是调整方框。
						if (recfx > rectx) { int tmp = recfx; recfx = rectx; rectx = tmp; }
						if (recfy > recty) { int tmp = recfy; recfy = recty; recty = tmp; }
						int xf, yf,xt, yt;
						xf = startx+recfx / scalesize - 1;
						yf = starty+recfy / scalesize - 1;
						xt = startx+rectx / scalesize + 1;
						yt = starty+recty / scalesize + 1;
						save_as_file_ranged(xf, yf, xt, yt);
						BEEP();
						goto out;
					}
					break;
				default:break;
				}
			}
		}
		if (_kbhit())
		{
			bool port = false;
			int sig = _getch();
			switch (sig)
			{
			case'w':scopemove(up); port = true; break;
			case 's':scopemove(down); port = true; break;
			case 'a':scopemove(left); port = true; break;
			case 'd':scopemove(right); port = true; break;
			case 'q':sizesmaller(); port = true; break;
			case 'e':sizebigger(); port = true; break;
			default:break;
			}
			if (port == true)
			{
				if (isrec == true) {
					isrec = false;
					rectangle(recfx, recfy, rectx, recty);
				}
				POR_IN();
			}
		}
		Sleep(2);
	}
out:
	extrainfo();
	flushcontrol();
}
#endif
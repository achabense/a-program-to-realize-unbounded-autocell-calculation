#include"rule.h"
/*****************************************
*2020/6/15重构。
*优化了绘图行为。
*不常用模块，以稳健为主。
******************************************/


const _count_ game_of_life_bool = { 1,1,1,1,0,1,1,1,1 };
const _bs_ game_of_life_bs = { NO,NO,S,BS,NO,NO,NO,NO,NO,NO };

_genrule_ nowrule{};
_count_ nowcount = { 1,1,1,1,0,1,1,1,1 };
_bs_ nowbs = { NO,NO,S,BS,NO,NO,NO,NO,NO,NO };

#define SUMMODE 0
#define GENERALMODE 1
int rulemode = SUMMODE;

bool _translate_(const _count_ count, const _bs_ sb)
{
	if (sb[0] == B || sb[0] == SB)return false;//sum0->1 prohibited.
	for (int i = 0; i < 9; i++)
		if (count[i] != 0 && count[i] != 1)return false;
	for (int i = 0; i < 9; i++)
		if (!(0 <= sb[i] && sb[i] <= 3))return false;
	for (int i = 0; i < 512; i++)
	{
		int self = (i & (1 << center)) != 0;
		int sum = 0;
		for (int k = 0; k < 9; k++)
		{
			sum += count[k] * ((i & (1 << k)) != 0);
		}
		switch (sb[sum])
		{
		case NO:nowrule[i] = 0; break;
		case B:nowrule[i] = !self; break;
		case S:nowrule[i] = self; break;
		case SB:nowrule[i] = 1; break;
		default:throw(-1);
		}
	}
	return true;
}
void GAMIZE(void)
{
	for (int i = 0; i < 9; i++)
		nowcount[i] = game_of_life_bool[i];
	for (int i = 0; i < 10; i++)
		nowbs[i] = game_of_life_bs[i];
	_translate_(nowcount, nowbs);
	rulemode = SUMMODE;
}
void CLIZE(void)
{
	for (int i = 0; i < 10; i++)
		nowbs[i] = 0;
	_translate_(nowcount, nowbs);
}
void BOARDIZE(void)
{
	for (int i = 0; i < 512; i++)
	{
		int self = (i & (1 << center)) != 0;
		nowrule[i] = self;
	}
	rulemode = GENERALMODE;
}

/************************
*不负责边界检查。
************************/
inline void square(int x, int y, int r, DWORD color)
{
	static const DWORD edgecolor = 0x444444;
	int xto = x + r - 1, yto = y + r - 1;
	for (int dy = y + 1; dy < yto; dy++)
		for (int dx = x + 1; dx < xto; dx++)
			p[dy][dx] = color;
	for (int dx = x; dx <= xto; dx++)
		p[y][dx] = edgecolor, p[yto][dx] = edgecolor;
	for (int dy = y; dy <= yto; dy++)
		p[dy][x] = edgecolor, p[dy][xto] = edgecolor;
}
/********************
*只负责绘图，不清屏。
*由于参数是一点点调出来的，因此结构惨不忍睹。
*********************/
void sumpage(void)
{
	const int r = 64;
	DWORD sbcolor[4];
	sbcolor[SB] = 0xffffff, sbcolor[S] = 0xff, sbcolor[B] = 0xff00, sbcolor[NO] = 0x444444;//语义依赖而非数据依赖。
	DWORD boolcolor[] = { 0x44,0xff };

	const int startx = PIC / 4, starty = 32+PIC / 8;
	outtextxy(startx, starty - 32, L"<<SUMMUP:>>");
	outtextxy(startx, starty - 16, L"<keys:qweasdzxc>");
	for (int j = 0; j < 9; j++)
	{
		square(startx + r * (j % 3), starty + r * (j / 3), r, boolcolor[nowcount[j]]);
	}

	outtextxy(startx + r * 4, starty +2* r - 16, L"      NO          YES");
	square(startx + r * 4, starty +2* r , r, boolcolor[0]);
	square(startx + r * 5, starty +2* r , r, boolcolor[1]);

	const int textx = PIC / 4, texty = starty+3*r+16;
	int i = 0;
	outtextxy(textx, texty + 16 * i++, L"<<B/S/BS/D MODE:>>");
	outtextxy(textx, texty + 16 * i++, L"<keys:0123456789>");
	outtextxy(textx, texty + 16 * i++, L">>B:0->1,1->0");
	outtextxy(textx, texty + 16 * i++, L">>S:0->0,1->1");
	outtextxy(textx, texty + 16 * i++, L">>BS:0->1,1->1");
	outtextxy(textx, texty + 16 * i++, L">>NO:0->0,1->0");

	const int lsdx = PIC / 4, lsdy = texty + r + 48;
	outtextxy(lsdx + r * 4, lsdy - r - 32, L"      NO           B              S             BS");
	square(lsdx + r * 4, lsdy - r-16, r, sbcolor[NO]);
	square(lsdx + r * 5, lsdy - r-16, r, sbcolor[B]);
	square(lsdx + r * 6, lsdy - r-16, r, sbcolor[S]);
	square(lsdx + r * 7, lsdy - r-16, r, sbcolor[BS]);
	outtextxy(lsdx, lsdy, L"       0              1              2              3              4              5              6              7              8              9");
	for (int j = 0; j < 10; j++)
		square(lsdx + r * j, lsdy+16, r, sbcolor[nowbs[j]]);


	const int othertextx = PIC / 4, othertexty = lsdy + r+32;
	int j = 0;
	outtextxy(othertextx, othertexty + 16 * j++, L"<<other keys:>>");
	outtextxy(othertextx, othertexty + 16 * j++, L">>u to clear up.");
	outtextxy(othertextx, othertexty + 16 * j++, L">>o to load White Board rule.");
	outtextxy(othertextx, othertexty + 16 * j++, L">>p to load Game of Life rule.");
	outtextxy(othertextx, othertexty + 16 * j++, L">>C to copy the rule to the clipboard.");
	outtextxy(othertextx, othertexty + 16 * j++, L">>V to paste the rule from the clipboard.");
	outtextxy(othertextx, othertexty + 16 * j++, L">>enter to go to general edit page.");
	outtextxy(othertextx, othertexty + 16 * j++, L">>' ' to quit.");
}

void _rulelil_(int x, int y, int r, int icode)
{
	DWORD color[2] = { 0x444444,0xffffff };
	for (int j = 0; j < 9; j++)
		square(x + r * (j % 3), y + r * (j / 3), r, color[(icode & (1 << j)) != 0]);
	x += r * 3;
	for (int j = 0; j < 9; j++)
		square(x + r * (j % 3), y + r * (j / 3), r, j == 4 ? (icode != 0 ? color[nowrule[icode]] : 0xff0000) : 0);
}
#define ru 24
void generalpage(int page)
{
	int startx = ru / 2;
	int starty = ru / 2;
	WCHAR buffer[64] = { 0 };
	for (int i = 0; i < 32; i++)
	{
		_rulelil_(startx + 6 * ru * (i % 4), starty + 4 * ru * (i / 4), ru, page * 32 + i);
	}
	int i = 0;
	swprintf(buffer, L"Page %d(%d ~ %d)            ", page, page * 32, page * 32 + 31);
	outtextxy(ru * 26, ru / 2 + 16 * i++, buffer);
	i++;
	outtextxy(ru * 26, ru / 2 + 16 * i++, L"<<keys:>>");
	outtextxy(ru * 26, ru / 2 + 16 * i++, L">>L/R click to edit ruleunit.");
	outtextxy(ru * 26, ru / 2 + 16 * i++, L">>ad/mouse scroll to flip page.");
	outtextxy(ru * 26, ru / 2 + 16 * i++, L">>u to clear up.");
	outtextxy(ru * 26, ru / 2 + 16 * i++, L">>o to load White Board rule.");
	outtextxy(ru * 26, ru / 2 + 16 * i++, L">>p to load Game of Life rule.");
	outtextxy(ru * 26, ru / 2 + 16 * i++, L">>C to copy the rule to the clipboard.");
	outtextxy(ru * 26, ru / 2 + 16 * i++, L">>V to paste the rule from the clipboard.");
	outtextxy(ru * 26, ru / 2 + 16 * i++, L">>enter to go to sumup edit page,maybe with rule loss");
	outtextxy(ru * 26, ru / 2 + 16 * i++, L">>' ' to quit.");
}

void ruleedit(void)//主要接口。进入，离开时均清屏。
{
	memset(p, 0, picbyte);
	int page = 0;

	if (rulemode == SUMMODE)goto summode;
	else goto generalmode;

summode:
	sumpage();
	for (;;) {
		int signal = _getch();
		switch (signal)
		{
		case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
		{
			int num = signal - '0';
			nowbs[num] = nowbs[num] == 3 ? 0 : nowbs[num] + 1;//取消语义依赖。
			_translate_(nowcount, nowbs);
			sumpage();
			break;
		}
		case '0':
		{
			nowbs[0] = nowbs[0] == S ? NO : S;
			_translate_(nowcount, nowbs);
			sumpage();
			break;
		}
		case 'q':case 'w':case 'e':case 'a':case 's':case 'd':case 'z':case 'x':case 'c':
			nowcount[WASDtoDir(signal)] = !nowcount[WASDtoDir(signal)];
			_translate_(nowcount, nowbs);
			sumpage();
			break;
		case 'u':
			CLIZE();
			sumpage();
			break;
		case 'o':
			BOARDIZE();
			memset(p, 0, picbyte);
			page = 0;
			goto generalmode;
		case 'p':
			GAMIZE();
			sumpage();
			break;
		case 'C':
			copyrule();
			break;
		case 'V':
			if (pasterule() == true) {
				memset(p, 0, picbyte);
				page = 0;
				//mode = !mode;//paste函数内置的功能。
				goto generalmode;
			}
			else break;
		case ENTER:
		{
			memset(p, 0, picbyte);
			page = 0;
			rulemode = !rulemode;
			goto generalmode;
		}
		case ' ':goto out;
		default:break;
		}
	}

generalmode:
	generalpage(page);
	for (;;)
	{
		if (_kbhit())
		{
			int signal = _getch();
			switch (signal)
			{
			case 'a':page = page == 0 ? 15 : page - 1; generalpage(page); break;
			case 'd':page = page == 15 ? 0 : page + 1; generalpage(page); break;
			case 'u':
				CLIZE();
				generalpage(page);
				break;
			case 'o':
				BOARDIZE();
				generalpage(page);
				break;
			case 'p':
				GAMIZE();
				generalpage(page);
				break;
			case 'C':
				copyrule();
				break;
			case 'V':
				if (pasterule() == true)
					generalpage(page);
				break;
			case ENTER:
			{
				memset(p, 0, picbyte);
				page = 0;
				rulemode = !rulemode;
				_translate_(nowcount, nowbs);//!!!
				goto summode;
			}
			case ' ':goto out;
			default:break;
			}
		}
		if (MouseHit())
		{
			MOUSEMSG m = GetMouseMsg();//测试击中。
			if (m.wheel > 0)
			{
				page = page == 0 ? 15 : page - 1; generalpage(page);
			}
			else if (m.wheel < 0)
			{
				page = page == 15 ? 0 : page + 1; generalpage(page);
			}
			else if (m.x >= ru / 2 && m.y >= ru / 2 && m.x < pwidth && m.y < pwidth)
			{
				int x = (m.x - ru / 2) / (ru * 6);
				int y = (m.y - ru / 2) / (ru * 4);
				int dx = (m.x - ru / 2) % (ru * 6);
				int dy = (m.y - ru / 2) % (ru * 4);
				if (x < 4 && y < 8 && dx >= ru * 3 && dy >= 0 && dx < ru * 6 && dy < ru * 3)
				{
					int i = page * 32 + x + y * 4;
					if (i!=0)
					{
						if (m.mkLButton == true)
							nowrule[i] = 1;
						else if (m.mkRButton == true)
							nowrule[i] = 0;
						int j = i % 32;
						_rulelil_(ru/2 + 6 * ru * (j % 4), ru/2 + 4 * ru * (j / 4), ru, i);
					}

				}
			}
		}
		Sleep(1);
	}
out:
	memset(p, 0, picbyte);
}

void copyrule(void)
{
	static char buffer[512 + 1]{};
	buffer[512] = '\0';
	for (int i = 0; i < 512; i++)
		buffer[i] = nowrule[i] == 0 ? '0' : '1';
	if (ClipBoardAcTextWrite(buffer, strlen(buffer) + 1) != true)
		BEEP();
}
bool pasterule(void)
{
	static char buffer[512]{};

	memset(buffer, 0, 512);
	if (ClipBoardAcTextRead(buffer, 512) != true)
	{
		BEEP(); return false;
	}
	for (int i = 0; i < 512; i++)
		if (buffer[i] != '0' && buffer[i] != '1') { BEEP(); return false; }
		else {
			buffer[i] = buffer[i] == '0' ? 0 : 1;
		}
	for (int i = 0; i < 512; i++)
	{
		nowrule[i] = buffer[i];
	}
	rulemode = GENERALMODE;
	return true;
}


#pragma warning(disable:26495)
RL_structure::RL_structure(void)
{
	assert(rulemode == GENERALMODE || rulemode == SUMMODE);
	savekind = rulemode;
	if (rulemode == GENERALMODE) {
		memset(rl.totrl, 0, 16 * sizeof(unsigned));//这一步是非常必要的。
		for (int i = 0; i < 512; i++)
		{
			rl.totrl[i / 32] |= ((unsigned)(nowrule[i] != 0) << (i % 32));
		}
	}
	else {
		for (int i = 0; i < 9; i++)
		{
			rl.sumrl.count[i] = nowcount[i];
		}
		for (int i = 0; i < 10; i++)
		{
			rl.sumrl.bs[i] = nowbs[i];
		}
	}
}
bool RL_structure::release(void)
{
	if (savekind != GENERALMODE && savekind != SUMMODE)
		return false;
	if (savekind == SUMMODE)
	{
		for (int i = 0; i < 9; i++)
			if (rl.sumrl.count[i] != 0 && rl.sumrl.count[i] != 1)
				return false;
		for (int i = 0; i < 10; i++)
			if (!(rl.sumrl.bs[i] >= 0 && rl.sumrl.bs[i] <= 3))
				return false;
	}
	rulemode = savekind;
	if (rulemode == GENERALMODE) {
		for (int i = 0; i < 512; i++)
		{
			nowrule[i] = (rl.totrl[i / 32] & (1 << i % 32)) != 0;
		}
	}
	else {
		for (int i = 0; i < 9; i++)
		{
			nowcount[i] = rl.sumrl.count[i];
		}
		for (int i = 0; i < 10; i++)
		{
			nowbs[i] = rl.sumrl.bs[i];
		}
		_translate_(nowcount, nowbs);
	}
	return true;
}
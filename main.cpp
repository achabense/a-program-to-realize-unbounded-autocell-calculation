#if 1
/*****************************
*2020/6/16�ع���
*�����˲������Ż���ĳЩ����ķ��
*��ʵ����һ���������Դ��Ľ���ͼЧ�ʡ�
*��ָ��塣��ᵼ�´��ģ�Ĵ�������������ԡ�
*��Ϊarea�ĸ������ܣ��ṩ���޿ռ�Ļ�ͼ�Ĺ��ܡ�
******************************/
#include<time.h>//Ϊ��ʵ����������������
#include"rule.h"
#include"area.h"
#pragma warning(disable:4244)
DWORD(*p)[pwidth];

/* ps:��ʵ���д��500���Ԫ��ʱ���ͱȽ������ˡ�*/
const char* proginfo = 
"����һ��ʵ�����޿ռ�(1)Ԫ������(2)�ĳ���(3)��\n"
"����������������ɣ�Ԫ���������(�ο�2)��BS����༭����(�ο�2)���Լ�512����༭����(�ο�2)��\n"
"����Ԫ�����������������ִ��ģʽ���Զ�ģʽ�Ϳ���ģʽ���ֱ�֧�ֲ�ͬ�Ĳ������򿪳���ķ�������\n"
"����(5)�������ᵼ�½��������档\n"
"���潫ֱ�ӽ��ܼ�������еĲ����������ڴ�֮ǰ��ο����漰������Ľ���(4)��/n"
"\n"
"������ģʽ�У����²���������ͬ�ģ�\n"
">>>���ڲ�����\n"
"wasd�����ڴ�������λ�ã�qe�����ڴ��ڴ�С��h��ʹ���ڵĳߴ������λ�ûָ�Ϊ��ʼֵ��\n"
"��������Ĭ�ϻ���һ��Сʮ�֣�0�����Կ����͹ر�������ܡ�\n"
">>>������������\n"
"123��������ʱ������3��ʹ��ʱ����Ϊ1��\n"
"zxc�����ڻ�ͼƵ�ʣ�����c��ʹ��ͼƵ�ʹ�Ϊ1��\n"
">>>���������Q������տռ��е�Ԫ����\n"
"���������OKLP��(5)��\n"
">>>ճ���������\n"
"I������ճ���巢�������Ϣ��(������(11)��)\n"
"G������ճ���巢��һ�����й����µľ���(6)(������)�����ܻ�����ʧ��(7)��\n"
"Z������ճ������ճ��Ԫ��(6)��(ʧ��ʱ�����塣)\n"
">>>����������\n"
"��Щ�����������й���༭�����ʽʵ�ֵģ�������ᵼ������(11)��\n"
"+����ʹ���򼯵���Ϊ�װ����(8)(��ο�2)��\n"
"%�������ƹ���ճ������(9)��\n"
"^������ճ�����ж�ȡ����(9)��\n"
"&��������������Ϸ����(2)��\n"
"*������չ���(10)��\n"
"\n"
"���ߵ��������ڣ�\n"
"�����Ʋ�ͬ�����Զ�ģʽ�£�����������ʵ�ִ��ڵ���ק�����ֵ��ڴ��ڴ�С���ڿ���ģʽ�£����\n"
"���Ҽ�ʵ�ֻ�ͼ����(Ҫ�󴰿ڳߴ粻����768)�����ֵ��ڴ��ڴ�С��\n"
"�Զ�ģʽ��f���Ϳո���������ģʽ������ģʽ�°�f�������Զ�ģʽ�����ո��ִ��һ�����㡣����ģ\n"
"ʽ�¾���enter���������༭���档�ӹ�����淵�أ�ͳһ���ؿ��ƽ��档\n"
"\n"
"���ڹ�����棬���Բο������ڲ�����ʾ���в�����\n"
"\n"
"\t\t\t\t\t\t\t2020/6/16 ���ߣ�irmae\n"
"\n"
"<<<*1>>>\n"
"ʵ������(rangeof int)*32��Χ�Ŀռ䡣\n"
"<<<*2>>>\n"
"����Ԫ�����Ľ��ܣ�����Ͳ���ϸչ���ˣ�����Ҫ��˵��һЩ������ʵ�ֵ�Ԫ���������ʣ�����������ʹ\n"
"��Ԫ�������˽�Ķ�������京�塣\n"
"����������У�ÿ��Ԫ�����������ܵ�״̬��0̬��1̬����һ�ּ����У�������Ԫ����Χ��8��Ԫ������\n"
"XXX\n"
"XSX\n"
"XXX,��������Ԫ��S���ڣ�ȡ��һ��Ԫ�����ܵ�״̬(���ĳ�֮Ϊ����)������������Ԫ����һ����״̬(0��1)��\n"
"�����Էǳ���Ȼ�ķ�ʽ������б��룬����һ������Ϊ\n"
"qwe\n"
"asd\n"
"zxc��(i��{0,1})����ֱ����ΪС�˶���������qweasdzxc������\n"
"010\n"
"110\n"
"001���Ա���Ϊ010110001(С�˶�����)������������£����Գ�֮Ϊ״̬282(���ʮ���ƣ�����˵��ͨ\n"
"��ʮ������),��2^1+2^3+2^4+2^8�������״̬�£���������Ԫ����Ϊj(��{0,1}),�����state[282]->i��\n"
"��state[qweasdzxc]->jΪһ������Ԫ���ܹ���2^9=512�ֻ�����state[i]->j��i��{0,1,...,511},\n"
"j��{0,1}�������������(����)Ԫ�������п��ܵĻ����֮Ϊ���Ԫ���Ĺ���(���²���Ϊ��������\n"
"��֮Ϊ512����)��\n"
"����ʵ��ʵ���У��涨�ռ�������Ԫ������ͬһ�����򼯡�������ͬ�ķ�����һ��512�����൱��һ��\n"
"512λ�������������Ժ����׵���һ��512�ֽڵ��������洢���������ּܹ��У��ܹ���2^512��512���򼯣�\n"
"�����嵽ʵ���ϣ���������֧����2^511�ֿ��ܣ�����state[0]->1�Ǳ���ֹ�ġ�\n"
"���Ͼͽ�Ϊ����ؽ�����Ԫ�������ֿ���״̬���ֲ��ڶ�ά��������ռ䣬������Ϊ���ڵ�8����Ԫ����\n"
"����Ԫ�����ù��򼯵����Ρ�������ǳ�����ʵ�ֵ����㡣���������£��±߽����ܸ�Ϊ�ռ���BS����\n"
"�������оͰ���������������Ϸ(Game of Life)��\n"
"BS����Ҫ����㲻���������8������Ԫ����״̬Ϊ1��Ԫ��(��Ϊ���)���������Դ�����������Ԫ��ת\n"
"��Ϊʲô״̬����0->1Ϊ������(B)����1->0Ϊ������1->1Ϊ�����(S)���������³����������͵��´���\n"
"�����ֱ��г��͹�����BS������ϵ���������Ĺ涨�£�������Ϸ�Ĺ���ΪB23/S3�������µı�ʾ������\n"
"�����ܸ�������˵�������\n"
"B&&S:0->1,1->1��\n"
"B:0->1,1->0��\n"
"S:0->0,1->1��\n"
"��:0->0,1->0��\n"
"BS�����ǵ�512���򼯵��Ӽ���֮������ô˵������ΪBS�����ܿ��Է���Ϊ512���򼯡����ﲻ�г�����\n"
"�����������ڴ�ָ����������Ϸ���Է���Ϊ��\n"
"00000001000101100001011101111110000101100110100001111110111010000001011001101000011111101\n"
"11010000110100010000000111010001000000000010110011010000111111011101000011010001000000011\n"
"10100010000000011010001000000011101000100000001000000000000000100000000000000000010110011\n"
"01000011111101110100001101000100000001110100010000000011010001000000011101000100000001000\n"
"00000000000010000000000000000110100010000000111010001000000010000000000000001000000000000\n"
"0001000000000000000100000000000000000000000000000000000000000000000\n"
"�ر�ģ������Ĭ�ϳ�ʼ�������������Ϸ��\n"
"��BS������΢���廯(��Ȼ����512���򼯵��Ӽ�)���ڼ������ڻ����(�����Լ�)ʱͳһ����һ�����룬\n"
"iopjklbnm,ÿλ����Ϊ0��1������\n"
"qwe\n"
"asd\n"
"zxc�Ļ��������������Ϊi*q+o*w+p*e+...��������֮�����BS���򡣳���֧�ֹ���ı����Լ����ִ������\n"
"(�������)BS���򼯵ı��롣ʵ���ϣ������ڲ�ͳһ���õ���512���򼯣��ڱ༭BS�����ͬʱ������Ὣ֮\n"
"����Ϊ512���򼯡�����������ʣ��ڹ���ҳ���л�ʱ��BS�����л���512���治�������ʧ�����Ǵ�512����\n"
"���ػὫ�����л������ʹ�õ�BS�����ϣ��Ӷ����ܻ���ɹ�����ʧ��\n"
"������һ����Ȥ�Ĺ��򼯣���֧����Ԫ���ռ���ֱ�ӽ��л�ͼ���Ǿ���state[qweaXdzxc]->X������ĵ��÷���\n"
"�μ����ġ�\n"
"<<<*3>>>\n"
"��ʽ��Ԫ���������Ƽ�Golly��\n"
"<<<*4>>>\n"
"�Ƽ��������ߵĲ���Ϊ\"����\"���ұ�Ϊ��Ϣ������Ϣ���еĲ������ϵ��°�����\n"
"���������ڼ��μ��㡣\n"
"��������ÿ����������32*32��Ԫ������Ϊ����ά���ռ�ĵ�Ԫ������̬ά��һ������Ԫ�������鼯�ϡ��ڻ�\n"
"ͼ�У���������1ֵԪ��������Ͳ�����������Ļ��Ʒ�����ͬ��\n"
"Ԫ������ָ����ֵΪ1��Ԫ��������������\n"
"���ڳߴ磺���ڶ�Ӧ�ڼ��˼���Ԫ���Ŀռ䡣���ߴ糬��768ʱ������Ԫ����������ǳɫ����档���ڳߴ���ܵ�\n"
"ȡֵΪ96��192��384��768��1536��3072��6144��12288��24576��\n"
"����λ�ã�ָ���Ǵ��ڵ�����ָ�����������ꡣ\n"
"��ʱ�����Զ�ģʽ�£�����ÿ��������i�κ��ͼ����sleepһ��ʱ�䣬�Ժ���ơ�\n"
"��ͼƵ�ʣ�iֵ�����ֵΪ128��\n"
"<<<*5>>>\n"
"�������ı������,����ͼ�񱣴棬�����Զ�����ļ���ʽ���Լ�Ԫ��Ƭ��ճ����\n"
"�ڼ�������£�P�������ڲ��ֽ�ͼ���洢ΪPNG�ļ������塣\n"
"�����Զ�����sve�ļ���ʽ��sve�ļ�������Ԫ���ռ�Ĺ��򼯣������������ֱ���ģʽ���ֱ�Ϊȫ�屣��(O��)��\n"
"floodfill����(L��)���Լ������ȡ����(K��)���ֱ������ڲ�ͬ�ı���Ŀ�ġ��ⶼ���ڼ��������ʵ�ֵġ�\n"
"\n"
"O����ֱ�ӱ���Ԫ���ռ�Ϊtotal-(...).sve��K/L����ʹ����ֹͣ���㣬���ȴ�������floodfill������Ʊ���\n"
"������������鼰������(��Χ8���������)���������飬������Ϊflood-(...).sve�����鱣����Ʊ�����\n"
"�껮���ķ��������ǵ������飬������Ϊrange-(...).sve��KL���ֹ����£����̵�qweasd����Ȼ��Ч���Ҿ�����\n"
"������Ҽ�ȡ���������������ֹ��ܲ��ᱣ��û��Ԫ�����ļ��������۳ɹ���񣬾��ᴥ�����塣\n"
"\n"
"Z��ʵ��ճ�����ơ�����ֻ�����ı�ճ��������ʶ��ͳrle��ʽ(Ҫ��rle���ֲ�����2048),���ڲ���ʶ��Ļ��ƣ�\n"
"��ͳһ����Ĭ��ģʽʶ�𡣰���Z��֮�󣬳���ֹͣ���㣬ʹ����ʹ�����ָ��ճ�������Ͻǣ�Ҫ�󴰿ڳߴ粻��\n"
"��768���Ծ�ȷ��Ԫ����λ�á����԰��Ҽ�ȡ��ճ����\n"
"\n"
"rle��ʽ�����ӣ�(ֻ֧��bo�汾)\n"
"# ark1 -- 16 cells, stabilizes at 736692 gens, found by Nick Gotts.\n"
"x = 32, y = 29, rule = B3/S23\n"
"27bo$28bo$29bo$28bo$27bo$29b3o20$oo$bbo$bbo$3b4o!\n"
"\n"
"Ĭ�ϸ�ʽ����Ч�����ӣ�(daynight����(B3678/S34678))\n"
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
"����' '��'\\t'��'.'��'\\r'����Ϊ0������'\\n'����Ϊ���У����ദ��Ϊ1����\n"
"��ճ����Ϊ�գ���ճ���嶥����ʽ��Ϊ�ı���ʽ����ᵼ��ճ��ʧ�ܲ����塣\n"
"���Ͻ��ܵĻ��ƣ�����Ԫ��Ƭճ������ճ��ʧ��ʱ�����⣬���ಿ�־��������塣\n"
"\n"
"sve�ļ��򿪵ķ�ʽΪ��ק�򿪡����ִ򿪷�ʽʹ����ֱ�ӽ������ģʽ��ֱ�Ӵ򿪳��򣬳��������Զ�ģʽ����\n"
"������ϷΪ��ʼ���򼯣�����������blinker(������Ϸ������)��\n"
"<<<*6>>>\n"
"���壺���ض������£�����һ�ּ���󱣳�ԭ����\n"
"<<<*7>>>\n"
"�����ȡ���������Ʋ�һ����Ч�������Ƕ��ڴ��ھ���Ĺ��򼯡���ʧ�ܣ�����ճ���巢��һ��(fail)��\n"
"<<<*8>>>\n"
"�װ�����£��κηֲ������ھ��壬��ʹ�ó������ֱ��ά��һ��û�г����ͼƬ��\n"
"<<<*9>>>\n"
"����512���򼯵���ʽ���ݵġ�\n"
"<<<*10>>>\n"
"state[i]->0��\n"
"<<<*11>>>\n"
"���庯���ķֲ�ʮ�ֻ��ң�����ĵ����ܻ���ֽ��ܴ���\n";

void sendproginfo()
{
	ClipBoardAcTextWrite(proginfo, strlen(proginfo) + 1);
	BEEP();
}
long long generations = 0;
long long Blocks = 0;
long long Cells = 0;
int startx = 0;
int starty = 0;//���Ͻǵ����ꡣ
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
	_vsnwprintf(buffer, 512, fmt, vp);//...//nΪbuffer��С��
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


//�ڹ�ϣ�£�ֻҪ��������̫�ߣ�Ч�ʲ����͡�
bool centercross = true;
void outportrait(void) {

	int bitsize = scalesize / scale;
	int gridper = PIC / scalesize;//ÿ�е�grid����

	if (scalesize >= scale) {//���ֶ�ȡ������
		for (int y = 0; y < gridper; y++)
			for (int x = 0; x < gridper; x++)
			{
				int pix = x * scalesize;
				int piy = y * scalesize;//ÿ��grid�������ʹ���ꡣ
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
	else {//<scale��
		for (int y = 0; y < gridper; y++)//������
			for (int x = 0; x < gridper; x++) {
				int pix = x * scalesize;
				int piy = y * scalesize;//ÿ��grid�������ʹ���ꡣ
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
*�ع�������Լ�����ֵ���
*��ȷ�ԡ�
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
//��Ϊ�ο�ϵͼ������ϵ��
//�һ����Ǹ㷴�ˡ����Ǿ����ڵ���ʱ�Ե�ft��ֱ������������
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
	case '*':CLIZE(); BEEP(); break;//�����ǲ��ó��ֵġ�����������������Ļ���Ҳ����ʵ�֡�
	case '+':BOARDIZE(); BEEP(); break;

	default:break;
	}
	if (isportrait == true)outportrait();
	if (isinfo == true)info();
}



/***************
�����ɶԳ��֣�
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

	//��ʱ�İ��ţ�
	clearall();
	bool loadfile = false;
	if (argc == 2 && read_from_file(argv[1]) == true) { loadfile = true; }
	else {
		GAMIZE();
		/*��������������*/
		outerset(startx + 2, starty + 2, scale / 2 - 1, scale / 2, 1);
		outerset(startx + 2, starty + 2, scale / 2, scale / 2, 1);
		outerset(startx + 2, starty + 2, scale / 2 + 1, scale / 2, 1);

		outerset(startx + 3, starty + 3, scale / 2 - 1, scale / 2, 1);
		outerset(startx + 3, starty + 3, scale / 2, scale / 2, 1);
		outerset(startx + 3, starty + 3, scale / 2 + 1, scale / 2, 1);
	}
	POR_IN();
	if (loadfile == true)goto stopping;

	clock_t lasttime;//������������
	int lastx, lasty;
	bool isactived;
nonstopping:
	//�����ק���ܡ�
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
			if (sig == ENTER ||sig=='f'|| sig == ' ')//˫�뵥��ģʽ��
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
			}//��ק�ƶ���ͷ�����ǲ�Ҫ֧�����Ҽ��ˡ�
			else if(m.mkRButton==true){
				if (isactived == false) {
					lastx = m.x, lasty = m.y;
					isactived = true;
					lasttime = clock();
				}
				else {//Ҫ����8����Ϊ�˽��"�������"�����⡣
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
	����֮ǰ��ʵ���б�ҪŪ���Ҫ������Щ������
	�ѵ�����CV���ơ�
	C���ռ�ı��档�������ļ���֧������ģʽ��
	V����ճ�����л��Ԫ��ͼ����
	��Ӧ�ĺ�����ʵ���ˣ������׵������������������������ѡ�
	Ҫʵ�ֽϺõĽ��档��rangesave��Ҫʵ�ַ��򡣶���������mandelbrot����ʵ���е�һ���ѵ㡣
	*************************************************/
stopping:
	lasttime = 0;//������������
	lastx = 0, lasty = 0;//��û��golly���������𡣡������ƶ�ʱ������߱�����ˡ���
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
			case 'f':goto nonstopping;//f�� ' '������
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
//��������󼸸������ˣ��Ͳ���ʲô����Ľ����ԣ��������ˡ�
void paste_interface(void)
{
	bool isacrossin = false;
	int lastcrossx = 0, lastcrossy = 0;
	extrainfo(L"paste from clipboard.right click to quit.");
	flushcontrol();
	while (true) {
		bool isportrait = false;
		bool isinfo = false;
		if (_kbhit())//�ṩһ���ǳ����޵Ĳ������ϡ�
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
			}//�Ҽ�ȡ����
			if (isacrossin == false && m.x >= 2 && m.y >= 2 && m.x < PIC - 2 && m.y < pheight - 2) {
				lastcrossx = m.x, lastcrossy = m.y;
				isacrossin = true;
				movingcross(lastcrossx, lastcrossy);
			}
			else if (m.x >= 2 && m.y >= 2 && m.x < PIC - 2 && m.y < pheight - 2) {
				movingcross(lastcrossx, lastcrossy, m.x, m.y);
				lastcrossx = m.x, lastcrossy = m.y;
			}
			if (m.mkLButton == true) {//debug:�������rawpaste������˰�ȫ��⡣�Ż������̡�
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
//���ϱߵĶ���ֻ��һ��������
void floodsave_interface(void)
{
	bool isacrossin = false;
	int lastcrossx = 0, lastcrossy = 0;
	extrainfo(L"floodfill save.right click to quit.");
	flushcontrol();
	while (true) {
		bool isportrait = false;
		bool isinfo = false;
		if (_kbhit())//�ṩһ���ǳ����޵Ĳ������ϡ�
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
			}//�Ҽ�ȡ����
			if (isacrossin == false && m.x >= 2 && m.y >= 2 && m.x < PIC - 2 && m.y < pheight - 2) {
				lastcrossx = m.x, lastcrossy = m.y;
				isacrossin = true;
				movingcross(lastcrossx, lastcrossy);
			}
			else if (m.x >= 2 && m.y >= 2 && m.x < PIC - 2 && m.y < pheight - 2) {
				movingcross(lastcrossx, lastcrossy, m.x, m.y);
				lastcrossx = m.x, lastcrossy = m.y;
			}
			if (m.mkLButton == true) {//debug:�������rawpaste������˰�ȫ��⡣�Ż������̡�
				if (m.x < 0 || m.y < 0 || m.x >= PIC || m.y >= PIC) { BEEP(); }//���û�гߴ���Ҫ��
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
//����ͱȽϵļ����ˡ�
//�Һ�д��mand����
void rangesave_interface(void)
{
	setcolor(WHITE);
	setwritemode(R2_XORPEN);
	bool isrec = false;//���޳�����
	int recfx = 0, recfy = 0, rectx = 0, recty = 0;	//ѡ����

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
						//�±��ǵ�������
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
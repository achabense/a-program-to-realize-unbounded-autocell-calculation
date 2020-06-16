#pragma once
#include"def.h"
/**************************
*2020/5/18重构。
*从main中分离出来的。
**************************/
grid search(int x,int y);
void acirc(long long* Blocks, long long* Cells);
void clearall(void);

void outerset(int x, int y, int dx, int dy, int as);

#define seed 27357171
//返回一个静体，若成功贴在粘贴板上。
void stillgrid(void);




/*************************************************
指从粘贴板中获得数据，并以输入变量为起始量进行绘制:
自带判断功能。
area.cpp中为源码提供了一些辅助函数。
该函数并没有充分应用它们。有兴趣的话可以改进它。
至多支持256kb的输入。
这个输入没有更新网格数据。为此，程序提供了独立的统计函数。
*************************************************/
/*******************************************
这个程序指支持txt输入。将'.',' ','\t'视为0胞，遇见换行则进行换行，
遇到其它符号均视为1胞。
如，以下东西(Day and Night中的图样)是可以录进去的：
 .............................OO..................................
 ..............................O..................................
 ..........................O...O...OO.............................
 ..O......................OO.OOOOOOOO.........OO..................
 ...OO...................OOOOOOOOOOO.O...OO.OOOOOO.............OO.
 O.OO.O.................OOOOOOOOOOOOO..OOOOOOOOOOO.......O....OOO.
 .OOOOO................OOOOOOOOOOOOOOOOOOOOOOOOOO..........OOOOO.O
 O.OO.O.................OOOOOOOOOOOOO..OOOOOOOOOOO.......O....OOO.
 ...OO...................OOOOOOOOOOO.O...OO.OOOOOO.............OO.
 ..O......................OO.OOOOOOOO.........OO..................
 ..........................O...O...OO.............................
 ..............................O..................................
 .............................OO..................................
 尤其注意换行与空格是视为0胞的。
********************************************/
/************************
*2020/5/21转换通用的规则转换函数。
*golly可以直接打开。但是这个程序只可以用复制粘贴实现。
*普通BS规则的解析函数。
*标准状态压缩的解析函数。
*检测器。
*通过检测的话，则压缩，否则不压缩。
*支持注释解析。
*勉强实现的，反正是线性时间，没有进行优化。
#C p416 2c/5 spaceship gun: Dave Greene, 11 Apr 2003
#C 64 p416 guns driving 60 Herschel-based glider inserters
#C to produce four construction salvos totalling 63 gliders.
#C Synthesis of 2c/5 spaceship mostly by Noam Elkies.
x = 990, y = 979, rule = B3/S23
864bobo$867bo9boo$863bo4bobboo3bobbo$863boobobboboo3bobobo34boo$867boo
8bobbo22boo10b3o$881bo20bobbo3boobbobbobo$878bobo22bobo3boobboobboo$
871bo29boobo8boo$871bob3o25b3o$873bobbo24boo$735bo131bo6boo33boo$725b
...典型的规则如上。考虑到大pattern，支持到2048的连续长度吧。
//  x=1,y = 157, rule = B3/S23
//banned.
//y=157,rule=B3/S23
//banned
//x=1,y = 157,
//allowed.

//可见，x被简单地用来当作#符。
//boob...前边是允许由空白符的。
//b：1bit。
//o：0bit。
************************/
/******************
*另一个小的调整：由内部beep改为返回false。
******************/
bool patternpaste(int x, int y, int dx, int dy);


//重新统计数据。
void blockandcell(long long* blocks, long long* cells);
#pragma once
#include<conio.h>
#include<graphics.h>
#include"def.h"

/********************
*2020/6/16
*添加了新的规则集：画板规则集，即
env(cent)=cent。
*无限画板，是元胞规则的内部性质，实在是太好了。
********************/
//一部分函数在def.h中声明。
void GAMIZE(void);
void BOARDIZE(void);
void CLIZE(void);
void ruleedit(void);
void copyrule(void);
bool pasterule(void);



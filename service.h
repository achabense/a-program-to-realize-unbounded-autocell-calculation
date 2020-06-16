#pragma once
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<windows.h>
#include<time.h>
#pragma warning(disable:4996)
//2020/6/14重构。重构了头文件。和除了beep外的所有代码。

//程序允许的最大字节长度。16mb。
//clipio(wr)：
//均要求使用CF_TEXT格式。
//成功返回true，否则false。
//size指buffer size。
//getsize：
//成功返回size，包含结尾的\0，失败返回(size_t)(-1)。

#define maxbufsize (16 * 1024 * 1024)
bool ClipBoardAcTextWrite(const void* buffer, size_t bytesize);
bool ClipBoardAcTextRead(void* buffer, size_t bytesize);
size_t getclipsize(void);

//name：
//若都为空，则不生成名字。
//若bodyh或suffix为空，则不包含前/后缀。后缀要自带'.'。
//若正常使用，理论上不会出错。
//beep：
//响铃。
wchar_t* getwname(const wchar_t* body, const wchar_t* suffix);
char* getname(const char* body, const char* suffix);
inline void BEEP(void)
{
	putchar('\a');
}

//线遍历函数。
//没有函数指针检测。
void linefunc(int xf, int yf, int xto, int yto, int extra, void (*pointfunc)(int x, int y, int extra));



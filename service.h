#pragma once
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<windows.h>
#include<time.h>
#pragma warning(disable:4996)
//2020/6/14�ع����ع���ͷ�ļ����ͳ���beep������д��롣

//�������������ֽڳ��ȡ�16mb��
//clipio(wr)��
//��Ҫ��ʹ��CF_TEXT��ʽ��
//�ɹ�����true������false��
//sizeָbuffer size��
//getsize��
//�ɹ�����size��������β��\0��ʧ�ܷ���(size_t)(-1)��

#define maxbufsize (16 * 1024 * 1024)
bool ClipBoardAcTextWrite(const void* buffer, size_t bytesize);
bool ClipBoardAcTextRead(void* buffer, size_t bytesize);
size_t getclipsize(void);

//name��
//����Ϊ�գ����������֡�
//��bodyh��suffixΪ�գ��򲻰���ǰ/��׺����׺Ҫ�Դ�'.'��
//������ʹ�ã������ϲ������
//beep��
//���塣
wchar_t* getwname(const wchar_t* body, const wchar_t* suffix);
char* getname(const char* body, const char* suffix);
inline void BEEP(void)
{
	putchar('\a');
}

//�߱���������
//û�к���ָ���⡣
void linefunc(int xf, int yf, int xto, int yto, int extra, void (*pointfunc)(int x, int y, int extra));



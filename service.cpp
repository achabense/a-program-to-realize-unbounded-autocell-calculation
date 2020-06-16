#include"service.h"
/*****************************************
*2020/6/14重构。
*优化得比较彻底。
*去除了错误追溯机制，在这个例子中是不必要的。
*改为cpp风格，并使代码更加紧凑。
*将返回值改为bool模式。
*等等。
******************************************/
bool ClipBoardAcTextWrite(const void* buffer, size_t bytesize)
{
    assert(buffer != nullptr && bytesize <= maxbufsize);
    if (OpenClipboard(nullptr)==0|| EmptyClipboard() == 0) {
        return false;
    }
    HGLOBAL hwd = nullptr;
    void* mem = nullptr;
    if ((hwd = GlobalAlloc(GHND, bytesize)) == nullptr) {
        CloseClipboard();
        return false;
    }
    if ((mem = GlobalLock(hwd)) == nullptr) {
        CloseClipboard(),GlobalFree(hwd);
        return false;
    }
    memcpy(mem, buffer, bytesize),SetClipboardData(CF_TEXT, hwd);
    GlobalFree(hwd),CloseClipboard();
    return true;
}
size_t getclipsize(void)
{
    if (OpenClipboard(nullptr) == 0) {
        return -1;
    }
    HANDLE h = GetClipboardData(CF_TEXT);
    if (h == nullptr) {
        CloseClipboard();
        return -1;
    }
    size_t size = GlobalSize(h);
    CloseClipboard();
    return size;
}
bool ClipBoardAcTextRead(void* buffer, size_t bytesize)
{
    assert(buffer != nullptr && bytesize <= maxbufsize);
    if (OpenClipboard(nullptr) == 0) {
        return false;
    }
    if (IsClipboardFormatAvailable(CF_TEXT) == false) {
        CloseClipboard();
        return false;
    }
    HANDLE h = nullptr;
    void* mt = nullptr;
    if ((h = GetClipboardData(CF_TEXT)) == nullptr|| (mt = GlobalLock(h)) == nullptr) {
        CloseClipboard();
        return false;
    }
    memcpy(buffer, mt, bytesize);
    GlobalUnlock(h),CloseClipboard();

    return true;
}

/*****************************************
*2020/6/14重构。
*使用了不同的名字处理方法。
*与其它函数相配合。
******************************************/
wchar_t* getwname(const wchar_t* body, const wchar_t* suffix)
{
    static wchar_t wname[MAX_PATH]{};
    if (body != nullptr || suffix != nullptr)
        swprintf_s(wname, MAX_PATH, L"%ws-(%u-%u)%ws", body != nullptr ? body : L"", time(nullptr), clock(), suffix != nullptr ? suffix : L"");
    return wname;
}
char* getname(const char* body, const char* suffix)
{
    static char name[MAX_PATH]{};
    if (body != nullptr || suffix != nullptr)
        sprintf_s(name, MAX_PATH, "%s-(%u-%u)%s", body != nullptr ? body : "", time(nullptr), clock(), suffix != nullptr ? suffix : "");
    return name;
}

/*****************************************
*2020/6/14重构。
*没有改变接口，改善了代码。
******************************************/
void linefunc(int xf, int yf, int xto, int yto, int extra, void (*pointfunc)(int x, int y, int extra))
{
    if (xf == xto && yf == yto)pointfunc(xf, yf, extra);
    else {
        if (xf > xto) { int med = xf; xf = xto, xto = med; med = yf, yf = yto, yto = med; }
        //开始时出现了连逗号错误。。
        //以及逻辑错误。
        if (xf == xto) {
            if (yf < yto)
                for (int y = yf; y <= yto; y++) {
                    pointfunc(xf, y, extra);
                }
            else
                for (int y = yf; y >= yto; y--) {
                    pointfunc(xf, y, extra);
                }
        }
        else if (yf == yto) {
            for (int x = xf; x <= xto; x++) {
                pointfunc(x, yf, extra);
            }
        }
        else {
            double k = ((double)yto - yf) / ((double)xto - xf);
            if (k <= -1 || k >= 1) {
                double k_1 = 1 / k;
                if (yf < yto)
                    for (int y = yf, i = 0; y <= yto; y++, i++)
                    {
                        pointfunc(xf + (int)(i * k_1), y, extra);
                    }
                else
                    for (int y = yf, i = 0; y >= yto; y--, i--)
                    {
                        pointfunc(xf + (int)(i * k_1), y, extra);
                    }
            }
            else {
                for (int x = xf, i = 0; x <= xto; x++, i++)
                {
                    pointfunc(x, yf + (int)(i * k), extra);
                }
            }
        }
    }
}
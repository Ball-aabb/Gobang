//起这个名字是因为它在很多程序中都有用
#ifndef BGAME_H
#define BGAME_H

//常用头文件
#include<iostream>
#include<fstream>
#include<string>
#include<graphics.h>
#include<conio.h>
#include<ctime>
#include<vector>
#include<windows.h>

//懒人缩写
#define cleard() cleardevice()
//懒人缩写2
#define rgbc COLORREF
//按键检测必备
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME)&0x8000)?1:0)&&(GetForegroundWindow()==GetHWnd()) 
//懒人缩写3
#define L_MOUSE KEY_DOWN(VK_LBUTTON)
//懒人缩写4
#define R_MOUSE KEY_DOWN(VK_RBUTTON)
//将窗口移动到屏幕正中央
#define move_window() MoveWindow(GetHWnd(),(GetSystemMetrics(SM_CXFULLSCREEN)-getwidth())/2,(GetSystemMetrics(SM_CYFULLSCREEN)-getheight())/2,getwidth(),getheight(),1)

//获取针对当前窗口的鼠标指针
void getmouse(LPPOINT lpPoint){
    GetCursorPos(lpPoint);
    ScreenToClient(GetHWnd(),lpPoint);
}

//改背景颜色（因为改完要清屏，这里把两个函数加到了一起）
void _setbkcolor(rgbc color){
    setbkcolor(color);
    cleard();
}

/*针对win11的sb新版命令提示符会自动弹出的问题（以前版本都不会），
特地借助ds之力编写了以下2个函数以在程序运行时关闭命令提示符窗口*/

std::string st = "\\main";
bool windowHidden = false;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    const std::string* targetString = reinterpret_cast<const std::string*>(lParam);
    char windowTitle[256];
    GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
    std::string title(windowTitle);
    if (title.find(*targetString) != std::string::npos && !title.empty()) {
        if (ShowWindow(hwnd, SW_HIDE)) {
            windowHidden = true;
        }
    }
    return TRUE;
}

bool HideWindowByPartialTitle(const std::string& partialTitle = "\\main") {
    windowHidden=false;
    st=partialTitle;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&st));
    return windowHidden;
}

#endif
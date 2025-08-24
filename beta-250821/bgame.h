#ifndef BGAME_H
#define BGAME_H

#include<iostream>
#include<fstream>
#include<graphics.h>
#include<conio.h>
#include<ctime>
#include<vector>
#include<map>
#include<windows.h>

#define clear() cleardevice()
#define rgbc COLORREF
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME)&0x8000)?1:0)
#define L_MOUSE KEY_DOWN(VK_LBUTTON)
#define R_MOUSE KEY_DOWN(VK_RBUTTON)
#define move_window() MoveWindow(GetHWnd(),(GetSystemMetrics(SM_CXFULLSCREEN)-getwidth())/2,(GetSystemMetrics(SM_CYFULLSCREEN)-getheight())/2,getwidth(),getheight(),1)

void getmouse(LPPOINT lpPoint){
    GetCursorPos(lpPoint);
    ScreenToClient(GetHWnd(),lpPoint);
}

void __setbkcolor(rgbc color){
    setbkcolor(color);
    clear();
}
#define setbkcolor __setbkcolor

#endif
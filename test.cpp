#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include<conio.h>
#include<graphics.h>
using namespace std;
int main() {
    int a,b,key;
    ofstream fout;
    ifstream fin;
    char _c[100];
    initgraph(100,100);
    fin.open(".key_state");
    fin>>key;
    while(key==999){
        Sleep(200);
        fin>>key;
    }
    fin.close();
    if(key==1){
        MessageBox(GetHWnd(),"您的密钥无效或未授权","错误",MB_ICONERROR);
    }else if(key==2){
        MessageBox(GetHWnd(),"网络连接错误","错误",MB_ICONERROR);
    }else if(key==3){
        MessageBox(GetHWnd(),"服务器错误","错误",MB_ICONERROR);
    }else if(key==4){
        MessageBox(GetHWnd(),"其它错误","错误",MB_ICONERROR);
    }else if(key==0){
        return 0;
    }
    //cout<<MessageBox(GetHWnd(),"确定要关闭程序吗？将不保存任何数据","关闭",MB_OKCANCEL|MB_ICONQUESTION);
    return 0;
}
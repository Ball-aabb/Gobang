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
        MessageBox(GetHWnd(),"������Կ��Ч��δ��Ȩ","����",MB_ICONERROR);
    }else if(key==2){
        MessageBox(GetHWnd(),"�������Ӵ���","����",MB_ICONERROR);
    }else if(key==3){
        MessageBox(GetHWnd(),"����������","����",MB_ICONERROR);
    }else if(key==4){
        MessageBox(GetHWnd(),"��������","����",MB_ICONERROR);
    }else if(key==0){
        return 0;
    }
    //cout<<MessageBox(GetHWnd(),"ȷ��Ҫ�رճ����𣿽��������κ�����","�ر�",MB_OKCANCEL|MB_ICONQUESTION);
    return 0;
}
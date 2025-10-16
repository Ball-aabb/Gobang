#include "bgame.h"
#define classic 0
#define single 1
#define AI 2
using namespace std;
struct xy{
    int x;
    int y;
};
int a[15][15],f,wx,wy,hh,ohh,mode,time1,time2,time3,PID,dirs[4][2]={{1,0},{0,1},{1,1},{1,-1}},checkPattern(int x,int y,int player);
bool g(int x,int y,int t);
ofstream fout;
ifstream fin;
vector<xy> ww;
POINT p;
xy xyt,temp,white(),bigwhite();
void start(),sfill(int t),drawb(int lx,int rx,int y,string str),runexe(),kill();
int main(){
    codde:
    start();
    char c[10];
    string st;
    code:
    clear();
    memset(a,-1,sizeof(a));
    hh=ohh=0;
    time1=time(0);
    time3=15;
    time2=time3;
    setbkcolor(WHITE);
    setlinecolor(0);
    setfillcolor(0);
    settextcolor(0);
    settextstyle(100,50,"msyh");
    outtextxy(200,0,"五子棋");
    settextstyle(50,25,"msyh");
    outtextxy(250,110,(mode==classic?"人机对战":(mode==AI?" AI 对战":"单机对战")));
    rectangle(70,170,630,730);
    line(700,0,700,800);
    settextstyle(30,0,"msyh");
    solidrectangle(345,445,355,455);
    solidrectangle(185,285,195,295);
    solidrectangle(505,285,515,295);
    solidrectangle(185,605,195,615);
    solidrectangle(505,605,515,615);
    outtextxy(55,155,"1");
    outtextxy(40,715,"15");
    outtextxy(67,735,"1");
    outtextxy(615,735,"15");
    for(int i=210;i<=710;i+=40){
        line(70,i,630,i);
        if((i-210)/40+2<10){
            outtextxy(55,i-15,itoa((i-210)/40+2,c,10));
        }else{
            outtextxy(40,i-15,itoa((i-210)/40+2,c,10));
        }
    }
    for(int i=110;i<=610;i+=40){
        line(i,170,i,730);
        if((i-110)/40+2>9){
            outtextxy(i-15,735,itoa((i-110)/40+2,c,10));
        }else{
            outtextxy(i-7,735,itoa((i-110)/40+2,c,10));
        }
    }
    line(700,420,900,420);
    settextstyle(30,0,"msyh");
    roundrect(750,180,850,240,30,30);
    outtextxy(770,196,"暂停");
    roundrect(750,260,850,320,30,30);
    outtextxy(770,276,"退出");
    roundrect(750,340,850,400,30,30);
    outtextxy(770,356,"认输");
    roundrect(750,440,850,500,30,30);
    outtextxy(770,456,"让步");
    roundrect(750,520,850,580,30,30);
    outtextxy(770,536,"悔棋");
    roundrect(750,600,850,660,30,30);
    outtextxy(770,616,"清屏");
    roundrect(720,30,880,160,50,50);
    fillcircle(800,65,20);
    setfillcolor(GREEN);
    fillrectangle(754,25,846,35);
    while(true){
        f=0;
        setfillcolor((!hh?BLACK:WHITE));
        fillcircle(800,65,20);
        code1:
        if(L_MOUSE){
            while(L_MOUSE);
            getmouse(&p);
            if(p.x>=50&&p.x<650&&p.y>=150&&p.y<750&&hh==0){
                f=1;
                p.x-=50;
                p.y-=150;
                if(a[p.x/40][p.y/40]==-1){
                    a[p.x/40][p.y/40]=1;
                    setfillcolor(BLACK);
                    fillcircle(p.x/40*40+70,p.y/40*40+170,18);
                    if(mode==AI){
                        fout.open(".in_state");
                        fout<<"1 1";
                        fout.close();
                        fout.open(".in");
                        fout<<(p.y/40+1)<<' '<<(p.x/40+1);
                        fout.close();
                    }
                }else{
                    f=0;
                    goto code1;
                }
                if(g(p.x/40,p.y/40,1)){
                    MessageBox(GetHWnd(),"黑方胜利！","游戏结束",MB_OK);
                    goto code;
                }
                hh=1;
            }else if(p.x>=750&&p.x<=850&&p.y>=180&&p.y<=240){
                while(1){
                    if(L_MOUSE){
                        getmouse(&p);
                        while(L_MOUSE);
                        if(p.x>=750&&p.x<=850&&p.y>=180&&p.y<=240){
                            break;
                        }
                    }
                }
            }else if(p.x>=750&&p.x<=850&&p.y>=260&&p.y<=320){
                if(MessageBox(GetHWnd(),"确定要退出游戏吗？","退出",MB_OKCANCEL|MB_ICONQUESTION)==1){
                    goto codde;
                }
            }else if(p.x>=750&&p.x<=850&&p.y>=340&&p.y<=400){
                if(MessageBox(GetHWnd(),"确定要认输吗？","认输",MB_OKCANCEL|MB_ICONQUESTION)==1){
                    MessageBox(GetHWnd(),(hh?"黑方胜利！":"白方胜利！"),"游戏结束",MB_OK);
                    goto codde;
                }
            }else if(p.x>=750&&p.x<=850&&p.y>=440&&p.y<=500){
                f=1;
                hh=!hh;
            }else if(p.x>=750&&p.x<=850&&p.y>=600&&p.y<=660){
                goto code;
            }
        }
        if(R_MOUSE&&mode==single){
            while(R_MOUSE);
            getmouse(&p);
            if(p.x>=50&&p.x<650&&p.y>=150&&p.y<750&&hh==1){
                p.x-=50;
                p.y-=150;
                if(a[p.x/40][p.y/40]==-1){
                    a[p.x/40][p.y/40]=0;
                    setfillcolor(WHITE);
                    fillcircle(p.x/40*40+70,p.y/40*40+170,18);
                }else{
                    goto code1;
                }
                if(g(p.x/40,p.y/40,0)){
                    MessageBox(GetHWnd(),"白方胜利！","游戏结束",MB_OK);
                    goto code;
                }
                hh=0;
            }
        }
        setfillcolor((!hh?BLACK:WHITE));
        fillcircle(800,65,20);
        setfillcolor(WHITE);
        outtextxy(740,100,(!hh?(mode==single?"黑方回合":"玩家回合"):(mode==classic?"程序回合":(mode==single?"白方回合":" AI 回合"))));
        if(time(0)-time1>=1){
            time2--;
            time1=time(0);
            setfillcolor(WHITE);
            solidrectangle(846-(time3-time2)*(90/time3),26,845+(90/time3)-(time3-time2)*(90/time3),34);
            if(time2<=0){
                MessageBox(GetHWnd(),(hh?"黑方胜利！":"白方胜利！"),"游戏结束",MB_OK);
                goto code;
            }
        }
        itoa(time2,c,10);
        st=c;
        if(time2>9){
            st+=" s";
        }else{
            st=" "+st+" s";
        }
        outtextxy(770,140,st.c_str());
        if(ohh!=hh){
            time2=time3;
            setfillcolor(GREEN);
            fillrectangle(754,25,846,35);
            ohh=hh;
        }
        if(f&&mode!=single){
            hh=0;
            xyt=bigwhite();
            a[xyt.x][xyt.y]=0;
            setfillcolor(WHITE);
            fillcircle(xyt.x*40+70,xyt.y*40+170,18);
            if(g(xyt.x,xyt.y,0)){
                MessageBox(GetHWnd(),"白方胜利！","游戏结束",MB_OK);
                goto code;
            }
        }
        Sleep(20);
    }
    getch();
}
xy bigwhite(){
    if(mode==classic){
        return white();
    }
    int _a,b;
    fin.open(".out_state");
    if(fin.is_open()){
        Sleep(400);
        fin>>_a;
        fin.close();
        if(!_a){
            int flag=0;
            for(int i=1;i<=48;i++){
                Sleep(200);
                fin.open(".out_state");
                fin>>_a;
                fin.close();
                if(_a){
                    flag=1;
                    break;
                }
            }
            if(!flag){
                MessageBox(GetHWnd(),"调用AI的过程中出现了错误，将切换至人机对战模式\n\n错误原因：AI连接超时","错误",MB_ICONERROR);
                mode=classic;
                settextstyle(50,25,"msyh");
                outtextxy(250,110,"人机对战");
                settextstyle(30,15,"msyh");
                return white();
            }
        }
        fin.open(".out");
        fin>>_a>>b;
        fin.close();
        fout.open(".out_state");
        fout<<"0";
        fout.close();
        return xy{_a-1,b-1};
    }else{
        MessageBox(GetHWnd(),"调用AI的过程中出现了错误，将切换至人机对战模式\n\n错误原因：传输文件损坏","错误",MB_ICONERROR);
        mode=classic;
        settextstyle(50,25,"msyh");
        outtextxy(250,110,"人机对战");
        return white();
    }
}
void start(){
    int ttt=0;
    initgraph(900,800);
    move_window();
    code_s:
    setbkcolor(WHITE);
    settextstyle(120,0,"msyh");
    settextcolor(0);
    setlinecolor(0);
    outtextxy(270,0,"五子棋");
    drawb(220,660,200,"开始游戏");
    drawb(220,660,350,"游戏设置");
    drawb(220,660,500,"关于游戏");
    drawb(220,660,650,"退出");
    while(true){
        getmouse(&p);
        if(p.y>=150&&p.y<=250&&p.x>=270&&p.x<=610){
            sfill(1);
            setfillcolor(RGB(0,0,1));
            floodfill(225,200,0);
            floodfill(655,200,0);
            ttt=1;
        }else if(p.y>=300&&p.y<=400&&p.x>=270&&p.x<=610){
            sfill(2);
            setfillcolor(RGB(0,0,1));
            floodfill(225,350,0);
            floodfill(655,350,0);
            ttt=2;
        }else if(p.y>=450&&p.y<=550&&p.x>=270&&p.x<=610){
            sfill(3);
            setfillcolor(RGB(0,0,1));
            floodfill(230,500,0);
            floodfill(650,500,0);
            ttt=3;
        }else if(p.y>=600&&p.y<=700&&p.x>=270&&p.x<=610){
            sfill(4);
            setfillcolor(RGB(0,0,1));
            floodfill(225,650,0);
            floodfill(655,650,0);
            ttt=4;
        }
        if(ttt==1&&!(p.y>=150&&p.y<=250&&p.x>=270&&p.x<=610)){
            setfillcolor(WHITE);
            floodfill(225,200,0);
            floodfill(655,200,0);
            ttt=0;
        }else if(ttt==2&&!(p.y>=300&&p.y<=400&&p.x>=270&&p.x<=610)){
            setfillcolor(WHITE);
            floodfill(225,350,0);
            floodfill(655,350,0);
            ttt=0;
        }else if(ttt==3&&!(p.y>=450&&p.y<=550&&p.x>=270&&p.x<=610)){
            setfillcolor(WHITE);
            floodfill(225,500,0);
            floodfill(655,500,0);
            ttt=0;
        }else if(ttt==4&&!(p.y>=600&&p.y<=700&&p.x>=270&&p.x<=610)){
            setfillcolor(WHITE);
            floodfill(225,650,0);
            floodfill(655,650,0);
            ttt=0;
        }
        if(KEY_DOWN(L_MOUSE)){
            while(KEY_DOWN(L_MOUSE));
            if(p.y>=150&&p.y<=250&&p.x>=270&&p.x<=610){
                clear();
                line(0,50,900,50);
                line(20,25,40,5);
                line(20,25,40,45);
                line(20,25,70,25);
                settextstyle(40,0,"msyh");
                outtextxy(100,5,"开始游戏-玩法选择");
                roundrect(200,120,700,240,30,30);
                roundrect(200,300,700,420,30,30);
                roundrect(200,480,700,600,30,30);
                settextstyle(80,0,"msyh");
                outtextxy(450-textwidth("人机对战")/2,140,"人机对战");
                outtextxy(450-textwidth("单机对战")/2,320,"单机对战");
                outtextxy(450-textwidth(" AI 对战")/2,485," AI 对战");
                settextstyle(25,0,"msyh");
                outtextxy(450-textwidth("（暂缓开放）")/2,565,"（暂缓开放）");
                while(true){
                    if(KEY_DOWN(L_MOUSE)){
                        while(KEY_DOWN(L_MOUSE));
                        getmouse(&p);
                        if(p.x>=20&&p.x<=70&&p.y>=5&&p.y<=45){
                            goto code_s;
                        }else if(p.x>=200&&p.x<=700&&p.y>=120&&p.y<=240){
                            mode=classic;
                            return ;
                        }else if(p.x>=200&&p.x<=700&&p.y>=300&&p.y<=420){
                            mode=single;
                            return ;
                        }else if(p.x>=200&&p.x<=700&&p.y>=480&&p.y<=600){
                            MessageBox(GetHWnd()," AI 对战玩法正在开发中，暂缓开放，敬请谅解。"," AI 对战",MB_ICONINFORMATION);
                            /*mode=AI;
                            char c[100];
                            int key;
                            if(InputBox(c,100,"请输入Deepseek key:\n\n(输入后需等待若干秒检测key是否有效，若长时间未响应系bug)","key",NULL,0,0,0)){
                                fout.open(".apikey");
                                fout<<c;
                                fout.close();
                                fout.open(".key_state");
                                fout<<"999 999";
                                fout.close();
                                fin.open(".key_state");
                                if(fin.is_open()){
                                    Sleep(300);
                                    fin>>key;
                                    fin.close();
                                    while(key==999){
                                        fin.open(".key_state");
                                        fin>>key;
                                        fin.close();
                                        Sleep(200);
                                    }
                                }
                                if(key==1){
                                    MessageBox(GetHWnd(),"密钥无效!","错误",MB_ICONERROR);
                                }else if(key==2){
                                    MessageBox(GetHWnd(),"网络连接错误!","错误",MB_ICONERROR);
                                }else if(key==3){
                                    MessageBox(GetHWnd(),"服务器错误!","错误",MB_ICONERROR);
                                }else if(key==4){
                                    MessageBox(GetHWnd(),"其它错误!","错误",MB_ICONERROR);
                                }else if(key==0){
                                    return ;
                                }
                                //runexe();
                            }*/
                        }
                    }
                }
            }else if(p.y>=300&&p.y<=400&&p.x>=270&&p.x<=610){
            }else if(p.y>=450&&p.y<=550&&p.x>=270&&p.x<=610){
                MessageBox(GetHWnd(),"五子棋\n\n版本:beta-250825\n作者:Wzh\n联系邮箱：wzh_2011@139.com\nGitHub项目地址：https://github.com/Ball-aabb/Gobang\n\n更多内容，详见GitHub","关于游戏",MB_ICONINFORMATION);
            }else if(p.y>=600&&p.y<=700&&p.x>=270&&p.x<=610){
                exit(0);
            }
        }
        Sleep(60);
    }
    getch();
}
int checkPattern(int x,int y,int player){
    int maxLevel=0;
    for(const auto& dir:dirs){
        int dx=dir[0],dy=dir[1];
        int count=1;
        bool blocked1=false;
        for(int i=1;i<5;++i){
            int nx=x+dx*i,ny=y+dy*i;
            if(nx<0||nx>=15||ny<0||ny>=15||a[nx][ny]!=player){
                if(nx>=0&&nx<15&&ny>=0&&ny<15&&a[nx][ny]!=-1){
                    blocked1=true;
                }
                break;
            }
            count++;
        }
        bool blocked2=false;
        for(int i=1;i<5;++i){
            int nx=x-dx*i,ny=y-dy*i;
            if(nx<0||nx>=15||ny<0||ny>=15||a[nx][ny]!=player){
                if(nx>=0&&nx<15&&ny>=0&&ny<15&&a[nx][ny]!=-1){
                    blocked2=true;
                }
                break;
            }
            count++;
        }
        if(count>=5){
            return 6;
        }
        if(count==4){
            if(!blocked1&&!blocked2){
                return 5;
            }
            if(!blocked1||!blocked2){
                return 4;
            }
        }
        if(count==3&&!blocked1&&!blocked2){
            return 3;
        }
        if(count==3&&(!blocked1||!blocked2)){
            return 2;
        }
        if(count==2&&!blocked1&&!blocked2){
            return 1;
        }
    }
    return 0;
}
xy findBestMove(int player,int minPatternLevel){
    xy bestMove={-1,-1};
    int highestPriority=-1;
    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            if(a[i][j]==-1){
                a[i][j]=player;
                int pattern=checkPattern(i,j,player);
                a[i][j]=-1;
                if(pattern>=minPatternLevel&&pattern>highestPriority){
                    highestPriority=pattern;
                    bestMove={i,j};
                }
            }
        }
    }
    return bestMove;
}
int evaluatePosition(int x,int y){
    if(a[x][y]!=-1){
        return -1;
    }
    int value=0;
    int centerDist=abs(x-7)+abs(y-7);
    value=14-centerDist;
    for(int dx=-1;dx<=1;++dx){
        for(int dy=-1;dy<=1;++dy){
            if(dx==0&&dy==0){
                continue;
            }
            int nx=x+dx,ny=y+dy;
            if(nx>=0&&nx<15&&ny>=0&&ny<15){
                if(a[nx][ny]==0){
                    value+=2;
                }else if(a[nx][ny]==1){
                    value+=1;
                }
            }
        }
    }
    return value;
}
xy white(){
    xy myWin=findBestMove(0,5);
    if(myWin.x!=-1){
        a[myWin.x][myWin.y]=0;
        return myWin;
    }
    xy enemyWin=findBestMove(1,5);
    if(enemyWin.x!=-1){
        a[enemyWin.x][enemyWin.y]=0;
        return enemyWin;
    }
    xy myOpenFour=findBestMove(0,4);
    if(myOpenFour.x!=-1){
        a[myOpenFour.x][myOpenFour.y]=0;
        return myOpenFour;
    }
    xy enemyOpenFour=findBestMove(1,4);
    if(enemyOpenFour.x!=-1){
        a[enemyOpenFour.x][enemyOpenFour.y]=0;
        return enemyOpenFour;
    }
    xy myThree=findBestMove(0,3);
    if(myThree.x!=-1){
        a[myThree.x][myThree.y]=0;
        return myThree;
    }
    xy enemyThree=findBestMove(1,3);
    if(enemyThree.x!=-1){
        a[enemyThree.x][enemyThree.y]=0;
        return enemyThree;
    }
    xy bestMove={7,7};
    int maxValue=-1;
    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            int value=evaluatePosition(i,j);
            if(value>maxValue){
                maxValue=value;
                bestMove={i,j};
            }
        }
    }
    a[bestMove.x][bestMove.y]=0;
    Sleep(300);
    return bestMove;
}
bool g(int x,int y,int t){
    int b[8],i,j;
    memset(b,0,sizeof(b));
    i=x+1;
    while(i>=0&&i<15&&a[i][y]==t){
        b[0]++;
        i++;
    }
    i=x-1;
    while(i>=0&&i<15&&a[i][y]==t){
        b[1]++;
        i--;
    }
    i=y+1;
    while(i>=0&&i<15&&a[x][i]==t){
        b[2]++;
        i++;
    }
    i=y-1;
    while(i>=0&&i<15&&a[x][i]==t){
        b[3]++;
        i--;
    }
    i=x+1,j=y+1;
    while(i>=0&&i<15&&j>=0&&j<15&&a[i][j]==t){
        b[4]++;
        i++,j++;
    }
    i=x+1,j=y-1;
    while(i>=0&&i<15&&j>=0&&j<15&&a[i][j]==t){
        b[5]++;
        i++,j--;
    }
    i=x-1,j=y+1;
    while(i>=0&&i<15&&j>=0&&j<15&&a[i][j]==t){
        b[6]++;
        i--,j++;
    }
    i=x-1,j=y-1;
    while(i>=0&&i<15&&j>=0&&j<15&&a[i][j]==t){
        b[7]++;
        i--,j--;
    }
    if(b[0]+b[1]>=4||b[2]+b[3]>=4||b[4]+b[7]>=4||b[5]+b[6]>=4){
        return 1;
    }
    return 0;
}
void drawb(int lx,int rx,int y,string str){
    line(lx+50,y-50,lx,y);
    line(lx+50,y+50,lx,y);
    line(lx+50,y-50,rx-50,y-50);
    line(lx+50,y+50,rx-50,y+50);
    line(rx-50,y-50,rx,y);
    line(rx-50,y+50,rx,y);
    line(lx+65,y-50,lx+15,y);
    line(lx+65,y+50,lx+15,y);
    line(rx-65,y-50,rx-15,y);
    line(rx-65,y+50,rx-15,y);
    settextstyle(60,0,"msyh");
    outtextxy((rx+lx-textwidth(str.c_str()))/2,y-30,str.c_str());
}
void sfill(int t){
    setfillcolor(WHITE);
    if(t!=1){
        floodfill(225,200,0);
        floodfill(655,200,0);
    }
    if(t!=2){
        floodfill(225,350,0);
        floodfill(655,350,0);
    }
    if(t!=3){
        floodfill(225,500,0);
        floodfill(655,500,0);
    }
    if(t!=4){
        floodfill(225,650,0);
        floodfill(655,650,0);
    }
}
/*void runexe(){
    SHELLEXECUTEINFO sei = { sizeof(sei) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = "open";
    sei.lpFile = "ai.exe";
    sei.nShow = SW_HIDE;
    if(ShellExecuteEx(&sei)){
        cout<< "OK!" << endl;
        CloseHandle(sei.hProcess);
    } else {
        cout<<"Failed to start process: " << GetLastError() << endl;
    }
}
void kill(){
    char c[10];
    string s;
    itoa(PID,c,10);
    s=c;
    s="taskkill /pid "+s+" /f";
    system(s.c_str());
}*/
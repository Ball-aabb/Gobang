#include "bgame.h"
#define classic 0
#define single 1
#define AI 2
using namespace std;
struct xy{
    int x;
    int y;
};
int a[15][15],f,wx,wy,hh,mode,PID,dirs[4][2]={{1,0},{0,1},{1,1},{1,-1}},checkPattern(int x,int y,int player);
bool g(int x,int y,int t);
ofstream fout;
ifstream fin;
vector<xy> ww;
POINT p;
xy xyt,temp,white(),bigwhite();
void start(),sfill(int t),drawb(int lx,int rx,int y,string str),runexe(),kill();
int main(){
    start();
    code:
    clear();
    memset(a,-1,sizeof(a));
    hh=0;
    setbkcolor(WHITE);
    setlinecolor(BLACK);
    setfillcolor(BLACK);
    settextcolor(BLACK);
    settextstyle(100,50,"msyh");
    outtextxy(200,0,"五子棋");
    settextstyle(50,25,"msyh");
    outtextxy(250,110,(mode==classic?"人机对战":(mode==AI?" AI 对战":"单机对战")));
    rectangle(70,170,630,730);
    line(700,0,700,800);
    for(int i=210;i<=710;i+=40){
        line(70,i,630,i);
    }
    for(int i=110;i<=610;i+=40){
        line(i,170,i,730);
    }
    line(700,400,900,400);
    settextstyle(30,15,"msyh");
    roundrect(750,240,850,300,30,30);
    outtextxy(770,256,"认输");
    roundrect(750,330,850,390,30,30);
    outtextxy(770,347,"关闭");
    roundrect(750,420,850,480,30,30);
    outtextxy(770,438,"让步");
    roundrect(750,510,850,570,30,30);
    outtextxy(770,528,"清屏");
    roundrect(720,30,880,200,50,50);
    fillrectangle(750,25,850,35);
    fillcircle(800,75,20);
    while(true){
        f=0;
        setfillcolor((!hh?BLACK:WHITE));
        fillrectangle(750,25,850,35);
        fillcircle(800,75,20);
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
            }else if(p.x>=750&&p.x<=850&&p.y>=510&&p.y<=570){
                goto code;
            }else if(p.x>=750&&p.x<=850&&p.y>=420&&p.y<=480){
                f=1;
                hh=!hh;
            }else if(p.x>=750&&p.x<=850&&p.y>=330&&p.y<=390){
                if(MessageBox(GetHWnd(),"确定要关闭程序吗？将不保存任何数据","关闭",MB_OKCANCEL|MB_ICONQUESTION)==1){
                    return 0;
                }
            }else if(p.x>=750&&p.x<=850&&p.y>=240&&p.y<=300){
                if(MessageBox(GetHWnd(),"确定要认输吗？","关闭",MB_OKCANCEL|MB_ICONQUESTION)==1){
                    MessageBox(GetHWnd(),(hh?"黑方胜利！":"白方胜利！"),"游戏结束",MB_OK);
                    goto code;
                }
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
        fillrectangle(750,25,850,35);
        fillcircle(800,75,20);
        setfillcolor(WHITE);
        outtextxy(740,120,(!hh?(mode==single?"黑方回合":"玩家回合"):(mode==classic?"程序回合":(mode==single?"白方回合":" AI 回合"))));
        if(f&&mode!=single){
            hh=0;
            xyt=bigwhite();
            a[xyt.x][xyt.y]=0;
            setfillcolor(WHITE);
            fillcircle(xyt.x*40+70,xyt.y*40+170,18);
            if(g(p.x/40,p.y/40,0)){
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
    fout.open(".apikey");
    fout<<0;
    fout.close();
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
                outtextxy(450-textwidth(" AI 对战")/2,320," AI 对战");
                outtextxy(450-textwidth("单机对战")/2,500,"单机对战");
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
                            mode=AI;
                            char c[100];
                            int key;
                            if(InputBox(c,100,"请输入Deepseek API key:","key",NULL,0,0,0)){
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
                                cout<<key;
                                if(key==1){
                                    MessageBox(GetHWnd(),"您的密钥无效或未授权","错误",MB_ICONERROR);
                                }else if(key==2){
                                    MessageBox(GetHWnd(),"网络连接错误","错误",MB_ICONERROR);
                                }else if(key==3){
                                    MessageBox(GetHWnd(),"服务器错误","错误",MB_ICONERROR);
                                }else if(key==4){
                                    MessageBox(GetHWnd(),"其它错误","错误",MB_ICONERROR);
                                }else if(key==0){
                                    return ;
                                }
                                //runexe();
                            }
                        }else if(p.x>=200&&p.x<=700&&p.y>=480&&p.y<=600){
                            mode=single;
                            return ;
                        }
                    }
                }
            }else if(p.y>=300&&p.y<=400&&p.x>=270&&p.x<=610){
            }else if(p.y>=450&&p.y<=550&&p.x>=270&&p.x<=610){
                MessageBox(GetHWnd(),"五子棋\n\n版本：beat-250821\n更新日期：2025-08\n作者:Wzh\n联系邮箱：wzh_2011@139.com\nGitHub项目地址：https://github.com/Ball-aabb/Gobang","关于游戏",MB_ICONINFORMATION);
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
    for(int d=0;d<4;++d){
        int dx=dirs[d][0],dy=dirs[d][1];
        int maxCount=0;
        for(int center=-4;center<=4;center++){
            if(center==0)continue;
            int nx=x+dx*center,ny=y+dy*center;
            if(nx<0||nx>=15||ny<0||ny>=15)continue;
            if(a[nx][ny]!=player)continue;
            int count=1;
            bool leftBlocked=false,rightBlocked=false;
            for(int i=1;i<=4;i++){
                int leftX=x+dx*(center-i),leftY=y+dy*(center-i);
                if(leftX<0||leftX>=15||leftY<0||leftY>=15){
                    leftBlocked=true;
                    break;
                }
                if(a[leftX][leftY]!=player){
                    if(a[leftX][leftY]!=-1)leftBlocked=true;
                    break;
                }
                count++;
            }
            for(int i=1;i<=4;i++){
                int rightX=x+dx*(center+i),rightY=y+dy*(center+i);
                if(rightX<0||rightX>=15||rightY<0||rightY>=15){
                    rightBlocked=true;
                    break;
                }
                if(a[rightX][rightY]!=player){
                    if(a[rightX][rightY]!=-1)rightBlocked=true;
                    break;
                }
                count++;
            }
            if(count>maxCount)maxCount=count;
            if(count>=5)maxLevel=max(maxLevel,6);
            else if(count==4){
                if(!leftBlocked&&!rightBlocked)maxLevel=max(maxLevel,5);
                else if(!leftBlocked||!rightBlocked)maxLevel=max(maxLevel,4);
            }else if(count==3){
                if(!leftBlocked&&!rightBlocked)maxLevel=max(maxLevel,3);
                else if(!leftBlocked||!rightBlocked)maxLevel=max(maxLevel,2);
            }else if(count==2&&!leftBlocked&&!rightBlocked)maxLevel=max(maxLevel,1);
        }
    }
    return maxLevel;
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
    if(a[x][y]!=-1)return -1;
    int value=0;
    int centerDist=abs(x-7)+abs(y-7);
    value=14-centerDist;
    for(int dx=-1;dx<=1;++dx){
        for(int dy=-1;dy<=1;++dy){
            if(dx==0&&dy==0)continue;
            int nx=x+dx,ny=y+dy;
            if(nx>=0&&nx<15&&ny>=0&&ny<15){
                if(a[nx][ny]==0)value+=2;
                else if(a[nx][ny]==1)value+=1;
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
    return bestMove;
}
bool g(int x,int y,int t){
    int b[8],i;
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
    for(int i=x+1,j=y+1;i<x+5,j<y+5;i++,j++){
        if(i>=0&&i<15&&j>=0&&j<15&&a[i][j]==t){
            b[4]++;
        }else{
            break;
        }
    }
    for(int i=x+1,j=y-1;i<x+5,j>y-5;i++,j--){
        if(i>=0&&i<15&&j>=0&&j<15&&a[i][j]==t){
            b[5]++;
        }else{
            break;
        }
    }
    for(int i=x-1,j=y+1;i>x-5,j<y+5;i--,j++){
        if(i>=0&&i<15&&j>=0&&j<15&&a[i][j]==t){
            b[6]++;
        }else{
            break;
        }
    }
    for(int i=x-1,j=y-1;i>x-5,j>y-5;i--,j--){
        if(i>=0&&i<15&&j>=0&&j<15&&a[i][j]==t){
            b[7]++;
        }else{
            break;
        }
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
void runexe(){
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
}

// \n\n后期更新计划（根据优先级排序）：\n\t1.加入三人模式\n\t2.加入联机模式(仅同一局域网)\n\t3.加入“账号”，保存历史记录，分为云端和本地两种\n\n除“人机对战”玩法中程序代码由DeepSeek生成，其余代码均由作者本人完成，望支持(〃'▽'〃)

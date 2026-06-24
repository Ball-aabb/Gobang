#include "bgame.h"
#define classic 0
#define single 1
#define AI 2
#define ver "beta-260503"
using namespace std;
struct xy{
    int x;
    int y;
};
//                                                                                                                                                                      颜色:   黑   白       红     橙     黄     绿      蓝       灰      棕      紫
int a[15][15],f,wx,wy,mode,t,time1,time2,time3=30,time_start,time_gstart,time_gt,bwin,wwin,bkcolor,textcolor,linecolor,timecolor,bkci=1,tci=0,lci=0,timeci=5,colorl[10]={0,16777215,2763429,36095,55295,5737262,16760576,12500670,21930,13456052},dirs[4][2]={{1,0},{0,1},{1,1},{1,-1}},stime();
void start(),sfill(int t),sfor(int k),scolor(string type),drawb(int lx,int rx,int y,string str),refresh(),refreshs1(),refreshs2();
bool g(int x,int y,int t),sjudge(int t),s1=1,s2=1,windowHidden_forever=1,is_keymr=0,api_pending=0;
char c[50],ct;
POINT p;
string key,keymr="",st2;
ofstream fout;
ifstream fin;
vector<xy> cc;
vector<int> cci;
atomic<int> hh,ohh;
future<string> api_future;
xy xyt,temp,white();
int main(){
    initgraph(900,800);
    //move_window();
    time_start=time(0);
    codde:
    start();
    code:
    cleard();
    memset(a,-1,sizeof(a));
    cc.clear();
    cci.clear();
    hh=ohh=0;
    time1=time(0);
    time2=time3;
    _setbkcolor(bkcolor);
    setlinecolor(linecolor);
    settextcolor(textcolor);
    setfillcolor(0);
    settextstyle(100,50,"msyh");
    outtextxy(200,0,"五子棋");
    settextstyle(50,25,"msyh");
    outtextxy(250,110,(mode==classic?"人机对战":(mode==AI?" AI 对战":"单机对战")));
    rectangle(70,170,630,730);
    line(700,0,700,800);
    settextstyle(30,0,"msyh");
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
    solidrectangle(345,445,355,455);
    solidrectangle(185,285,195,295);
    solidrectangle(505,285,515,295);
    solidrectangle(185,605,195,615);
    solidrectangle(505,605,515,615);
    line(700,420,900,420);
    settextstyle(30,0,"msyh");
    setbkmode(TRANSPARENT);
    roundrect(750,180,850,240,30,30);
    outtextxy(770,196,"暂停 (P)");
    roundrect(750,260,850,320,30,30);
    outtextxy(770,276,"认输 (G)");
    roundrect(750,340,850,400,30,30);
    outtextxy(770,356,"退出 (E)");
    roundrect(750,440,850,500,30,30);
    outtextxy(770,456,"让步 (R)");
    roundrect(750,520,850,580,30,30);
    outtextxy(770,536,"清屏 (C)");
    roundrect(720,30,880,160,50,50);
    fillcircle(800,65,20);
    setfillcolor(timecolor);
    fillrectangle(754,25,846,35);
    setbkmode(OPAQUE);
    future<int> result=async(launch::async,stime);
    while(true){
        f=0;
        setfillcolor((!hh?BLACK:WHITE));
        fillcircle(800,65,20);
        code1:
        if(L_MOUSE){
            while(L_MOUSE);
            getmouse(&p);
            if(p.x>=50&&p.x<650&&p.y>=150&&p.y<750&&((mode==classic&&hh==0)||(mode==AI&&hh==0)||mode==single)){
                f=1;
                p.x-=50;
                p.y-=150;
                if(a[p.x/40][p.y/40]==-1){
                    a[p.x/40][p.y/40]=!hh;
                    cc.push_back({p.x/40,p.y/40});
                    cci.push_back(!hh);
                    setfillcolor((hh==0?0:WHITE));
                    fillcircle(p.x/40*40+70,p.y/40*40+170,18);
                }else{
                    f=0;
                    goto code1;
                }
                if(g(p.x/40,p.y/40,!hh)){
                    if(!hh){
                        bwin++;
                        st="黑方胜利！\n\n要再来一局吗？";
                    }else{
                        wwin++;
                        st="白方胜利！\n\n要再来一局吗？";
                    }
                    if(MessageBox(GetHWnd(),st.c_str(),"游戏结束",MB_OKCANCEL|MB_ICONINFORMATION)==1){
                        goto code;
                    }else{
                        time_gt=time(0)-time_gstart;
                        goto codde;
                    }
                }
                hh=!hh;
            }else if(p.x>=750&&p.x<=850&&p.y>=180&&p.y<=240){
                while(1){
                    if(L_MOUSE){
                        getmouse(&p);
                        while(L_MOUSE);
                        if(p.x>=750&&p.x<=850&&p.y>=180&&p.y<=240){
                            break;
                        }
                    }
                    if(KEY_DOWN('P')){
                        while(KEY_DOWN('P'));
                        break;
                    }
                }
            }else if(p.x>=750&&p.x<=850&&p.y>=260&&p.y<=320){
                t=1;
                if(s2){
                    t=MessageBox(GetHWnd(),"确定要认输吗？\n\n（注：可在游戏设置中\"显示操作执行确认框\"关闭）","认输",MB_OKCANCEL|MB_ICONQUESTION);
                }
                if(t==1){
                    if(hh){
                        bwin++;
                        st="黑方胜利！\n\n\n要再来一局吗？";
                    }else{
                        wwin++;
                        st="白方胜利！\n\n\n要再来一局吗？";
                    }
                    if(MessageBox(GetHWnd(),st.c_str(),"游戏结束",MB_OKCANCEL|MB_ICONINFORMATION)==1){
                        goto code;
                    }else{
                        time_gt=time(0)-time_gstart;
                        goto codde;
                    }
                }
            }else if(p.x>=750&&p.x<=850&&p.y>=340&&p.y<=400){
                t=1;
                if(s2){
                    t=MessageBox(GetHWnd(),"确定要退出游戏吗？（将默认认输）\n\n（注：可在游戏设置中\"显示操作执行确认框\"关闭）","退出",MB_OKCANCEL|MB_ICONQUESTION);
                }
                if(t==1){
                    if(hh){
                        bwin++;
                    }else{
                        wwin++;
                    }
                    time_gt=time(0)-time_gstart;
                    goto codde;
                }
            }else if(p.x>=750&&p.x<=850&&p.y>=440&&p.y<=500){
                f=1;
                hh=!hh;
            }else if(p.x>=750&&p.x<=850&&p.y>=520&&p.y<=580){
                goto code;
            }
        }
        if(KEY_DOWN('P')){
            while(KEY_DOWN('P'));
            while(1){
                if(L_MOUSE){
                    getmouse(&p);
                    while(L_MOUSE);
                    if(p.x>=750&&p.x<=850&&p.y>=180&&p.y<=240){
                        break;
                    }
                }
                if(KEY_DOWN('P')){
                    while(KEY_DOWN('P'));
                    break;
                }
            }
        }else if(KEY_DOWN('E')){
            while(KEY_DOWN('E'));
            t=1;
            if(s2){
                t=MessageBox(GetHWnd(),"确定要退出游戏吗？（将默认认输）\n\n（注：可在游戏设置中\"显示操作执行确认框\"关闭）","退出",MB_OKCANCEL|MB_ICONQUESTION);
            }
            if(t==1){
                if(hh){
                    bwin++;
                }else{
                    wwin++;
                }
                time_gt=time(0)-time_gstart;
                goto codde;
            }
        }else if(KEY_DOWN('G')){
            while(KEY_DOWN('G'));
            t=1;
            if(s2){
                t=MessageBox(GetHWnd(),"确定要认输吗？\n\n（注：可在游戏设置中\"显示操作执行确认框\"关闭）","认输",MB_OKCANCEL|MB_ICONQUESTION);
            }
            if(t==1){
                if(hh){
                    bwin++;
                    st="黑方胜利！\n\n\n要再来一局吗？";
                }else{
                    wwin++;
                    st="白方胜利！\n\n\n要再来一局吗？";
                }
                if(MessageBox(GetHWnd(),st.c_str(),"游戏结束",MB_OKCANCEL|MB_ICONINFORMATION)==1){
                    goto code;
                }else{
                    time_gt=time(0)-time_gstart;
                    goto codde;
                }
            }
        }else if(KEY_DOWN('R')){
            while(KEY_DOWN('R'));
            f=1;
            hh=!hh;
        }else if(KEY_DOWN('C')){
            while(KEY_DOWN('C'));
            goto code;
        }
        setfillcolor((!hh?BLACK:WHITE));
        fillcircle(800,65,20);
        setfillcolor(WHITE);
        outtextxy(740,100,(!hh?(mode==single?"黑方回合":"玩家回合"):(mode==classic?"程序回合":(mode==single?"白方回合":" AI 回合"))));
        // 检查任务是否完成（非阻塞）
        if(result.wait_for(chrono::milliseconds(0))==future_status::ready){
            t=result.get();
            result=async(launch::async,stime);
            if(t==1){
                goto code;
            }else if(t==0){
                goto codde;
            }
            if(ohh!=hh){
                time2=time3;
                setfillcolor(timecolor);
                fillrectangle(754,25,846,35);
                ohh.store(hh.load());
            }
        }
        if(!cc.empty()){
            setfillcolor(RED);
            solidcircle(cc.back().x*40+70,cc.back().y*40+170,5);
            if(cc.size()>1){
                setfillcolor(cci[cci.size()-2]?0:WHITE);
                solidcircle(cc[cc.size()-2].x*40+70,cc[cc.size()-2].y*40+170,5);
            }
        }
        if(f&&mode!=single){
            hh=0;
            xyt=white();
            a[xyt.x][xyt.y]=0;
            cc.push_back({xyt.x,xyt.y});
            cci.push_back(0);
            setfillcolor(WHITE);
            fillcircle(xyt.x*40+70,xyt.y*40+170,18);
            if(g(xyt.x,xyt.y,0)){
                if(hh){
                    bwin++;
                    st="黑方胜利！\n\n\n要再来一局吗？";
                }else{
                    wwin++;
                    st="白方胜利！\n\n\n要再来一局吗？";
                }
                if(MessageBox(GetHWnd(),st.c_str(),"游戏结束",MB_OKCANCEL|MB_ICONINFORMATION)==1){
                    goto code;
                }else{
                    time_gt=time(0)-time_gstart;
                    goto codde;
                }
            }
        }
        Sleep(25);
    }
    getch();
}
void start(){
    code_s:
    int ttt=0;
    refresh();
    settextstyle(120,0,"msyh");
    outtextxy(270,0,"五子棋");
    drawb(220,660,200,"开始游戏");
    drawb(220,660,350,"游戏设置");
    drawb(220,660,500,"游戏帮助");
    drawb(220,660,650,"退出");
    rectangle(40,25,220,85);
    settextstyle(40,0,"msyh");
    outtextxy(50,35,"游戏记录");
    settextstyle(30,0,"msyh");
    outtextxy(450-textwidth(ver)/2,120,ver);
    setlinecolor(RED);
    while(true){
        getmouse(&p);
        if(windowHidden_forever){
            windowHidden_forever=HideWindowByPartialTitle("\\main");
        }
        for(int k=1;k<=4;k++){
            if(sjudge(k)){
                sfill(k);
                setlinecolor(linecolor);
                sfor(k);
                ttt=k;
                break;
            }
        }
        for(int k=1;k<=4;k++){
            if(ttt==k&&!sjudge(k)){
                setlinecolor(bkcolor);
                sfor(k);
                ttt=0;
                break;
            }
        }
        if(KEY_DOWN(L_MOUSE)){
            while(KEY_DOWN(L_MOUSE));
            if(sjudge(1)){
                cleard();
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
                outtextxy(450-textwidth(" AI 对战")/2,500," AI 对战");
                if(s1){
                    MessageBox(GetHWnd(),"可在\"游戏介绍\"查看游戏玩法\n\n（注：可在游戏设置中\"显示游戏提示\"关闭）","提示",MB_ICONINFORMATION);
                }
                while(true){
                    if(KEY_DOWN(L_MOUSE)){
                        while(KEY_DOWN(L_MOUSE));
                        getmouse(&p);
                        if(p.x>=20&&p.x<=70&&p.y>=5&&p.y<=45){
                            goto code_s;
                        }else if(p.x>=200&&p.x<=700&&p.y>=120&&p.y<=240){
                            mode=classic;
                            time_gstart=time(0);
                            return ;
                        }else if(p.x>=200&&p.x<=700&&p.y>=300&&p.y<=420){
                            mode=single;
                            time_gstart=time(0);
                            return ;
                        }else if(p.x>=200&&p.x<=700&&p.y>=480&&p.y<=600){
                            if(s1){
                                MessageBox(GetHWnd()," AI 模式现处于测试阶段，出现问题可联系开发者，敬请谅解。\n\n（注：可在游戏设置中\"显示游戏提示\"关闭）","提示",MB_OK|MB_ICONINFORMATION);
                            }
                            if(is_keymr){
                                key=keymr;
                                mode=AI;
                                time_gstart=time(0);
                                return ;
                            }
                            if(InputBox(c,45,"请输入有效的deepseek api密钥的值"," AI 模式",NULL,0,0,0)){
                                st=check_key(c);
                                if(st=="t"){
                                    key=c;
                                    mode=AI;
                                    time_gstart=time(0);
                                    return ;
                                }else if(st=="f"){
                                    MessageBox(GetHWnd(),"密钥无效！"," AI 模式",MB_OK|MB_ICONERROR);
                                }else{
                                    st2="检查密钥过程中出现错误！\n错误内容："+utf8_to_gb2312(st);
                                    MessageBox(GetHWnd(),st2.c_str()," AI 模式",MB_OK|MB_ICONERROR);
                                }
                            }
                        }
                    }
                }
            }else if(sjudge(2)){
                code_sy:
                cleard();
                line(0,50,900,50);
                line(20,25,40,5);
                line(20,25,40,45);
                line(20,25,70,25);
                settextstyle(40,0,"msyh");
                outtextxy(100,5,"游戏设置");
                outtextxy(50,100,"背景颜色");
                line(50,150,850,150);
                rectangle(800,105,840,145);
                outtextxy(50,160,"文字颜色");
                line(50,210,850,210);
                setfillcolor(textcolor);
                fillrectangle(800,165,840,205);
                outtextxy(50,220,"线条颜色");
                line(50,270,850,270);
                setfillcolor(linecolor);
                fillrectangle(800,225,840,265);
                outtextxy(50,280,"倒计时进度条颜色");
                line(50,330,850,330);
                setfillcolor(timecolor);
                fillrectangle(800,285,840,325);
                outtextxy(50,340,"游戏回合时间");
                line(50,390,850,390);
                rectangle(760,345,840,385);
                outtextxy(50,400,"显示游戏提示");
                line(50,450,850,450);
                refreshs1();
                outtextxy(50,460,"显示操作执行确认框");
                line(50,510,850,510);
                refreshs2();
                outtextxy(50,520,"AI模式默认密钥");
                line(50,570,850,570);
                rectangle(760,525,840,565);
                settextstyle(35,0,"msyh");
                outtextxy(765,347,"更改");
                outtextxy(765,527,"更改");
                outtextxy(595,596,"恢复自定义设置");
                roundrect(590,590,840,640,30,30);
                outtextxy(627,676,"恢复默认设置");
                roundrect(620,670,840,720,30,30);
                outtextxy(57,596,"保存为自定义设置");
                roundrect(50,590,340,640,30,30);
                itoa(time3,c,10);
                st=c;
                if(time3<10){
                    st2=" ";
                    st2+=st;
                    st=st2;
                }
                st+="s";
                outtextxy(695,350,st.c_str());
                while(true){
                    if(KEY_DOWN(L_MOUSE)){
                        while(KEY_DOWN(L_MOUSE));
                        getmouse(&p);
                        if(p.x>=20&&p.x<=70&&p.y>=5&&p.y<=45){
                            goto code_s;
                        }else if(p.x>=620&&p.x<=840&&p.y>=590&&p.y<=640){
                            t=1;
                            if(s2){
                                t=MessageBox(GetHWnd(),"确定要恢复自定义设置吗？\n\n（注：可在游戏设置中\"显示操作执行确认框\"关闭）","恢复自定义设置",MB_ICONQUESTION|MB_OKCANCEL);
                            }
                            if(t==1){
                                fin.open("settings.txt");
                                if(!fin.is_open()){
                                    MessageBox(GetHWnd(),"没有保存的自定义设置！","恢复自定义设置",MB_OK);
                                    goto code_sy;
                                }
                                fin>>s1>>s2>>time3>>bkci>>tci>>lci>>timeci>>bkcolor>>textcolor>>linecolor>>timecolor>>is_keymr>>keymr;
                                fin.close();
                                refresh();
                                MessageBox(GetHWnd(),"已成功恢复自定义设置","恢复自定义设置",MB_OK);
                                goto code_sy;
                            }
                        }else if(p.x>=50&&p.x<=303&&p.y>=590&&p.y<=640){
                            MessageBox(GetHWnd(),"已成功保存为自定义设置","保存为自定义设置",MB_OK);
                            fout.open("settings.txt");
                            fout<<s1<<' '<<s2<<' '<<time3<<' '<<bkci<<' '<<tci<<' '<<lci<<' '<<timeci<<' '<<bkcolor<<' '<<textcolor<<' '<<linecolor<<' '<<timecolor<<' '<<is_keymr<<' '<<keymr;
                            fout.close();
                        }else if(p.x>=620&&p.x<=840&&p.y>=670&&p.y<=720){
                            t=1;
                            if(s2){
                                t=MessageBox(GetHWnd(),"确定要恢复默认设置吗？\n\n（注：可在游戏设置中\"显示操作执行确认框\"关闭）","恢复默认设置",MB_ICONQUESTION|MB_OKCANCEL);
                            }
                            if(t==1){
                                s1=1,s2=1,time3=30,bkci=1,tci=0,lci=0,timeci=5,is_keymr=0,keymr="";                                refresh();
                                MessageBox(GetHWnd(),"已成功恢复默认设置","恢复默认设置",MB_OK);
                                goto code_sy;
                            }
                        }else if(p.x>=800&&p.x<=840&&p.y>=105&&p.y<=145){
                            if(InputBox(c,20,"请输入背景颜色的编号：\n\n黑：0\t\t白：1\n红：2\t\t橙：3\n黄：4\t\t绿：5\n蓝：6\t\t灰：7\n棕：8\t\t紫：9\n\n自定义：'#'+16进制颜色编码\n(示例：#ff0033)","背景颜色",NULL,0,0,0)){
                                scolor("bk");
                                goto code_sy;
                            }
                        }else if(p.x>=800&&p.x<=840&&p.y>=165&&p.y<=205){
                            if(InputBox(c,20,"请输入文字颜色的编号：\n\n黑：0\t\t白：1\n红：2\t\t橙：3\n黄：4\t\t绿：5\n蓝：6\t\t灰：7\n棕：8\t\t紫：9\n\n自定义：'#'+16进制颜色编码\n(示例：#ff0033)","文字颜色",NULL,0,0,0)){
                                scolor("text");
                                goto code_sy;
                            }
                        }else if(p.x>=800&&p.x<=840&&p.y>=225&&p.y<=265){
                            if(InputBox(c,20,"请输入线条颜色的编号：\n\n黑：0\t\t白：1\n红：2\t\t橙：3\n黄：4\t\t绿：5\n蓝：6\t\t灰：7\n棕：8\t\t紫：9\n\n自定义：'#'+16进制颜色编码\n(示例：#ff0033)","线条颜色",NULL,0,0,0)){
                                scolor("line");
                                goto code_sy;
                            }
                        }else if(p.x>=800&&p.x<=840&&p.y>=285&&p.y<=325){
                            if(InputBox(c,20,"请输入倒计时进度条颜色的编号：\n\n黑：0\t\t白：1\n红：2\t\t橙：3\n黄：4\t\t绿：5\n蓝：6\t\t灰：7\n棕：8\t\t紫：9\n\n自定义：'#'+16进制颜色编码\n(示例：#ff0033)","倒计时进度条颜色",NULL,0,0,0)){
                                scolor("time");
                                goto code_sy;
                            }
                        }else if(p.x>=760&&p.x<=840&&p.y>=345&&p.y<=385){
                            if(InputBox(c,10,"请输入游戏回合时间(3~90)：\n\n(建议为90的因数，否则倒计时进度条可能会出现显示问题)","游戏回合时间",NULL,0,0,0)){
                                t=atoi(c);
                                if(t<3||t>90){
                                    MessageBox(GetHWnd(),"超出范围！","错误",MB_ICONERROR);
                                }else{
                                    time3=t;
                                    itoa(time3,c,10);
                                    st=c;
                                    st+="s";
                                    outtextxy(695,350,st.c_str());
                                    goto code_sy;
                                }
                            }
                        }else if(p.x>=760&&p.x<=840&&p.y>=525&&p.y<=565){
                            st="说明：设置 AI 模式默认密钥后，AI模式中将自动使用该密钥。\n\n\n当前的 AI 模式默认密钥为：";
                            if(is_keymr){
                                st+=keymr;
                            }else{
                                st+="（空）";
                            }
                            st+="\n\n请输入新的 AI 模式默认密钥，将自动检查:";
                            if(InputBox(c,45,st.c_str(),"AI模式默认密钥",NULL,0,0,0)){
                                st=check_key(c);
                                if(st=="t"){
                                    keymr=c;
                                    is_keymr=1;
                                    MessageBox(GetHWnd(),"设置成功！","AI模式默认密钥",MB_OK|MB_ICONINFORMATION);
                                    goto code_sy;
                                }else if(st=="f"){
                                    MessageBox(GetHWnd(),"密钥无效！","AI模式默认密钥",MB_OK|MB_ICONERROR);
                                }else{
                                    st2="检查密钥过程中出现错误！\n错误内容："+utf8_to_gb2312(st);
                                    MessageBox(GetHWnd(),st2.c_str(),"AI模式默认密钥",MB_OK|MB_ICONERROR);
                                }
                            }
                        }else if(p.x>=760&&p.x<=840&&p.y>=405&&p.y<=445){
                            s1=!s1;
                            refreshs1();
                        }else if(p.x>=760&&p.x<=840&&p.y>=465&&p.y<=505){
                            s2=!s2;
                            refreshs2();
                        }
                    }
                }
            }else if(sjudge(3)){
                cleard();
                line(0,50,900,50);
                line(20,25,40,5);
                line(20,25,40,45);
                line(20,25,70,25);
                roundrect(200,120,700,240,30,30);
                roundrect(200,300,700,420,30,30);
                settextstyle(40,0,"msyh");
                outtextxy(100,5,"游戏帮助");
                outtextxy(450-textwidth("（此页面持续更新中）")/2,500,"（此页面持续更新中）");
                settextstyle(70,0,"msyh");
                outtextxy(450-textwidth("玩法介绍")/2,140,"玩法介绍");
                outtextxy(450-textwidth("关于游戏")/2,320,"关于游戏");
                while(true){
                    if(KEY_DOWN(L_MOUSE)){
                        while(KEY_DOWN(L_MOUSE));
                        getmouse(&p);
                         if(p.x>=20&&p.x<=70&&p.y>=5&&p.y<=45){
                            goto code_s;
                        }else if(p.x>=200&&p.x<=700&&p.y>=120&&p.y<=240){
                            MessageBox(GetHWnd(),"按鼠标左键即可落子，黑方先手，轮流落子，率先连成5个子的一方获胜。注意每回合有时间限制，超时将判定对方胜利。\n\n棋盘背景、线条、字体的颜色和每回合时限均可在\"游戏设置\"中个性化调整。","玩法介绍",MB_ICONINFORMATION);
                        }else if(p.x>=200&&p.x<=700&&p.y>=300&&p.y<=420){
                            st="五子棋 Gobang\n\n版本:";
                            st+=ver;
                            st+="\n作者:Wzh\n联系邮箱：wzh_2011@139.com\nGitHub项目地址：https://github.com/Ball-aabb/Gobang\n\n更多内容，详见GitHub";
                            MessageBox(GetHWnd(),st.c_str(),"关于游戏",MB_ICONINFORMATION);
                        }
                    }
                }
            }else if(sjudge(4)){
                fout.open("saves.txt",ios::app);
                fout<<time(0)-time_start<<' '<<bwin<<' '<<wwin<<' '<<time_gt<<endl;
                fout.close();
                exit(0);
            }else if(p.x>=40&&p.x<=220&&p.y>=25&&p.y<=85){
                st="本次游戏记录：\n\n总局数：";
                itoa(bwin+wwin,c,10);
                st+=c;
                st+="\n黑方胜利局数：";
                itoa(bwin,c,10);
                st+=c;
                st+="\n白方胜利局数：";
                itoa(wwin,c,10);
                st+=c;
                st+="\n登录时长：";
                itoa((time(0)-time_start)/60,c,10);
                st+=c;
                st+="min";
                itoa((time(0)-time_start)%60,c,10);
                st+=c;
                st+="s\n游戏时长：";
                itoa(time_gt/60,c,10);
                st+=c;
                st+="min";
                itoa(time_gt%60,c,10);
                st+=c;
                st+="s\n----------------------------------------\n本地游戏历史记录(不含本次)：\n\n";
                fin.open("saves.txt");
                if(fin.is_open()){
                    st+="总登录次数：";
                    int ta,tb,tc,td,ttn=0,ttb=0,ttw=0,tt_time=0,tt_gtime=0;
                    while(fin>>ta>>tb>>tc>>td){
                        ttn++;
                        tt_time+=ta,ttb+=tb,ttw+=tc,tt_gtime+=td;
                    }
                    fin.close();
                    itoa(ttn,c,10);
                    st+=c;
                    st+="\n总局数：";
                    itoa(ttb+ttw,c,10);
                    st+=c;
                    st+="\n黑方胜利局数：";
                    itoa(ttb,c,10);
                    st+=c;
                    st+="\n白方胜利局数：";
                    itoa(ttw,c,10);
                    st+=c;
                    st+="\n总登录时长：";
                    itoa(tt_time/60,c,10);
                    st+=c;
                    st+="min";
                    itoa(tt_time%60,c,10);
                    st+=c;
                    st+="s\n总游戏时长：";
                    itoa(tt_gtime/60,c,10);
                    st+=c;
                    st+="min";
                    itoa(tt_gtime%60,c,10);
                    st+=c;
                    st+="s";
                }else{
                    st+="没有历史记录或数据丢失";
                }
                MessageBox(GetHWnd(),st.c_str(),"游戏记录",MB_ICONINFORMATION);
            }
        }
        //cout<<p.x-660<<' '<<p.y-200<<endl;
        Sleep(60);
    }
    getch();
}
int evaluateThreat(int x,int y,int player){
    if(a[x][y]!=-1){
        return -1;
    }
    int totalScore=0;
    a[x][y]=player;
    for(const auto& dir:dirs){
        int dx=dir[0],dy=dir[1];
        int count=1;
        bool blocked1=false,blocked2=false;
        for(int i=1;i<6;++i){
            int nx=x+dx*i,ny=y+dy*i;
            if(nx<0||nx>=15||ny<0||ny>=15){
                blocked1=true;
                break;
            }
            if(a[nx][ny]!=player){
                if(a[nx][ny]!=-1){
                    blocked1=true;
                }
                break;
            }
            count++;
        }
        for(int i=1;i<6;++i){
            int nx=x-dx*i,ny=y-dy*i;
            if(nx<0||nx>=15||ny<0||ny>=15){
                blocked2=true;
                break;
            }
            if(a[nx][ny]!=player){
                if(a[nx][ny]!=-1){
                    blocked2=true;
                }
                break;
            }
            count++;
        }
        if(count>=5){
            totalScore+=1000000;
        }else if(count==4){
            if(!blocked1&&!blocked2){
                totalScore+=100000;
            }else if(!blocked1||!blocked2){
                totalScore+=10000;
            }
        }else if(count==3){
            if(!blocked1&&!blocked2){
                totalScore+=1000;
            }else if(!blocked1||!blocked2){
                totalScore+=100;
            }
        }else if(count==2){
            if(!blocked1&&!blocked2){
                totalScore+=10;
            }
        }
    }
    a[x][y]=-1;
    return totalScore;
}
int evaluatePosition(int x,int y){
    if(a[x][y]!=-1){
        return -1;
    }
    int score=0;
    int myScore=evaluateThreat(x,y,0);
    int enemyScore=evaluateThreat(x,y,1);
    score=myScore*2+enemyScore*3;
    int centerDist=abs(x-7)+abs(y-7);
    score+=14-centerDist;
    for(int dx=-2;dx<=2;++dx){
        for(int dy=-2;dy<=2;++dy){
            if(dx==0&&dy==0){
                continue;
            }
            int nx=x+dx,ny=y+dy;
            if(nx>=0&&nx<15&&ny>=0&&ny<15){
                if(a[nx][ny]==0){
                    score+=5-abs(dx)-abs(dy);
                }else if(a[nx][ny]==1){
                    score+=3-abs(dx)-abs(dy);
                }
            }
        }
    }
    return score;
}
xy findBestMove(int player,int minScore){
    xy bestMove={-1,-1};
    int bestScore=-1;
    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            if(a[i][j]==-1){
                int score=evaluateThreat(i,j,player);
                if(score>=minScore&&score>bestScore){
                    bestScore=score;
                    bestMove={i,j};
                }
            }
        }
    }
    return bestMove;
}
xy white(){
    if(mode==classic){
        white_classic:
        xy myWin=findBestMove(0,100000);
        if(myWin.x!=-1){
            a[myWin.x][myWin.y]=0;
            return myWin;
        }
        xy enemyWin=findBestMove(1,100000);
        if(enemyWin.x!=-1){
            a[enemyWin.x][enemyWin.y]=0;
            return enemyWin;
        }
        xy myOpenFour=findBestMove(0,10000);
        if(myOpenFour.x!=-1){
            a[myOpenFour.x][myOpenFour.y]=0;
            return myOpenFour;
        }
        xy enemyOpenFour=findBestMove(1,10000);
        if(enemyOpenFour.x!=-1){
            a[enemyOpenFour.x][enemyOpenFour.y]=0;
            return enemyOpenFour;
        }
        xy myThree=findBestMove(0,1000);
        if(myThree.x!=-1){
            a[myThree.x][myThree.y]=0;
            return myThree;
        }
        xy enemyThree=findBestMove(1,1000);
        if(enemyThree.x!=-1){
            a[enemyThree.x][enemyThree.y]=0;
            return enemyThree;
        }
        xy bestMove={7,7};
        int maxScore=-1;
        for(int i=0;i<15;++i){
            for(int j=0;j<15;++j){
                int score=evaluatePosition(i,j);
                if(score>maxScore){
                    maxScore=score;
                    bestMove={i,j};
                }
            }
        }
        Sleep(40);
        return bestMove;
    }else{
        string _st="";
        for(int i=0;i<cc.size();i++){
            _st+='(';
            _st+=itoa(cc[i].x+1,c,10);
            _st+=',';
            _st+=itoa(cc[i].y+1,c,10);
            _st+=')';
        }
        cout<<_st<<' ';
        _st=call_ai(key,"你是一个五子棋超级大师。棋盘上有15行15列的格点，现给出一个由奇数个整数数对组成的列表，依次表示棋盘上已有的棋子的行和列，奇数项是黑子，偶数项是白子，你下白子。请严格按照x,y的格式返回两个有效整数，表示你下的棋子的位置，不要有括号和思考过程。列表:"+st);
        cout<<_st<<endl;
        if(_st[0]=='E'){
            st2=" AI 模式中出现错误！\n错误编码:";
            st2+=_st.substr(1,2);
            st2+="\n错误描述:";
            st2+=utf8_to_gb2312(_st.substr(3));
            st2+="\n\n将自动切换至经典模式。";
            MessageBox(GetHWnd(),st2.c_str(),"错误",MB_OK|MB_ICONERROR);
            mode=classic;
            settextstyle(50,25,"msyh");
            outtextxy(250,110,"人机对战");
            settextstyle(30,0,"msyh");
            goto white_classic;
        }
        stringstream ss(_st);
        int ta,tb;
        ss>>ta>>ct>>tb;
        return {ta-1,tb-1};
    }
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
        setfillcolor(cci[cci.size()-2]?0:WHITE);
        solidcircle(cc[cc.size()-2].x*40+70,cc[cc.size()-2].y*40+170,5);
        setfillcolor(YELLOW);
        solidcircle(x*40+70,y*40+170,5);
        if(b[0]+b[1]>=4){
            for(i=x+1;i<=x+b[0];i++){
                solidcircle(i*40+70,y*40+170,5);
            }
            for(i=x-1;i>=x-b[1];i--){
                solidcircle(i*40+70,y*40+170,5);
            }
        }else if(b[2]+b[3]>=4){
            for(i=y+1;i<=y+b[2];i++){
                solidcircle(x*40+70,i*40+170,5);
            }
            for(i=y-1;i>=y-b[3];i--){
                solidcircle(x*40+70,i*40+170,5);
            }
        }else if(b[4]+b[7]>=4){
            for(i=x+1,j=y+1;i<=x+b[4],j<=y+b[4];i++,j++){
                solidcircle(i*40+70,j*40+170,5);
            }
            for(i=x-1,j=y-1;i>=x-b[7],j>=y-b[7];i--,j--){
                solidcircle(i*40+70,j*40+170,5);
            }
        }else if(b[5]+b[6]>=4){
            for(i=x+1,j=y-1;i<=x+b[5],j>=y-b[5];i++,j--){
                solidcircle(i*40+70,j*40+170,5);
            }
            for(i=x-1,j=y+1;i>=x-b[6],j<=y+b[6];i--,j++){
                solidcircle(i*40+70,j*40+170,5);
            }
        }
        Sleep(2000);
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
    setlinecolor(bkcolor);
    for(int k=1;k<=4;k++){
        if(t!=k){
            sfor(k);
        }
    }
}
void sfor(int k){
    for(int i=1;i<15;i++){
        line(220+49+i,k*150+1,220+i,50+k*150);
        line(220+49+i,k*150+99,220+i,50+k*150);
        line(660-64+i,k*150+1,660-15+i,50+k*150);
        line(660-64+i,k*150+99,660-15+i,50+k*150);
    }
}
void scolor(string type){
    if(c[0]=='#'){
        st=c;
        if(st.size()!=7){
            MessageBox(GetHWnd(),"输入格式错误！","错误",MB_ICONERROR);
            return ;
        }
        for(int i=1;i<=6;i++){
            if(st[i]<'0'||(st[i]>'9'&&st[i]<'a')||st[i]>'f'){
                MessageBox(GetHWnd(),"输入格式错误！","错误",MB_ICONERROR);
                return ;
            }
        }
        t=0;
        for(int i=6;i>=1;i--){
            if(st[i]>='0'&&st[i]<='9'){
                t+=(st[i]-'0')*pow(16,6-i);
            }else{
                t+=(st[i]-'a'+10)*pow(16,6-i);
            }
        }
        if(type=="bk"&&(t==linecolor||t==textcolor||t==timecolor)){
            MessageBox(GetHWnd(),"背景不能与线条或文字或倒计时进度条同色！","错误",MB_ICONERROR);
        }else if(type=="text"&&(t==bkcolor)){
            MessageBox(GetHWnd(),"文字不能与背景同色！","错误",MB_ICONERROR);
        }else if(type=="line"&&(t==bkcolor)){
            MessageBox(GetHWnd(),"线条不能与背景同色！","错误",MB_ICONERROR);
        }else if(type=="time"&&(t==bkcolor)){
            MessageBox(GetHWnd(),"倒计时进度条不能与背景同色！","错误",MB_ICONERROR);
        }else{
            if(type=="bk"){
                bkci=-1;
                bkcolor=t;
            }else if(type=="text"){
                tci=-1;
                textcolor=t;
            }else if(type=="line"){
                lci=-1;
                linecolor=t;
            }else if(type=="time"){
                timeci=-1;
                timecolor=t;
            }
            refresh();
        }
    }else{
        st=c;
        for(int i=0;i<st.size();i++){
            if(st[i]<'0'||st[i]>'9'){
                MessageBox(GetHWnd(),"请检查输入内容！","错误",MB_ICONERROR);
                return ;
            }
        }
        t=atoi(c);
        if(t<0||t>9){
            MessageBox(GetHWnd(),"颜色不存在！","错误",MB_ICONERROR);
        }else{
            if(type=="bk"&&(t==lci||t==tci||t==timeci)){
                MessageBox(GetHWnd(),"背景不能与线条或文字或倒计时进度条同色！","错误",MB_ICONERROR);
            }else if(type=="text"&&(t==bkci)){
                MessageBox(GetHWnd(),"文字不能与背景同色！","错误",MB_ICONERROR);
            }else if(type=="line"&&(t==bkci)){
                MessageBox(GetHWnd(),"线条不能与背景同色！","错误",MB_ICONERROR);
            }else if(type=="time"&&(t==bkci)){
                MessageBox(GetHWnd(),"倒计时进度条不能与背景同色！","错误",MB_ICONERROR);
            }else{
                if(type=="bk"){
                    bkci=t;
                }else if(type=="text"){
                    tci=t;
                }else if(type=="line"){
                    lci=t;
                }else if(type=="time"){
                    timeci=t;
                }
                refresh();
            }
        }
    }
}
bool sjudge(int t){
    if(p.y>=t*150&&p.y<=t*150+100&&p.x>=270&&p.x<=610){
        return 1;
    }
    int ty=p.y-t*150-50,tx;
    if(p.y>=t*150&&p.y<=t*150+50&&p.x>=220&&p.x<=270){
        tx=p.x-220;
        return (tx+ty>=0);
    }else if(p.y>=t*150+50&&p.y<=t*150+100&&p.x>=220&&p.x<=270){
        tx=p.x-220;
        return (tx>=ty);
    }else if(p.y>=t*150&&p.y<=t*150+50&&p.x>=610&&p.x<=660){
        tx=p.x-660;
        return (tx<=ty);
    }else if(p.y>=t*150+50&&p.y<=t*150+100&&p.x>=610&&p.x<=660){
        tx=p.x-660;
        return (tx+ty<=0);
    }
    return 0;
}
void refresh(){
    if(bkci!=-1)bkcolor=colorl[bkci];
    if(lci!=-1)linecolor=colorl[lci];
    if(tci!=-1)textcolor=colorl[tci];
    if(timeci!=-1)timecolor=colorl[timeci];
    _setbkcolor(bkcolor);
    setlinecolor(linecolor);
    settextcolor(textcolor);
}
void refreshs1(){
    if(s1==0){
        setfillcolor(RGB(190,190,190));
        fillrectangle(760,405,840,445);
        setfillcolor(WHITE);
        fillrectangle(795,410,765,440);
    }else{
        setfillcolor(GREEN);
        fillrectangle(760,405,840,445);
        setfillcolor(WHITE);
        fillrectangle(805,410,835,440);
    }
}
void refreshs2(){
    if(s2==0){
        setfillcolor(RGB(190,190,190));
        fillrectangle(760,465,840,505);
        setfillcolor(WHITE);
        fillrectangle(795,470,765,500);
    }else{
        setfillcolor(GREEN);
        fillrectangle(760,465,840,505);
        setfillcolor(WHITE);
        fillrectangle(805,470,835,500);
    }
}
int stime(){
    if(time(0)-time1>=1){
        time2--;
        time1=time(0);
        setfillcolor(bkcolor);
        solidrectangle(846-(time3-time2)*(90/time3),26,845+(90/time3)-(time3-time2)*(90/time3),34);
        if(time2<=0){
            outtextxy(770,140," 0 s");
            if(hh){
                bwin++;
                st="黑方胜利！\n\n\n要再来一局吗？";
            }else{
                wwin++;
                st="白方胜利！\n\n\n要再来一局吗？";
            }
            if(MessageBox(GetHWnd(),st.c_str(),"游戏结束",MB_OKCANCEL|MB_ICONINFORMATION)==1){
                return 1;
            }else{
                time_gt=time(0)-time_gstart;
                return 0;
            }
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
    return -1;
}